/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_GFX_VIRTUAL_SURFACE_H_
#define CPPUAL_GFX_VIRTUAL_SURFACE_H_
#ifdef __cplusplus

#include <cppual/ui/wm.h>
#include <cppual/gfx/gl/texture.h>

namespace cppual { namespace Ui {

class ProxyRenderable final : public IWindow
{
public:
    ProxyRenderable () = delete;
    ProxyRenderable (IWindow*) noexcept;
    ProxyRenderable (ProxyRenderable const&) noexcept;
    ProxyRenderable (shared_window const& owner, Rect const& rect) noexcept;
    ProxyRenderable& operator = (IWindow*) noexcept;
    ProxyRenderable& operator = (ProxyRenderable const&) noexcept;

    string title () const noexcept { return string (); }
    void   setTitle (string const&) noexcept { }
    void   setShaded (bool) noexcept { }
    bool   isShaded () noexcept { return false; }
    void   setModal (bool) noexcept { }
    bool   isModal () noexcept { return false; }
    void   setFullscreen (bool) noexcept { }
    bool   isFullscreen () noexcept { return false; }
    void   setMaximized (bool) noexcept { }
    bool   isMaximized () noexcept { return false; }
    void   setMinimized (bool) noexcept { }
    bool   isMinimized () noexcept { return false; }
    void   setVisibleInTaskbar (bool) noexcept { }
    bool   isVisibleInTaskbar () noexcept { return false; }
    void   setVisibleInPager (bool) noexcept { }
    bool   isVisibleInPager () noexcept { return false; }
    void   setMimimumSize (point2u) noexcept { }
    void   setMaximumSize (point2u) noexcept { }
    void   flash (uint) noexcept { }
    void   setGeometry (Rect const&) noexcept;
    void   setOwner (const_pointer) noexcept;
    void   move (point2i) noexcept;
    void   map () noexcept;
    void   unmap () noexcept;

    WindowFlags flags () const { return WindowFlags (); }
    void        setFlags (WindowFlags) { }

    inline ~ProxyRenderable     ()       noexcept { unmap ();            }
    inline weak_window owner    () const noexcept { return m_pParent;    }
    inline Rect        geometry () const noexcept { return m_gRect;      }
    inline bool        isMapped () const noexcept { return m_bIsVisible; }

    inline u32 screen () const noexcept
    { return m_pParent != nullptr ? m_pParent->screen () : 0; }

    inline void raise () noexcept
    { if (m_pParent != nullptr) m_pParent->raise (); }

    inline void lower () noexcept
    { if (m_pParent != nullptr) m_pParent->lower (); }

private:
    shared_window m_pParent;
    Rect          m_gRect;
    bool          m_bIsVisible;
};

// =========================================================

class VirtualBuffer final
{
public:
    VirtualBuffer ();
    VirtualBuffer (VirtualBuffer&&) noexcept;
    VirtualBuffer (VirtualBuffer const&) noexcept;
    VirtualBuffer (VirtualBuffer& parent, Rect const& rect) noexcept;
    VirtualBuffer& operator = (VirtualBuffer&&) noexcept;
    VirtualBuffer& operator = (VirtualBuffer const&) noexcept;

    void setGeometry (Rect const&) noexcept;
    void setParent (VirtualBuffer& parent, point2i) noexcept;
    void move (point2u) noexcept;
    void map () noexcept;
    void unmap () noexcept;

    ~VirtualBuffer () noexcept { unmap (); }
    VirtualBuffer& parent   () const noexcept { return *m_pParent;    }
    Rect           geometry () const noexcept { return  m_gRect;      }
    bool           isMapped () const noexcept { return  m_bIsVisible; }

private:
    VirtualBuffer* m_pParent;
    Rect           m_gRect;
    bool           m_bIsVisible;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_GFX_VIRTUAL_SURFACE_H_
