/**
 *  @file oglplus/buffer.hpp
 *  @brief Buffer wrappers
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_BUFFER_1107121519_HPP
#define OGLPLUS_BUFFER_1107121519_HPP

#include <oglplus/error.hpp>
#include <oglplus/object.hpp>
#include <oglplus/friend_of.hpp>
#include <cassert>

namespace oglplus {

/// Wrapper for OpenGL buffer operations
/**
 *  @note Do not use this class directly, use Buffer instead
 *
 *  @see Buffer
 */
class BufferOps
 : public Named
{
protected:
	static void _init(GLsizei count, GLuint& _name)
	{
		::glGenBuffers(count, &_name);
		ThrowOnError(OGLPLUS_ERROR_INFO(GenBuffers));
	}

	static void _cleanup(GLsizei count, GLuint& _name)
	{
		assert(_name != 0);
		::glDeleteBuffers(count, &_name);
	}

	static GLboolean _is_x(GLuint _name)
	{
		assert(_name != 0);
		return ::glIsBuffer(_name);
	}

	friend class FriendOf<BufferOps>;
public:
	/// Buffer bind targets
	enum class Target : GLenum {
		Array = GL_ARRAY_BUFFER,
		CopyRead = GL_COPY_READ_BUFFER,
		CopyWrite = GL_COPY_WRITE_BUFFER,
		DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
		ElementArray = GL_ELEMENT_ARRAY_BUFFER,
		PixelPack = GL_PIXEL_PACK_BUFFER,
		PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
		TextureBuffer = GL_TEXTURE_BUFFER,
		TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
		Uniform = GL_UNIFORM_BUFFER
	};

	/// Buffer usage
	enum class Usage : GLenum {
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY,
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY
	};

	/// Mapping of the buffer to the client address space
	class Map
	{
	public:
		/// Mapped data access types
		enum class Access : GLbitfield {
			Read = GL_MAP_READ_BIT,
			Write = GL_MAP_WRITE_BIT,
			ReadWrite = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT
		};
	private:
		const GLintptr _offset;
		GLsizeiptr _size;
		GLvoid* _ptr;
		const Target _target;

		static GLsizeiptr _get_size(Target target)
		{
			GLint value = 0;
			::glGetBufferParameteriv(
				GLenum(target),
				GL_BUFFER_SIZE,
				&value
			);
			ThrowOnError(OGLPLUS_ERROR_INFO(GetBufferParameteriv));
			return GLsizeiptr(value);
		}

		static GLenum _translate(Access access)
		{
			switch(access)
			{
				case Access::Read: return GL_READ_ONLY;
				case Access::Write: return GL_WRITE_ONLY;
				case Access::ReadWrite: return GL_READ_WRITE;
			}
		}
	public:
		Map(Target target, GLintptr offset, GLsizeiptr size, Access access)
		 : _offset(offset)
		 , _size(size)
		 , _ptr(
			::glMapBufferRange(
				GLenum(target),
				offset,
				size,
				GLbitfield(access)
			)
		), _target(target)
		{
			ThrowOnError(OGLPLUS_ERROR_INFO(MapBufferRange));
		}

		Map(Target target, Access access)
		 : _offset(0)
		 , _size(_get_size(target))
		 , _ptr(::glMapBuffer(GLenum(target), _translate(access)))
		 , _target(target)
		{
			ThrowOnError(OGLPLUS_ERROR_INFO(MapBuffer));
		}

		Map(const Map&) = delete;
		Map(Map&& temp)
		 : _offset(temp._offset)
		 , _size(temp._size)
		 , _ptr(temp._ptr)
		 , _target(temp._target)
		{
			temp._ptr = nullptr;
		}

		~Map(void)
		{
			if(_ptr != nullptr) ::glUnmapBuffer(GLenum(_target));
		}

		/// Returns the size in bytes of the mapped buffer
		GLsizeiptr Size(void) const
		{
			return _size;
		}

		/// Returns the pointer to the mapped data
		void* Data(void) const
		{
			return _ptr;
		}

		template <typename T>
		T& At(GLuint index) const
		{
			assert(_ptr != nullptr);
			assert(((index + 1) * sizeof(T)) <= _size);
			return ((T*)_ptr)[index];
		}
	};

	/// Bind this buffer to the specified target
	/**
	 *  @throws Error
	 */
	void Bind(Target target) const
	{
		assert(_name != 0);
		::glBindBuffer(GLenum(target), _name);
		AssertNoError(OGLPLUS_ERROR_INFO(BindBuffer));
	}

	/// Unbind the current buffer from the specified target
	/**
	 *  @throws Error
	 */
	static void Unbind(Target target)
	{
		::glBindBuffer(GLenum(target), 0);
		AssertNoError(OGLPLUS_ERROR_INFO(BindBuffer));
	}

	/// Uploads (sets) the buffer data
	/**
	 *  @throws Error
	 */
	template <typename GLtype>
	static void Data(
		Target target,
		GLtype* data,
		GLsizei count,
		Usage usage = Usage::StaticDraw
	)
	{
		::glBufferData(
			GLenum(target),
			count * sizeof(GLtype),
			data,
			GLenum(usage)
		);
		ThrowOnError(OGLPLUS_ERROR_INFO(BufferData));
	}

	/// Uploads (sets) the buffer data
	/**
	 *  @throws Error
	 */
	template <typename GLtype>
	static void Data(
		Target target,
		const std::vector<GLtype>& data,
		Usage usage = Usage::StaticDraw
	)
	{
		::glBufferData(
			GLenum(target),
			data.size() * sizeof(GLtype),
			data.data(),
			GLenum(usage)
		);
		ThrowOnError(OGLPLUS_ERROR_INFO(BufferData));
	}
};

#ifdef OGLPLUS_DOCUMENTATION_ONLY
/// Encapsulates the OpenGL buffer-related functionality
class Buffer
 : public BufferOps
{ };
#else
typedef Object<BufferOps, true> Buffer;
#endif

} // namespace oglplus

#endif // include guard