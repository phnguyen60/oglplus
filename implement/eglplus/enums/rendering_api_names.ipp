//  File implement/eglplus/enums/rendering_api_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/rendering_api.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2014 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC CStrRef ValueName_(
	RenderingAPI*,
	EGLenum value
) noexcept
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_RENDERINGAPI)
#define EGLPLUS_IMPL_EVN_RENDERINGAPI
{
switch(value)
{
#if defined EGL_OPENGL_API
	case EGL_OPENGL_API: return CStrRef("OPENGL_API");
#endif
#if defined EGL_OPENGL_ES_API
	case EGL_OPENGL_ES_API: return CStrRef("OPENGL_ES_API");
#endif
#if defined EGL_OPENVG_API
	case EGL_OPENVG_API: return CStrRef("OPENVG_API");
#endif
#if defined EGL_NONE
	case EGL_NONE: return CStrRef("NONE");
#endif
	default:;
}
EGLPLUS_FAKE_USE(value);
return CStrRef();
}
#else
;
#endif
} // namespace enums

