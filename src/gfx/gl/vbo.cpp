/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cppual/gfx/draw.h>
#include <cppual/gfx/gl/vbo.h>
#include "gldef.h"

namespace cppual { namespace Graphics { namespace GL {

inline uint getAccessMode (AccessMode eMode) noexcept
{
	switch (eMode)
	{
	case AccessMode::Read:
		return GL_READ_ONLY;
	case AccessMode::Write:
		return GL_WRITE_ONLY;
	case AccessMode::ReadWrite:
		return GL_READ_WRITE;
	}

	return 0;
}

inline uint getBufferType (BufferType eType) noexcept
{
	switch (eType)
	{
	case BufferType::Array:
		return GL_ARRAY_BUFFER;
	case BufferType::AtomicCounter:
		return GL_ATOMIC_COUNTER_BUFFER;
	case BufferType::CopyRead:
		return GL_COPY_READ_BUFFER;
	case BufferType::CopyWrite:
		return GL_COPY_WRITE_BUFFER;
	case BufferType::DispatchIndirect:
		return GL_DISPATCH_INDIRECT_BUFFER;
	case BufferType::DrawIndirect:
		return GL_DRAW_INDIRECT_BUFFER;
	case BufferType::ElementArray:
		return GL_ELEMENT_ARRAY_BUFFER;
	case BufferType::PixelPack:
		return GL_PIXEL_PACK_BUFFER;
	case BufferType::PixelUnpack:
		return GL_PIXEL_UNPACK_BUFFER;
	case BufferType::Query:
		return GL_QUERY_BUFFER;
	case BufferType::ShaderStorage:
		return GL_SHADER_STORAGE_BUFFER;
	case BufferType::Texture:
		return GL_TEXTURE_BUFFER;
	case BufferType::TransformFeedback:
		return GL_TRANSFORM_FEEDBACK_BUFFER;
	case BufferType::Uniform:
		return GL_UNIFORM_BUFFER;
	}

	return 0;
}

inline static uint getUsageType (UsageType eType) noexcept
{
	switch (eType)
	{
	case UsageType::StreamRead:
		return GL_STREAM_READ;
	case UsageType::StreamCopy:
		return GL_STREAM_COPY;
	case UsageType::StreamDraw:
		return GL_STREAM_DRAW;
	case UsageType::StaticRead:
		return GL_STATIC_READ;
	case UsageType::StaticCopy:
		return GL_STATIC_COPY;
	case UsageType::StaticDraw:
		return GL_STATIC_DRAW;
	case UsageType::DynamicRead:
		return GL_DYNAMIC_READ;
	case UsageType::DynamicCopy:
		return GL_DYNAMIC_COPY;
	case UsageType::DynamicDraw:
		return GL_DYNAMIC_DRAW;
	}

	return 0;
}

// ====================================================

VertexBuffer::VertexBuffer (size_type uSize) noexcept
: m_gData (),
  m_eType (),
  m_bIsUploaded ()
{
	m_gData.reserve (uSize);
}

void* VertexBuffer::mapBufferToMemory (AccessMode eMode) noexcept
{
	if (!m_bIsUploaded) return nullptr;
	return IDeviceContext::current ()->version () < 3 ?
				glMapBuffer (getBufferType (m_eType), getAccessMode (eMode)) :
				glMapBufferARB (getBufferType (m_eType), getAccessMode (eMode));
}

void* VertexBuffer::mapSubBufferToMemory (ptrdiff	 uOffset,
										  ptrdiff	 uLen,
										  GLMapFlags gAccesFlags) noexcept
{
	if (!m_bIsUploaded) return nullptr;
	return glMapBufferRange (getBufferType (m_eType),
							 uOffset,
							 uLen,
							 gAccesFlags);
}

void VertexBuffer::unmapBuffer () noexcept
{
	if (id ()) glUnmapBuffer (getBufferType (m_eType));
}

void VertexBuffer::bind (BufferType eType) noexcept
{
	if (id ())
	{
		m_eType = eType;
		glBindBuffer (getBufferType (eType), id ());
	}
}

void VertexBuffer::upload (UsageType eUsage)
{
	if (id ())
	{
		glBufferData (getBufferType (m_eType),
					  (ptrdiff) m_gData.size (),
					  &m_gData[0],
					  getUsageType (eUsage));

		m_bIsUploaded = true;
		m_gData.clear ();
	}
}

void VertexBuffer::addData (pointer pData, size_type uDataSize)
{
	if (id ())
	{
		m_gData.insert (m_gData.end (), pData, pData + uDataSize);
		m_bIsUploaded = false;
	}
}

// ====================================================

VertexArray::VertexArray () noexcept
: Object (ResourceType::Macro)
{ }

} } } // namespace GL
