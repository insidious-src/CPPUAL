/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#include <cppual/fn_meta.h>
#include <cppual/ui/view.h>
#include <cppual/ui/manager.h>
#include <cppual/ui/vsurface.h>

#include <iostream>
//#include <exception>
//#include <algorithm>
#include <unordered_map>

// =========================================================

namespace std {

// =========================================================

using cppual::uptr;
using cppual::ui::event_queue;

// =========================================================

template <>
struct hash <event_queue::handle_type>
{
    constexpr size_t operator () (event_queue::handle_type const& val) const
    {
        return sizeof (uptr) == 8 ? (val.get<uptr> ()) >> 3 : (val.get<uptr> ()) >> 1;
    }
};

// =========================================================

} // namespace std

// =========================================================

namespace cppual::ui {

namespace { namespace internal {

typedef event_queue::handle_type                               handle_type         ;
typedef memory::allocator<std::pair<handle_type const, view*>> view_allocator      ;
typedef memory::allocator<proxy_renderable>                    renderable_allocator;
typedef dyn_array<std::pair<handle_type, view*>>               vec_type            ;

typedef std::unordered_map<handle_type,
                           view*,
                           std::hash<handle_type>,
                           std::equal_to<handle_type>,
                           view_allocator
                           >
                           map_type;

typedef consteval_bimap<std::pair<uptr, view*>, 1> bimap_type;

inline bimap_type& map ()
{
    static auto views_map = make_consteval_bimap<uptr, view*> (bimap_type::pair_type ());
    return views_map;
}

inline vec_type& vec ()
{
    static vec_type views_vec;

    if (views_vec.capacity () == 0) views_vec.reserve (5);
    return views_vec;
}

#ifdef DEBUG_MODE
inline void print_map_values (handle_type wnd)
{
    std::cout << "wnd value: " << wnd.get<uptr> () << std::endl;

    for (auto it = internal::map ().cbegin (); it != internal::map ().cend (); ++it)
    {
        std::cout << "view map key: " << it->first << " value: " << it->second << std::endl;
    }
}
#endif

inline view::window_type create_renderable (view* pParentObj, rect const& gRect, u32 nScreen)
{
    return pParentObj ? memory::allocate_shared<platform_wnd_interface, proxy_renderable>
                                                               (renderable_allocator (),
                                                                pParentObj->renderable (),
                                                                gRect) :
                        platform::factory::instance ()->createWindow (gRect, nScreen);
}

} } // anonymous namespace Internal

// =========================================================

view::view ()
: self_type (nullptr, rect (0, 0, default_width, default_height))
{ }

view::view (self_type* pParentObj, rect const& gRect, u32 nScreen, resource_type* rc)
: _M_gChildrenList (rc ? allocator_type(*rc) : allocator_type ()),
  _M_gMinSize { 0, 0 },
  _M_gMaxSize { 0, 0 },
  _M_pRenderable (internal::create_renderable (pParentObj, gRect, nScreen)),
  _M_pSurface (_M_pRenderable != nullptr && _M_pRenderable->valid () && !pParentObj ?
               gfx::draw_factory::instance ()->create_surface (_M_pRenderable->connection ()->native (),
                                                               _M_pRenderable->connection ()->legacy (),
                                                               gfx::pixel_format::default2d (),
                                                               _M_pRenderable->geometry ().size (),
                                                               _M_pRenderable->handle ()) :
               pParentObj->_M_pSurface),
  _M_pContext (_M_pSurface ?
               gfx::draw_factory::instance ()->create_context (_M_pRenderable->connection ()->native (),
                                                               _M_pRenderable->connection ()->legacy (),
                                                               gfx::pixel_format::default2d ()) :
               pParentObj->_M_pContext),
  _M_pParentObj  (),
  _M_gStateFlags ()
{
    if (_M_pRenderable == nullptr || !_M_pRenderable->valid ())
    {
        _M_pRenderable.reset ();
        throw std::logic_error ("FAILED to create renderable!");
    }
    else
    {
        static bool bRegEvents = false;

        if (!pParentObj)
        {
            using internal::vec_type;

            uptr const key = _M_pRenderable->handle ();

            if (internal::map ().get_index (key) != internal::bimap_type::npos)
            {
                internal::map ()[key] = this;

                for (auto& it : internal::vec ())
                {
                    if (it.first == key) { it.second = this; break; }
                }
            }
            else
            {
                if (!internal::map ().emplace (key, this).second)
                {
                    _M_pRenderable.reset ();
                    throw std::runtime_error ("FAILED to register view object!");

                    internal::vec ().emplace_back (key, this);

                    std::sort (internal::vec ().begin (), internal::vec ().end (),
                               [](vec_type::reference a, vec_type::reference b)
                    {
                        return a.first.get () < b.first.get ();
                    });
                }
            }

            display_queue_interface::primary ()->
                    set_window_events (*_M_pRenderable,
                                        event_type::key   |
                                        event_type::mouse |
                                        event_type::window);

            if (!bRegEvents)
            {
                connect (event_queue::events ().mouseMove,
                         [](event_queue::handle_type wnd, point2u pos)
                {
                    #ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
                    #endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->mouse_moved_event (pos);
                    else
                        internal::map ()[wnd]->mouse_moved_event (pos);
                });

                connect (event_queue::events ().mousePress,
                        [](event_queue::handle_type wnd, event_type::mbutton_data data)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->mouse_pressed_event (data);
                    else
                        internal::map ()[wnd]->mouse_pressed_event (data);
                });

                connect (event_queue::events ().mouseRelease,
                        [](event_queue::handle_type wnd, event_type::mbutton_data data)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->mouse_released_event (data);
                    else
                        internal::map ()[wnd]->mouse_released_event (data);
                });

                connect (event_queue::events ().keyPress,
                        [](event_queue::handle_type wnd, event_type::key_data data)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->key_pressed_event (data);
                    else
                        internal::map ()[wnd]->key_pressed_event (data);
                });

                connect (event_queue::events ().keyRelease,
                        [](event_queue::handle_type wnd, event_type::key_data data)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->key_released_event (data);
                    else
                        internal::map ()[wnd]->key_released_event (data);
                });

