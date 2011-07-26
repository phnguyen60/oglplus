/**
 *  @file oglplus/transform_feedback.hpp
 *  @brief TransformFeedback object wrappers
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_TRANSFORM_FEEDBACK_1107121519_HPP
#define OGLPLUS_TRANSFORM_FEEDBACK_1107121519_HPP

#include <oglplus/error.hpp>
#include <oglplus/object.hpp>
#include <oglplus/friend_of.hpp>
#include <cassert>

namespace oglplus {

class TransformFeedbackOps
 : public Named
{
protected:
	static void _init(GLsizei count, GLuint& _name)
	{
		::glGenTransformFeedbacks(count, &_name);
		ThrowOnError(OGLPLUS_ERROR_INFO(GenTransformFeedbacks));
	}

	static void _cleanup(GLsizei count, GLuint& _name)
	{
		assert(_name != 0);
		::glDeleteTransformFeedbacks(count, &_name);
	}

	static GLboolean _is_x(GLuint _name)
	{
		assert(_name != 0);
		return ::glIsTransformFeedback(_name);
	}

	friend class FriendOf<TransformFeedbackOps>;
public:
	enum class Target : GLenum {
		Default = GL_TRANSFORM_FEEDBACK
	};

	void Bind(Target target = Target::Default) const
	{
		assert(_name != 0);
		::glBindTransformFeedback(GLenum(target), _name);
		AssertNoError(OGLPLUS_ERROR_INFO(BindTransformFeedback));
	}

	static void Unbind(Target target = Target::Default)
	{
		::glBindTransformFeedback(GLenum(target), 0);
		AssertNoError(OGLPLUS_ERROR_INFO(BindTransformFeedback));
	}
};

typedef Object<TransformFeedbackOps, true> TransformFeedback;

} // namespace oglplus

#endif // include guard