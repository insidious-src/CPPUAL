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

#include <cppual/gfx/display.h>
#include <Windows.h>

namespace cppual { namespace Graphics { namespace GL {

class CWinNTServer final : public IBackend
{
public:
	CWinNTServer () noexcept;
	CDisplay* AquireDisplay (ctchar*);
	bool ReleaseDisplay (CDisplay*);
	bool isValid () const;
	~CWinNTServer ();

	DisplayServerType getType () const noexcept { return sm_eType; }
	DisplayServerPolicy GetPolicy () const noexcept { return m_ePolicy; }

private:
	static const DisplayServerType sm_eType = DisplayServerType::WinNT;
	DisplayServerPolicy m_ePolicy;
};

} } } // GL

#endif // OS_WINDOWS