/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#include <string>
#include <vector>
#include <memory>
#include <cppual/flags.h>
#include <cppual/gfx/color.h>
#include <cppual/gfx/coord.h>
#include <cppual/gfx/dsp_details.h>

using std::string;

namespace cppual { namespace Graphics {

struct PixelFlag final
{
    enum Type
    {
        Drawable = 1 << 0, // support surface buffer
        Bitmap   = 1 << 1, // support pixmap buffer
        Palette  = 1 << 2  // support off-screen buffer
    };
};

enum class DeviceType : unsigned char
{
    Custom,
    Native,
    GL,
    Vulkan,
    Direct3D,
    Direct3D12,
    Direct2D,
    Metal
};

enum class PolygonFace : unsigned char
{
    Front,
    Back,
    BothSides
};

struct  GFXVersion;
struct  IDeviceContext;
struct  IPixelSurface;
typedef BitSet<PixelFlag::Type>         PixelFlags;
typedef std::shared_ptr<IDeviceContext> shared_context;
typedef std::shared_ptr<IPixelSurface>  shared_buffer;
typedef std::weak_ptr  <IDeviceContext> weak_context;
typedef std::weak_ptr  <IPixelSurface>  weak_buffer;

// =========================================================

struct GFXVersion
{
    int major, minor;
};

// =========================================================

struct PixelFormat final
{
    u8         red, green, blue, alpha;
    u8         depth, stencil;
    PixelFlags flags;
    ColorType  colorType;

    constexpr u8 bits () const noexcept
    { return u8 (red + green + blue + alpha); }

    constexpr static PixelFormat default2D () noexcept
    {
        return
        {
            8,
            8,
            8,
            0,
            0,
            0,
            PixelFlag::Drawable | PixelFlag::Bitmap,
            ColorType::TrueType
        };
    }

    constexpr static PixelFormat default3D () noexcept
    {
        return
        {
            8,
            8,
            8,
            8,
            24,
            0,
            PixelFlag::Drawable | PixelFlag::Bitmap,
            ColorType::Direct
        };
    }
};

// =========================================================

// memory buffer
class VirtualBuffer
{
public:
    typedef std::vector<Color> vector_type;
    typedef PixelFormat        format_type;
    typedef std::size_t        size_type;
    typedef Color              value_type;

    inline format_type  format () const noexcept { return m_gFormat; }
    inline vector_type& data   ()       noexcept { return m_gPixels; }

    inline VirtualBuffer () noexcept
    : m_gFormat (PixelFormat::default2D ()),
      m_gPixels ()
    { }

    VirtualBuffer (point2i gSize, PixelFormat const& gFormat = PixelFormat::default2D ())
    : m_gFormat   (gFormat),
      m_gPixels   (size_type (gSize.x * gSize.y))
    { }

private:
    PixelFormat m_gFormat;
    vector_type m_gPixels;
};

// =========================================================

class Transform2D
{
public:
    Transform2D () noexcept = default;
    constexpr Rect geometry () const noexcept { return m_gRect; }

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
    virtual format_type format     () const = 0;
    virtual value_type  handle     () const = 0;
    virtual DeviceType  device     () const = 0;
    virtual void        flush      ()       = 0;
};

// =========================================================

// Surface
struct IPixelSurface : public IResource
{
    enum class Type : unsigned char
    {
        Drawable,
        DoubleBuffer,
        BackBuffer,
        Pixmap
    };

    virtual point2u size  () const = 0;
    virtual Type    type  () const = 0;
    virtual void    scale (point2u size) = 0;
};

// =========================================================

// Device
struct IDeviceContext : public IResource
{
    typedef IPixelSurface*       pointer;
    typedef IPixelSurface const* const_pointer;

    virtual pointer       drawable () const = 0;
    virtual const_pointer readable () const = 0;
    virtual GFXVersion    version  () const = 0;
    virtual bool          assign   () = 0;
    virtual bool          use      (pointer, const_pointer) = 0;
    virtual void          finish   () = 0;
    virtual void          release  () = 0;

    static IDeviceContext* current () noexcept;
    static void            acquire (IDeviceContext*) noexcept;

    bool active () const noexcept
    { return this == current (); }
};

// =========================================================

struct IDrawable2D
{
    virtual DeviceType type () const noexcept    = 0;
    virtual void       draw (Transform2D const&) = 0;

    virtual ~IDrawable2D () { }
};

// =========================================================

struct IDrawable3D
{
    virtual DeviceType type () const noexcept    = 0;
    virtual void       draw (Transform3D const&) = 0;

    virtual ~IDrawable3D () { }
};

// =========================================================

struct ITransformable2D
{
    virtual ~ITransformable2D () { }
};

// =========================================================

struct ITransformable3D
{
    virtual ~ITransformable3D () { }
};

// ====================================================

struct DrawableFactory
{
    static IDrawable2D* create2D (string const& name);
    static IDrawable3D* create3D (string const& name);
    static bool         load     (string const& module_path);
};

// ====================================================

inline bool operator  ! (GFXVersion const& gObj) noexcept
{ return !gObj.major and !gObj.minor; }

inline bool operator  < (GFXVersion const& gObj1, GFXVersion const& gObj2) noexcept
{ return (gObj1.major < gObj2.major or gObj1.minor < gObj2.minor); }

inline bool operator  > (GFXVersion const& gObj1, GFXVersion const& gObj2) noexcept
{ return (gObj1.major > gObj2.major or gObj1.minor > gObj2.minor); }

inline bool operator  < (GFXVersion const& gObj1, int uMajor) noexcept
{ return gObj1.major  < uMajor; }

inline bool operator  > (GFXVersion const& gObj1, int uMajor) noexcept
{ return gObj1.major  > uMajor; }

inline bool operator  == (GFXVersion const& gObj1, GFXVersion const& gObj2) noexcept
{ return (gObj1.major == gObj2.major and gObj1.minor == gObj2.minor); }

inline bool operator  != (GFXVersion const& gObj1, GFXVersion const& gObj2) noexcept
{ return (gObj1.major != gObj2.major or gObj1.minor != gObj2.minor); }

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>&
operator << (std::basic_ostream<CharT, Traits>& os, GFXVersion const& u)
{ return os << u.major << "." << u.minor; }

} } // namespace Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAW_H_
