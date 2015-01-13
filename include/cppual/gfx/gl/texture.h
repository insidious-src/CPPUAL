/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#ifndef CPPUAL_GFX_GL_TEXTURE_H_
#define CPPUAL_GFX_GL_TEXTURE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>
#include <cppual/gfx/gl/glbase.h>

using std::string;

namespace cppual { namespace Graphics { namespace GL {

class Texture : public Object, ITransformable2D
{
public:
	enum class MinFilter : unsigned char
	{
		Nearest,
		Bilinear,
		NearestMipMap,
		BilinearMipMap,
		TrilinearMipMap
	};

	enum class MagFilter : unsigned char
	{
		Nearest,
		Bilinear
	};

	enum class State : unsigned char
	{
		DirectAccess
	};

	Texture () noexcept;
	~Texture () noexcept;
	static void setState (State type, bool state) noexcept;
	//bool loadTexture2D (RasterImage const& image, bool gen_mipmap = false);
	void setParameter (uint name, int param) noexcept;
	void bind (uint texture_id) noexcept;
	void setFiltering (MagFilter mag, MinFilter min) noexcept;
	void release () noexcept;

	Texture (string      const& file,
			 PixelFormat const& format     = PixelFormat (),
			 bool               gen_mipmap = false);

	bool loadTexture2D (cvoid*             pixels,
						point2i     const& size,
						PixelFormat const& format     = PixelFormat (),
						bool               gen_mipmap = false);

	bool loadTexture2D (string      const& path,
						PixelFormat const& format     = PixelFormat (),
						bool               gen_mipmap = false);

	MinFilter getMinFilter () const noexcept { return m_eMin; }
	MagFilter getMagFilter () const noexcept { return m_eMag; }
	PixelFormat  format       () const noexcept { return m_gFormat; }
	uint         sampleId     () const noexcept { return m_uSampleId; }

	bool isLoaded () const noexcept
	{ return m_gStates.test (Texture::IsLoaded); }

	bool hasMipMaps () const noexcept
	{ return m_gStates.test (Texture::HasMipMaps); }

private:
	enum    Flag { IsLoaded = 1 << 0, HasMipMaps = 1 << 1 };
	typedef BitSet<Texture::Flag> Flags;

	PixelFormat m_gFormat;
	point2i     m_gSize;
	uint        m_uSampleId;
	MinFilter   m_eMin;
	MagFilter   m_eMag;
	Flags       m_gStates;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_TEXTURE_H_
