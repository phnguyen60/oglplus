/**
 *  @example standalone/002_triangle_compatibility.cpp
 *  @brief Shows usage of the ARB_compatibility wrapper
 *
 *  @image html 002_triangle.png
 *
 *  Copyright 2008-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <oglplus/all.hpp>
#include <oglplus/ext/ARB_compatibility.hpp>

class Example
{
private:
	oglplus::Context gl;
	oglplus::ARB_compatibility glc;

public:
	Example(size_t width, size_t height)
	{
		using namespace oglplus;

		gl.ClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		gl.Viewport(width, height);

		glc.MatrixMode(MatrixMode::Projection);
		glc.LoadIdentity();
		glc.MatrixMode(MatrixMode::Modelview);
		glc.LoadIdentity();
	}

	void Display(void)
	{
		using namespace oglplus;

		gl.Clear().ColorBuffer();

		glc.Begin(CompatibilityPrimitiveType::Triangles);
			glc.Color(1.0, 0.0, 0.0);
			glc.Vertex(0.0f, 0.0f);

			glc.Color(0.0, 1.0, 0.0);
			glc.Vertex(1.0f, 0.0f);

			glc.Color(0.0, 0.0, 1.0);
			glc.Vertex(0.0f, 0.1f);
		glc.End();
	}
};

class SingleExample
{
private:
	static Example*& SingleInstance(void)
	{
		static Example* test = nullptr;
		return test;
	}
public:
	SingleExample(const SingleExample&) = delete;

	SingleExample(size_t width, size_t height)
	{
		assert(!SingleInstance());
		SingleInstance() = new Example(width, height);
	}

	~SingleExample(void)
	{
		assert(SingleInstance());
		delete SingleInstance();
		SingleInstance() = nullptr;
	}

	static void Display(void)
	{
		assert(SingleInstance());
		SingleInstance()->Display();
		glutSwapBuffers();
	}
};

int main(int argc, char* argv[])
{
	size_t width = 800, height = 600;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OGLplus+GLUT+GLEW");

	if(glewInit() == GLEW_OK) try
	{
		SingleExample example(width, height);
		glutDisplayFunc(&SingleExample::Display);
		glutMainLoop();
		return 0;
	}
	catch(oglplus::Error& err)
	{
		std::cerr <<
			"Error (in gl" << err.GLSymbol() << ", " <<
			err.ClassName() << ": '" <<
			err.ObjectDescription() << "'): " <<
			err.what() <<
			" [" << err.File() << ":" << err.Line() << "] ";
		std::cerr << std::endl;
		err.Cleanup();
	}
	catch(std::exception& se)
	{
		std::cerr << "Error: " << se.what() << std::endl;
	}
	return 1;
}