                connect (event_queue::events ().scroll,
                        [](event_queue::handle_type wnd, event_type::mwheel_data data)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->mouse_wheel_event (data);
                    else
                        internal::map ()[wnd]->mouse_wheel_event (data);
                });

                connect (event_queue::events ().winPaint,
                        [](event_queue::handle_type wnd, event_type::paint_data data)
                {
                    auto const window = internal::vec ().size () == 1 ?
                                        internal::vec ()[0].second :
                                        internal::map ()[wnd];

#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    window->_M_pSurface->paint_background (gfx::color (255, 255, 255));
                    window->paint (data.region);
                });

                connect (event_queue::events ().winFocus,
                        [](event_queue::handle_type wnd, bool state)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->focus_event (state);
                    else
                        internal::map ()[wnd]->focus_event (state);
                });

                connect (event_queue::events ().winSize,
                        [](event_queue::handle_type wnd, point2u size)
                {
                    auto const window = internal::vec ().size () == 1 ?
                                        internal::vec ()[0].second :
                                        internal::map ()[wnd];

#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    window->_M_pSurface->scale (size);
                    window->_M_pSurface->paint_background (gfx::color (255, 255, 255));
                    window->size (size);
                });

                connect (event_queue::events ().winVisible,
                        [](event_queue::handle_type wnd, bool state)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->focus_event (state);
                    else
                        internal::map ()[wnd]->focus_event (state);
                });

                connect(event_queue::events ().winStep,
                        [](event_queue::handle_type wnd, bool state)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->enter_leave_event (state);
                    else
                        internal::map ()[wnd]->enter_leave_event (state);
                });

                connect (event_queue::events ().winProperty,
                        [](event_queue::handle_type wnd, event_type::property_data data)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif


                    switch (data.prop)
                    {
                    case 0:
                        if (internal::vec ().size () == 1)
                            internal::vec ()[0].second->min_max_size_event (point2u ());
                        else
                            internal::map ()[wnd]->min_max_size_event (point2u ());
                        break;
                    default:
                        break;
                    }
                });

                connect (event_queue::events ().winHelp,
                        [](event_queue::handle_type wnd)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->help_event ();
                    else
                        internal::map ()[wnd]->help_event ();
                });

                connect (event_queue::events ().winMinimize,
                        [](event_queue::handle_type wnd, bool is_minimized)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->minimize_event (is_minimized);
                    else
                        internal::map ()[wnd]->minimize_event (is_minimized);
                });

                connect (event_queue::events ().winMaximize,
                        [](event_queue::handle_type wnd, bool is_maximized)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->maximize_event (is_maximized);
                    else
                        internal::map ()[wnd]->maximize_event (is_maximized);
                });

                connect (event_queue::events ().winFullscreen,
                        [](event_queue::handle_type wnd, bool is_fullscreen)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->fullscreen_event (is_fullscreen);
                    else
                        internal::map ()[wnd]->fullscreen_event (is_fullscreen);
                });

                connect (event_queue::events ().winClose,
                        [](event_queue::handle_type wnd)
                {
#                   ifdef DEBUG_MODE
                    internal::print_map_values (wnd);
#                   endif

                    if (internal::vec ().size () == 1)
                        internal::vec ()[0].second->close_event ();
                    else
                        internal::map ()[wnd]->close_event ();
                });

                bRegEvents = true;
            }
        }

    }

    if (pParentObj)
    {
        _M_pRenderable->set_owner (pParentObj->renderable ());
        _M_pParentObj = pParentObj;

        pParentObj->_M_gChildrenList.push_back (this);
        _M_gItFromParent = --pParentObj->_M_gChildrenList.end ();
    }

    _M_gStateFlags  = state_flag::is_valid;
    _M_gStateFlags += state_flag::enabled ;
}

