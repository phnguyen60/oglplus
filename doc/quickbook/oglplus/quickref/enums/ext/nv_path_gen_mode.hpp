//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_gen_mode.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_gen_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2014 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_gen_mode
namespace oglplus {

enum class PathNVGenMode : GLenum
{
	None              = GL_NONE,
	EyeLinear         = GL_EYE_LINEAR,
	ObjectLinear      = GL_OBJECT_LINEAR,
	ObjectBoundingBox = GL_PATH_OBJECT_BOUNDING_BOX_NV
};

template <>
__Range<PathNVGenMode> __EnumValueRange<PathNVGenMode>(void) noexcept;

__CStrRef __EnumValueName(PathNVGenMode) noexcept;

} // namespace oglplus
//]
