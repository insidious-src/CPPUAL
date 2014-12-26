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

#ifdef OS_WINDOWS

#include <cppual/gfx/gl/glbase.h>
#include <Windows.h>
#include <GL/gl.h>

namespace cppual { namespace Graphics { namespace GL {

class CWGLSurface final : public IGLSurface
{
public:
	explicit CWGLSurface (CDisplay*, const IPixelSurface*, const CRect&, const CPixelFormat&);
	CWGLSurface (IPixelSurface*);
	~CWGLSurface ();

	bool isDirect () const;
	void setRect (const CRect&);
	void swapBuffers ();
	void clear (const CRGBColor&);

	CWGLSurface& operator = (const CWGLSurface&);
	CWGLSurface& operator = (CWGLSurface&&);
	CWGLSurface& operator = (IPixelSurface*);

	CWGLSurface () = delete;

	SurfaceType getType () const noexcept { return sm_eType; }
	CPixelFormat GetPixelFormat () const noexcept { return m_gFormat; }
	int getHeight () const noexcept { return (m_gRect.bottom - m_gRect.top); }
	int getWidth () const noexcept { return (m_gRect.right - m_gRect.left); }
	CRect getRect () const noexcept { return m_gRect; }
	CDisplay* GetDisplay () const noexcept { return m_pDisplay; }

private:
	static const SurfaceType sm_eType = SurfaceType::GL;
	CPixelFormat			  m_gFormat;
	CRect					  m_gRect;
	CDisplay*				  m_pDisplay;
	Drawable				  m_uDrawableId;
};

CWGLSurface::CWGLSurface (CDisplay* pDisplay,
						  const IPixelSurface*,
						  const CRect& gRect,
						  const CGLVersion gVersion,
						  const CPixelFormat& gPixFormat)
: m_gVersion (gVersion),
  m_gRect (gRect),
  m_pGLInstance (),
  m_pDisplay (pDisplay),
  m_gFormat (gPixFormat)
{
	if (!m_pGLInstance = wglCreateContext (nullptr)) return;
	wglMakeCurrent (nullptr, m_pGLInstance);

	m_bIsValid = true;
}

CWGLSurface::CWGLSurface (const CWGLSurface&)
{
}

CWGLSurface::CWGLSurface (IPixelSurface*)
{
}

CWGLSurface::CWGLSurface (CWGLSurface&&)
{
}

CWGLSurface::~CWGLSurface ()
{
	if (!m_bIsValid) return;

	wglMakeCurrent (nullptr, nullptr);
	wglDestroyContext (m_pGLInstance);
}

void CWGLSurface::setRect (const CRect&)
{
}

void CWGLSurface::swapBuffers ()
{
	SwapBuffers (IBackend::getInstance ()->getX11Display (), 0);
}

void CWGLSurface::clear (const CRGBColor&)
{
}

CWGLSurface& CWGLSurface::operator = (const CWGLSurface&)
{
	return *this;
}

CWGLSurface& CWGLSurface::operator = (CWGLSurface&&)
{
	return *this;
}

CWGLSurface& CWGLSurface::operator = (IPixelSurface*)
{
	return *this;
}

} } } // GL

#endif // OS_WINDOWS
