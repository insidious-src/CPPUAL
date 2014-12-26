/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 fymfifa
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

#ifndef CPPUAL_PLATFORM_X_INPUT_H_
#define CPPUAL_PLATFORM_X_INPUT_H_
#ifdef __cplusplus

#include <cppual/ui/manager.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <xcb/xinput.h>

namespace cppual { namespace Input {

class DECL_EXPORT XQueue final : public IDisplayQueue
{
public:
	XQueue () noexcept;
	bool setRenderableEvents (Ui::IRenderable&, mask_type) noexcept;
	bool pop_front (event_type&, bool wait) noexcept;

	inline bool isValid () const noexcept
	{ return m_pXCBHandle != nullptr; }

private:
	xcb_connection_t* const m_pXCBHandle;
};

} } // namespace Input

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_X_INPUT_H_
