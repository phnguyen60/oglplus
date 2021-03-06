/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_LimitQueries
namespace oglplus {
namespace context {

class LimitQueries
{
public:
	static GLint IntLimit(__LimitQuery query); /*<
	Gets the implementation-dependent integer limit value.
	See [glfunc Get].
	>*/
	static GLint IntLimit(__LimitQuery query, GLuint index); /*<
	Gets the implementation-dependent integer indexed limit value.
	See [glfunc Get].
	>*/
	static GLfloat FloatLimit(__LimitQuery query); /*<
	Gets the implementation-dependent floating-point limit value.
	See [glfunc Get].
	>*/
#if GL_VERSION_4_1 || GL_ARB_viewport_array
	static GLfloat FloatLimit(__LimitQuery query, GLuint index); /*<
	Gets the implementation-dependent floating-point indexed limit value.
	See [glfunc Get].
	>*/
#endif

	static void RequireAtLeast(__LimitQuery limit, GLint value); /*<
	Raises a __LimitError if [^value] is greater than the specified [^limit].
	>*/
	static void RequireAtLeast(__LimitQuery limit, GLuint index, GLint value);
};

} // namespace context
} // namespace oglplus
//]

