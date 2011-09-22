/**
 *  @example oglplus/024_simple_picking.cpp
 *  @brief Shows how to do picking/hiliting of objects by mouse
 *
 *  @image html 024_simple_picking.png
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>

#include <map>
#include <cmath>

#include "example.hpp"

namespace oglplus {

class PickingExample : public Example
{
private:
	// helper object building cube vertex attributes
	shapes::Cube make_cube;
	// helper object encapsulating cube drawing instructions
	shapes::DrawingInstructions cube_instr;
	// indices pointing to cube primitive elements
	shapes::Cube::IndexArray cube_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Geometry shader
	GeometryShader gs;

	// Fragment shader
	FragmentShader fs;

	// Drawing and picking programs
	Program draw_prog, pick_prog;

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertices
	Buffer verts;

	// Transform feedback object
	TransformFeedback tfb;

	// VBO for the values returned by transform feedback
	Buffer picked_instances;

	// the pair of picked instance depth and id
	typedef std::pair<const GLfloat, GLint> DepthAndID;
public:
	PickingExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	{
		// Set the vertex shader source
		vs.Source(
			"#version 330\n"
			"uniform mat4 projectionMatrix, cameraMatrix;"
			"in vec4 vertex;"
			"out vec3 color;"
			"flat out int inst;"
			"void main(void)"
			"{"
			"	float x = gl_InstanceID % 6 - 2.5;"
			"	float y = gl_InstanceID / 6 - 2.5;"
			"	mat4 modelMatrix = mat4("
			"		 1.0, 0.0, 0.0, 0.0,"
			"		 0.0, 1.0, 0.0, 0.0,"
			"		 0.0, 0.0, 1.0, 0.0,"
			"		 2*x, 2*y, 0.0, 1.0 "
			"	);"
			"	gl_Position = "
			"		projectionMatrix *"
			"		cameraMatrix *"
			"		modelMatrix *"
			"		vertex;"
			"	color = vec3("
			"		abs(normalize((modelMatrix*vertex).xy)),"
			"		0.1"
			"	);"
			"	inst = gl_InstanceID;"
			"}"
		);
		vs.Compile();

		// Set the geometry shader source
		gs.Source(
			"#version 330\n"
			"layout(triangles) in;"
			"layout(points, max_vertices = 1) out;"

			"flat in int inst[];"
			"uniform vec2 mousePos;"

			"out int instance;"
			"out float depth;"

			"vec2 barycentric_coords(vec4 a, vec4 b, vec4 c)"
			"{"
			// we'll need normalized device coordinates
			// of the triangle vertices
			"	vec2 ad = vec2(a.x/a.w, a.y/a.w);"
			"	vec2 bd = vec2(b.x/b.w, b.y/b.w);"
			"	vec2 cd = vec2(c.x/c.w, c.y/c.w);"
			"	vec2 u = cd - ad;"
			"	vec2 v = bd - ad;"
			"	vec2 r = mousePos - ad;"
			"	float d00 = dot(u, u);"
			"	float d01 = dot(u, v);"
			"	float d02 = dot(u, r);"
			"	float d11 = dot(v, v);"
			"	float d12 = dot(v, r);"
			"	float id = 1.0 / (d00 * d11 - d01 * d01);"
			"	float ut = (d11 * d02 - d01 * d12) * id;"
			"	float vt = (d00 * d12 - d01 * d02) * id;"
			"	return vec2(ut, vt);"
			"}"

			"vec3 intersection(vec3 a, vec3 b, vec3 c, vec2 bc)"
			"{"
			"	return (c - a)*bc.x + (b - a)*bc.y;"
			"}"

			"bool inside_triangle(vec2 b)"
			"{"
			"	return ("
			"		(b.x >= 0.0) &&"
			"		(b.y >= 0.0) &&"
			"		(b.x + b.y <= 1.0)"
			"	);"
			"}"

			"void main(void)"
			"{"
			"	vec2 bc = barycentric_coords("
			"		gl_in[0].gl_Position,"
			"		gl_in[1].gl_Position,"
			"		gl_in[2].gl_Position "
			"	);"
			"	if(inside_triangle(bc))"
			"	{"
			"		gl_Position = vec4(intersection("
			"			gl_in[0].gl_Position.xyz,"
			"			gl_in[1].gl_Position.xyz,"
			"			gl_in[2].gl_Position.xyz,"
			"			bc"
			"		), 1.0);"
			"		depth = gl_Position.z;"
			"		instance = inst[0];"
			"		EmitVertex();"
			"		EndPrimitive();"
			"	}"
			"}"
		);
		gs.Compile();

		// set the fragment shader source
		fs.Source(
			"#version 330\n"
			"flat in int inst;"
			"in vec3 color;"
			"uniform int picked;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	if(inst == picked)"
			"		fragColor = vec4(1.0, 1.0, 1.0, 1.0);"
			"	else fragColor = vec4(color, 1.0);"
			"}"
		);
		fs.Compile();

		// attach the shaders to the picking program
		pick_prog.AttachShader(vs);
		pick_prog.AttachShader(gs);
		pick_prog.TransformFeedbackVaryings(
			{"depth", "instance"},
			TransformFeedbackMode::InterleavedAttribs
		);
		pick_prog.Link();

		// attach the shaders to the drawing program
		draw_prog.AttachShader(vs);
		draw_prog.AttachShader(fs);
		draw_prog.Link();

		// bind the VAO for the cube
		cube.Bind();

		// buffer for the picked instance depths and IDs
		picked_instances.Bind(Buffer::Target::TransformFeedback);
		picked_instances.BindBase(
			Buffer::IndexedTarget::TransformFeedback,
			0
		);
		{
			std::vector<DepthAndID> data(36);
			Buffer::Data(Buffer::Target::TransformFeedback, data);
		}

		// bind the VBO for the cube vertices
		verts.Bind(Buffer::Target::Array);
		{
			// make and upload the vertex data
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Vertices(data);
			Buffer::Data(Buffer::Target::Array, data);

			// setup the vertex attribs array for the vertices
			draw_prog.Use();
			VertexAttribArray draw_attr(draw_prog, "vertex");
			draw_attr.Setup(n_per_vertex, DataType::Float);
			draw_attr.Enable();

			pick_prog.Use();
			VertexAttribArray pick_attr(pick_prog, "vertex");
			pick_attr.Setup(n_per_vertex, DataType::Float);
			pick_attr.Enable();
		}

		gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		gl.FrontFace(make_cube.FaceWinding());
		gl.CullFace(Face::Back);
		gl.Enable(Capability::CullFace);
	}

	void Reshape(size_t width, size_t height)
	{
		gl.Viewport(width, height);
		auto perspective = CamMatrixf::Perspective(
			Degrees(24),
			double(width)/height,
			1, 100
		);
		draw_prog.Use();
		Uniform(draw_prog, "projectionMatrix").SetMatrix(perspective);
		pick_prog.Use();
		Uniform(pick_prog, "projectionMatrix").SetMatrix(perspective);
	}

	// we want to get mouse motion notifications
	bool UsesMouseMotion(void) const
	{
		return true;
	}

	// called when the mouse pointer is moved; x and y are between -1 and 1
	void MouseMoveNormalized(float x, float y, float aspect)
	{
		pick_prog.Use();
		Uniform(pick_prog, "mousePos").Set(Vec2f(x*aspect, y));
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			10.0,
			FullCircles(time * 0.1),
			Degrees(std::sin(time * 0.3) * 30)
		);

		// use the picking program
		pick_prog.Use();
		Uniform(pick_prog, "cameraMatrix").SetMatrix(camera);

		// query the number of values written to the feedabck buffer
		GLuint picked_count = 0;
		{
			Query count_query;
			auto query_exec = count_query.Execute(
				Query::Target::
				TransformFeedbackPrimitivesWritten,
				picked_count
			);
			TransformFeedback::Activator activates_tfb(
				TransformFeedback::PrimitiveType::Points
			);
			// draw 36 instances of the cube
			// the vertex shader will take care of their placement
			cube_instr.Draw(cube_indices, 36);
		}

		std::map<GLfloat, GLint> picked;
		{
			picked_instances.Bind(Buffer::Target::TransformFeedback);
			Buffer::TypedMap<DepthAndID> picked_instances_map(
				Buffer::Target::TransformFeedback,
				Buffer::MapAccess::Read
			);
			picked.insert(
				picked_instances_map.Data(),
				picked_instances_map.Data()+picked_count
			);
		}

		draw_prog.Use();

		if(picked.empty())
			Uniform(draw_prog, "picked").Set(-1);
		else Uniform(draw_prog, "picked").Set(picked.begin()->second);
		Uniform(draw_prog, "cameraMatrix").SetMatrix(camera);

		// draw 36 instances of the cube
		// the vertex shader will take care of their placement
		cube_instr.Draw(cube_indices, 36);
	}

	bool Continue(double time)
	{
		return time < 30.0;
	}
};

std::unique_ptr<Example> makeExample(void)
{
	return std::unique_ptr<Example>(new PickingExample);
}

} // namespace oglplus