view::view (view&& /*gObj*/)
{
}

view::view (view const& gObj)
: view (gObj._M_pParentObj,
        gObj._M_pRenderable->geometry (),
        gObj._M_pRenderable->screen   ())
{
    if (gObj.valid ())
    {
        if (!gObj.is_enabled ()) disable ();
        if (!gObj.is_hidden  ()) show    ();
    }
}

view& view::operator = (view&& /*gObj*/)
{
    return *this;
}

view& view::operator = (view const& gObj)
{
    if (this == &gObj) return *this;

    if (gObj.valid ())
    {
        if (internal::create_renderable (gObj._M_pParentObj,
                                         gObj._M_pRenderable->geometry (),
                                         gObj._M_pRenderable->screen   ()))
        {
            if (!gObj.is_enabled ()) disable ();
            if (!gObj.is_hidden  ()) show    ();
        }
    }

    return *this;
}

view::~view ()
{
    if (valid ()) destroy_resources ();
}

void view::destroy_children ()
{
    if (_M_gChildrenList.empty ()) return;

    for (auto pChild : _M_gChildrenList) if (pChild != nullptr) pChild->destroy ();
    _M_gChildrenList.clear ();
}

void view::destroy_resources ()
{
    //! destroy all child virtual surfaces
    destroy_children ();

    //! if the object is a child then remove it from
    //! the parent
    if (_M_pParentObj) *_M_gItFromParent = nullptr;

    //! if the surface is unique it gets deleted
    _M_pRenderable.reset ();

    if (!_M_pParentObj) internal::map ().erase (_M_pRenderable->handle ());
}

void view::invalidate () noexcept
{
    _M_pParentObj  = nullptr;
    _M_gMinSize    = _M_gMaxSize    = { 0, 0 };
    _M_gStateFlags = _M_gStateFlags = false;
}

void view::destroy ()
{
    if (!valid ()) return;
    destroy_resources ();
    invalidate ();
}

void view::show ()
{
    if (!valid ()) return;
    _M_pRenderable->map ();
}

void view::hide ()
{
    if (!valid ()) return;
    _M_pRenderable->unmap ();
}

void view::set_minimum_size (point2u gSize)
{
    if (_M_gStateFlags.test (state_flag::is_valid))
    {
        //m_pRenderable->setMimimumSize (gSize);
        _M_gMinSize = gSize;
    }
}

void view::set_maximum_size (point2u gSize)
{
    if (_M_gStateFlags.test (state_flag::is_valid))
    {
        //m_pRenderable->setMaximumSize (gSize);
        _M_gMinSize = gSize;
    }
}

bool view::set_parent (view* pParentObj, point2i /*gPos*/)
{
    if (!_M_gStateFlags.test (state_flag::is_valid) or _M_pParentObj == pParentObj)
        return false;

    if (pParentObj)
    {
        if (_M_pParentObj != nullptr) *_M_gItFromParent = nullptr;
        _M_pRenderable->set_owner (pParentObj->_M_pRenderable);

        pParentObj->_M_gChildrenList.push_back (this);
        //pParentObj->paint ();

        _M_gItFromParent = std::move (--pParentObj->_M_gChildrenList.end ());
    }
    //! pParentObj is invalid and _M_pParentObj is NOT nullptr
    else
    {
        //! recreate using physical surface
        _M_pRenderable = platform::factory::instance ()->
                        createWindow (_M_pRenderable->geometry (),
                                      _M_pRenderable->screen (),
                                      _M_pRenderable->connection ());
        if (_M_pRenderable == nullptr) return false;

        *_M_gItFromParent = nullptr;
    }

    _M_pParentObj = pParentObj;
    return true;
}

void view::paint (rect const& gRect)
{
    if (!_M_pContext->active ())
    {
        _M_pContext->use (_M_pSurface, _M_pSurface);
        gfx::context_interface::acquire (_M_pContext);
    }

    paint_event (gRect);

    if (_M_gChildrenList.size ())
        for (auto pChild : _M_gChildrenList) pChild->paint (gRect);

    if (_M_pSurface && !_M_pParentObj) _M_pSurface->flush ();
}

