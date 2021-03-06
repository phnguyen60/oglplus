//  File doc/quickbook/oglplus/quickref/enums/transform_feedback_mode.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/transform_feedback_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2014 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_transform_feedback_mode
namespace oglplus {

enum class TransformFeedbackMode : GLenum
{
	InterleavedAttribs = GL_INTERLEAVED_ATTRIBS,
	SeparateAttribs    = GL_SEPARATE_ATTRIBS
};

template <>
__Range<TransformFeedbackMode> __EnumValueRange<TransformFeedbackMode>(void);

__StrCRef __EnumValueName(TransformFeedbackMode);

} // namespace oglplus
//]
