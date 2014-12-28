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

#ifndef CPPUAL_GFX_DRAW_H_
#define CPPUAL_GFX_DRAW_H_
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/gfx/color.h>
#include <cppual/gfx/coord.h>
#include <cppual/circular_queue.h>
#include <cppual/gfx/dsp_details.h>

namespace cppual { namespace Graphics {

struct PixelFlag final
{
	enum Type
	{
		Drawable	 = 1 << 0, // applied in surface buffer
		Bitmap		 = 1 << 1, // applied in pixelmap buffer
		Accelerated  = 1 << 2, // is stored on the GPU
		Palette		 = 1 << 3,
		DoubleBuffer = 1 << 4, // front and back buffers
		VBlank       = 1 << 5, // make double-buffer swappable or a single one flippable
		Stereo		 = 1 << 6  // applied in quadriple stereo buffer
	};
};

enum class DeviceType : unsigned char
{
	Custom,
	Native,
	EGL,
	Direct3D,
	Direct2D,
	Metal,
	Software
};

enum class PolygonFace : unsigned char
{
	Front,
	Back,
	BothSides
};

struct  GFXVersion;
struct  IDeviceContext;
struct  IPixelBuffer;
typedef BitSet<PixelFlag::Type>         PixelFlags;
typedef std::shared_ptr<IDeviceContext> shared_context;
typedef std::shared_ptr<IPixelBuffer>   shared_buffer;
typedef std::weak_ptr  <IDeviceContext> weak_context;
typedef std::weak_ptr  <IPixelBuffer>   weak_buffer;

/****************************** Common Classes ******************************/

struct PixelFormat final
{
	u8		   red, green, blue, alpha;
	u8		   depth, stencil;
	PixelFlags flags;
	ColorType  colorType;

	constexpr u8 bits () const noexcept
	{ return u8 (red + green + blue + alpha); }

	constexpr static const PixelFormat default2D () noexcept
	{
		return
		{
			8,
			8,
			8,
			0,
			0,
			0,
			PixelFlag::Drawable | PixelFlag::Accelerated,
			ColorType::TrueType
		};
	}

	constexpr static const PixelFormat default3D () noexcept
	{
		return
		{
			8,
			8,
			8,
			8,
			24,
			0,
			PixelFlag::Drawable | PixelFlag::DoubleBuffer |
			PixelFlag::VBlank   | PixelFlag::Accelerated,
			ColorType::Direct
		};
	}
};

// =========================================================

// memory buffer
class PixelMap
{
public:
	typedef CircularQueue<RGBColor> vector_type;
	typedef std::size_t		        size_type;
	typedef RGBColor                value_type;

	inline PixelFormat	format () const noexcept { return m_gFormat; }
	inline vector_type& data () noexcept { return m_gPixels; }

	inline PixelMap () noexcept
	: m_gFormat (PixelFormat::default2D ()),
	  m_gPixels ()
	{ }

	PixelMap (point2i     gSize,
			  PixelFormat const& gFormat = PixelFormat::default2D ())
	: m_gFormat (gFormat),
	  m_gPixels (size_type (gSize.x * gSize.y))
	{ }

private:
	PixelFormat m_gFormat;
	vector_type	m_gPixels;
};

// =========================================================

class Transform2D
{
public:
	inline    Transform2D () noexcept = default;
	constexpr int16 x () const noexcept { return m_gRect.left; }
	constexpr int16 y () const noexcept { return m_gRect.top; }
	constexpr u16   width () const noexcept { return m_gRect.width (); }
	constexpr u16   height () const noexcept { return m_gRect.height (); }

private:
	Rect m_gRect;
};

// =========================================================

class Transform3D
{ };

// =========================================================

struct IResource
{
	typedef Connection  controller;
	typedef Element     value_type;
	typedef PixelFormat format_type;

	virtual ~IResource () { }

	virtual controller  connection () const = 0;
	virtual format_type format () const = 0;
	virtual value_type  handle () const = 0;
	virtual DeviceType  type () const = 0;
	virtual void        flush () = 0;
};

// =========================================================

// Surface
struct IPixelBuffer : public IResource
{
	virtual point2i size () const = 0;
	virtual int     colormap () const = 0;
};

// =========================================================

// Device
struct IDeviceContext : public IResource
{
	typedef IPixelBuffer*       pointer;
	typedef IPixelBuffer const* const_pointer;
	typedef IPixelBuffer&       reference;
	typedef IPixelBuffer const& const_reference;

	virtual pointer       writable () const = 0;
	virtual const_pointer readable () const = 0;
	virtual GFXVersion    version () const = 0;
	virtual bool          use (reference, const_reference) = 0;
	virtual bool          use () = 0;
	virtual void          finish () = 0;
	virtual void          release () = 0;

	static IDeviceContext* current () noexcept;
	static void            acquire (IDeviceContext*) noexcept;

	inline bool active () const noexcept
	{ return this == current (); }
};

// =========================================================

struct IDrawable2D
{
	virtual DeviceType type () const noexcept = 0;
	virtual void       draw (Transform2D const&) = 0;

	inline virtual ~IDrawable2D () { }
};

// =========================================================

struct IDrawable3D
{
	virtual DeviceType type () const noexcept = 0;
	virtual void       draw (Transform3D const&) = 0;

	inline virtual ~IDrawable3D () { }
};

// =========================================================

struct ITransformable2D
{
	inline virtual ~ITransformable2D () { }
};

// =========================================================

struct ITransformable3D
{
	inline virtual ~ITransformable3D () { }
};

// ====================================================

struct GFXVersion
{
	int major, minor;

	constexpr int egl () const noexcept
	{ return major; }
};

inline bool operator ! (GFXVersion const& gObj) noexcept
{ return !gObj.major and !gObj.minor; }

inline bool operator < (GFXVersion const& gObj1, GFXVersion const& gObj2) noexcept
{ return (gObj1.major < gObj2.major or gObj1.minor < gObj2.minor); }

inline bool operator > (GFXVersion const& gObj1, GFXVersion const& gObj2) noexcept
{ return (gObj1.major > gObj2.major or gObj1.minor > gObj2.minor); }

inline bool operator < (GFXVersion const& gObj1, int uMajor) noexcept
{ return gObj1.major < uMajor; }

inline bool operator > (GFXVersion const& gObj1, int uMajor) noexcept
{ return gObj1.major > uMajor; }

inline bool operator == (GFXVersion const& gObj1, GFXVersion const& gObj2) noexcept
{ return (gObj1.major == gObj2.major and gObj1.minor == gObj2.minor); }

inline bool operator != (GFXVersion const& gObj1, GFXVersion const& gObj2) noexcept
{ return (gObj1.major != gObj2.major or gObj1.minor != gObj2.minor); }

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>&
operator << (std::basic_ostream<CharT, Traits>& os, GFXVersion const& u)
{ return os << u.major << "." << u.minor; }

// =========================================================

static_assert (std::is_pod<PixelFormat>::value, "PixelFormat must be a POD");
static_assert (std::is_pod<GFXVersion>::value, "GFXVersion must be a POD");

} } // namespace Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAW_H_
