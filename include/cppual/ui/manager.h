/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_MANAGER_H_
#define CPPUAL_PLATFORM_MANAGER_H_
#ifdef __cplusplus

#include <cppual/process/module.h>
#include <cppual/ui/events.h>
#include <cppual/gfx/draw.h>
#include <cppual/ui/display.h>
#include <cppual/ui/wm.h>

using std::string;
using cppual::Process::Module;
using cppual::Input::shared_queue;
using cppual::Ui::shared_display;
using cppual::Ui::weak_display;
using cppual::Ui::shared_renderable;
using cppual::Graphics::shared_buffer;
using cppual::Graphics::shared_context;
using cppual::Ui::IDisplay;
using cppual::Ui::IRenderable;
using cppual::Graphics::GFXVersion;
using cppual::Graphics::PixelFormat;
using cppual::Graphics::DeviceType;

namespace cppual { namespace Platform {

struct  IService;
typedef shared_ptr<IService> shared_manager;

struct IService : public NonCopyableVirtual
{
	typedef shared_renderable const& shared_reference;

	virtual shared_renderable createRenderable (Rect const& rect,
												u32 screen = 0,
												IDisplay* display = IDisplay::instance ()) = 0;

	virtual shared_display connectDisplay (cchar* name = nullptr) = 0;
	virtual shared_queue   createQueueObject () = 0;

	static IService* instance ();
	static Module&   module ();
	static bool      hasValidInstance () noexcept;
};

} } // namespace Platform

#endif // __cplusplus
#endif // CPPUAL_PLATFORM_MANAGER_H_
