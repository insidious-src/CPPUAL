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

#include <cppual/gfx/draw.h>
#include <cppual/process/plugin.h>

namespace cppual { namespace Graphics {

namespace { namespace Internal {

typedef Process::PluginManager<IDrawable2D>    manager2d_type;
typedef Process::PluginManager<IDrawable3D>    manager3d_type;
typedef Process::PluginManager<IDeviceContext> manager_context_type;
typedef IDeviceContext                         context_type;
typedef IDeviceContext*                        context_pointer;

inline context_pointer& current () noexcept
{
    static thread_local context_pointer current_dc;
    return current_dc;
}

inline manager2d_type& manager2D () noexcept
{
    static manager2d_type drawable_mgr;
    return drawable_mgr;
}

inline manager3d_type& manager3D () noexcept
{
    static manager3d_type drawable_mgr;
    return drawable_mgr;
}

inline manager_context_type& managerContext() noexcept
{
    static manager_context_type context_mgr;
    return context_mgr;
}

} } // anonymous namespace Internal

// ====================================================

IDeviceContext* IDeviceContext::current () noexcept
{
    return Internal::current ();
}

void IDeviceContext::acquire (IDeviceContext* pContext) noexcept
{
    if (Internal::current () != pContext)
    {
        if (Internal::current () and
                pContext         and
                Internal::current ()->device  () != pContext->device ())
                Internal::current ()->release ();

        Internal::current () = pContext;
    }
}

// ====================================================

shared_drawable2d DrawableFactory::create2D (string const& gName)
{
    return Internal::manager2D ().construct (gName);
}

shared_drawable3d DrawableFactory::create3D (string const& gName)
{
    return Internal::manager3D ().construct (gName);
}

shared_context ContextFactory::create (string const& gName)
{
    return Internal::managerContext().construct (gName);
}

//bool ContextFactory::load(string const& module_path)
//{
//    return Internal::managerContext().load_plugin(module_path);
//}

// ====================================================

cppual::Graphics::IResource::~IResource()
{

}

// ====================================================

cppual::Graphics::IDrawable2D::~IDrawable2D()
{

}

// ====================================================

cppual::Graphics::IDrawable3D::~IDrawable3D()
{

}

// ====================================================

cppual::Graphics::ITransformable2D::~ITransformable2D()
{

}

// ====================================================

cppual::Graphics::ITransformable3D::~ITransformable3D()
{

}

} } // namespace Graphics