void view::size (point2u gSize)
{
    size_event (gSize);

    if (_M_gChildrenList.size ())
        for (auto pChild : _M_gChildrenList) pChild->on_parent_size (gSize);

    refresh ();
}

void view::set_geometry (rect const& gRect)
{
    if (_M_gStateFlags.test (state_flag::is_valid))
    {
        rect gNewRect (gRect);

        //! width
        if (gNewRect.width () < _M_gMinSize.x)
        {
            gNewRect.right = static_cast<rect::value_type> (gNewRect.left + _M_gMinSize.x);
        }
        else if (_M_gMaxSize != point2u () and gNewRect.width () < _M_gMaxSize.x)
        {
            gNewRect.right = static_cast<rect::value_type> (gNewRect.left + _M_gMinSize.x);
        }

        //! height
        if (gNewRect.height () > _M_gMinSize.y)
        {
            gNewRect.bottom = static_cast<rect::value_type> (gNewRect.top + _M_gMinSize.y);
        }
        else if (_M_gMaxSize != point2u () and gNewRect.height () > _M_gMaxSize.y)
        {
            gNewRect.bottom = static_cast<rect::value_type> (gNewRect.top + _M_gMinSize.y);
        }

        _M_pRenderable->set_geometry (gNewRect);
    }
}

void view::move (point2i gPoint)
{
    if (_M_gStateFlags.test (state_flag::is_valid))
    {
        _M_pRenderable->move (gPoint);
        move_event (gPoint);
    }
}

void view::get_focus ()
{
    if (_M_gStateFlags.test (state_flag::is_valid))
    {
        _M_pRenderable->raise ();
        focus_event (true);
    }
}

void view::kill_focus ()
{
    if (_M_gStateFlags.test (state_flag::is_valid))
    {
        for (view* pChild : _M_gChildrenList) pChild->kill_focus ();
        focus_event (false);
    }
}

void view::enable ()
{
    if (_M_gStateFlags.test (state_flag::is_valid))
        for (view* pChild : _M_gChildrenList) pChild->enable ();
}

void view::disable ()
{
    if (_M_gStateFlags.test (state_flag::is_valid))
        for (view* pChild : _M_gChildrenList) pChild->disable ();
}

void view::refresh ()
{
    update (geometry ());
}

void view::update (rect const& region)
{
    if (_M_gStateFlags.test (state_flag::is_valid))
    {
        paint (region);
    }
}

void view::mouse_moved_event (point2u)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

void view::mouse_wheel_event (event_type::mwheel_data const&)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

void view::mouse_pressed_event (event_type::mbutton_data const&)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

void view::mouse_released_event (event_type::mbutton_data const&)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

void view::key_pressed_event (event_type::key_data const&)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

void view::key_released_event (event_type::key_data const&)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

void view::help_event()
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

#ifdef DEBUG_MODE
void view::minimize_event(bool is_minimized)
#else
void view::minimize_event(bool /*is_minimized*/)
#endif
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << ' ' << is_minimized << std::endl;
#   endif
}

#ifdef DEBUG_MODE
void view::maximize_event(bool is_maximized)
#else
void view::maximize_event(bool /*is_maximized*/)
#endif
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__<< ' ' << is_maximized << std::endl;
#   endif
}

#ifdef DEBUG_MODE
void view::fullscreen_event(bool is_fullscreen)
#else
void view::fullscreen_event(bool /*is_fullscreen*/)
#endif
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__<< ' ' << is_fullscreen << std::endl;
#   endif
}

void view::close_event()
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

#ifdef DEBUG_MODE
void view::show_event(bool is_shown)
#else
void view::show_event(bool /*is_shown*/)
#endif
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__<< ' ' << is_shown << std::endl;
#   endif
}

void view::paint_event(rect const&)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

#ifdef DEBUG_MODE
void view::on_enable(bool is_enabled)
#else
void view::on_enable(bool /*is_enabled*/)
#endif
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__<< ' ' << is_enabled << std::endl;
#   endif
}

void view::size_event(point2u)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

void view::move_event(point2i)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

#ifdef DEBUG_MODE
void view::enter_leave_event(bool is_entered)
#else
void view::enter_leave_event(bool /*is_entered*/)
#endif
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__<< ' ' << is_entered << std::endl;
#   endif
}

void view::begin_size_move_event(rect const&)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

void view::end_size_move_event(rect const&)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

void view::min_max_size_event(point2u)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

#ifdef DEBUG_MODE
void view::focus_event(bool is_focused)
#else
void view::focus_event(bool /*is_focused*/)
#endif
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__<< ' ' << is_focused << std::endl;
#   endif
}

void view::on_parent_size(point2u)
{
#   ifdef DEBUG_MODE
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#   endif
}

} // namespace Ui