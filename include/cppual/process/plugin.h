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

#ifndef CPPUAL_PROCESS_PLUGIN_H_
#define CPPUAL_PROCESS_PLUGIN_H_
#ifdef __cplusplus


#include <cppual/abi>
#include <cppual/decl>
#include <cppual/types>
#include <cppual/string>
#include <cppual/concepts>
#include <cppual/resource>
#include <cppual/containers>
#include <cppual/noncopyable>
#include <cppual/memory/allocator.h>
#include <cppual/unbound_interface.h>

#include <memory>
#include <cstring>
#include <version>
#include <functional>
#include <string_view>

namespace cppual { namespace process {

// =========================================================

class SHARED_API dyn_loader : public non_copyable
{
public:
    typedef call_ret_t (DLCALL* function_type)();

    typedef dyn_loader       self_type  ;
    typedef string           string_type;
    typedef std::string_view string_view;
    typedef resource_handle  handle_type;

    enum class resolve_policy : u8
    {
        //! use as data file or load as a static library
        statically,
        //! resolve everything on load
        immediate,
        //! don't resolve any object or function references
        lazy
    };

    dyn_loader () = default;
    dyn_loader (self_type&&) noexcept;
    self_type& operator = (self_type&&) noexcept;
    bool attach () noexcept;
    void detach () noexcept;

    dyn_loader (string_type    path,
                bool           attach = true,
                resolve_policy policy = resolve_policy::lazy) noexcept;

    static string_type extension () noexcept;

    constexpr string_view    path        () const noexcept { return _M_gLibPath; }
    constexpr void_ptr       handle      () const noexcept { return _M_pHandle ; }
    constexpr resolve_policy policy      () const noexcept { return _M_eResolve; }
    constexpr bool           is_attached () const noexcept { return _M_pHandle ; }

    ~dyn_loader ()
    { if (_M_eResolve != resolve_policy::statically) detach (); }

    inline bool contains (string_view const& gName) const noexcept
    { return get_address (gName.data ()) != nullptr; }

    constexpr function_proxy<self_type> operator [] (string_view name) const noexcept
    {  return function_proxy<self_type> (*this, name); }

    template <typename T = void>
    constexpr T* import (string_view const& pName) const noexcept
    { return static_cast<T*> (get_address (pName.data ())); }

    template <typename R = void, typename... Args>
    constexpr auto import (string_view pName) const noexcept
    {
        return direct_cast<R(*)(Args...)> (get_function (pName.data ()));
    }

    template <typename R = void, typename... Args>
    constexpr R invoke (string_view pName, Args... args) const
    {
        auto fn = direct_cast<R(*)(Args...)> (get_function (pName.data ()));

        if (!fn) throw std::bad_function_call ();
        return (*fn)(std::forward<Args> (args)...);
    }

private:
    void_ptr      get_address  (string_view name) const noexcept;
    function_type get_function (string_view name) const noexcept;

private:
    handle_type    _M_pHandle ;
    string_type    _M_gLibPath;
    resolve_policy _M_eResolve { resolve_policy::lazy };
};

// =========================================================

struct SHARED_API plugin_vars
{
    cchar*                name     { };
    cchar*                provides { };
    cchar*                desc     { };
    uint                  verMajor { };
    uint                  verMinor { };
    u64                   priority { };
    std::shared_ptr<void> iface    { };
    dyn_array<cchar*>     required { };

    inline plugin_vars (plugin_vars const&) = default;
    inline plugin_vars& operator = (plugin_vars const&) = default;

    constexpr plugin_vars (memory::memory_resource* res = nullptr)
    : name     (),
      provides (),
      desc     (),
      verMajor (),
      verMinor (),
      iface    (),
      required (res == nullptr ? dyn_array<cchar*>::allocator_type () :
                                 dyn_array<cchar*>::allocator_type (*res))
    { }

    inline plugin_vars (plugin_vars&& obj)
    : name     (obj.name),
      provides (obj.provides),
      desc     (obj.desc),
      verMajor (obj.verMajor),
      verMinor (obj.verMinor),
      iface    (std::move (obj.iface   )),
      required (std::move (obj.required))
    { }

    ~plugin_vars () { }

    constexpr plugin_vars& operator = (plugin_vars&& obj)
    {
        if (this == &obj) return *this;

        name     = obj.name;
        provides = obj.provides;
        desc     = obj.desc;
        verMajor = obj.verMajor;
        verMinor = obj.verMinor;
        iface    = std::move (obj.iface   );
        required = std::move (obj.required);

        return *this;
    }
};

typedef is_movable_t<dyn_loader>                  loader_type;
typedef std::pair<loader_type const, plugin_vars> plugin_pair;

// =========================================================

template <typename Interface,
          typename A = memory::allocator<std::pair<string const, plugin_pair>>,
          typename   = std::enable_if_t<
              std::is_same_v<typename std::allocator_traits<A>::value_type,
                             std::pair<string const, plugin_pair>
                             >>
          >
class SHARED_API plugin_manager : public non_copyable
{
public:
    static_assert (memory::is_allocator_v<A>, "invalid allocator object type!");

    typedef std::allocator_traits<A>          traits_type     ;
    typedef traits_type::allocator_type       allocator_type  ;
    typedef traits_type::size_type            size_type       ;
    typedef traits_type::value_type           pair_type       ;
    typedef std::string_view                  string_type     ;
    typedef string                            key_type        ;
    typedef string const                      const_key       ;
    typedef std::hash<key_type>               hash_type       ;
    typedef std::equal_to<key_type>           equal_type      ;
    typedef plugin_pair                       mapped_type     ;
    typedef std::pair<const_key, plugin_pair> value_type      ;
    typedef Interface                         iface_type      ;
    typedef std::shared_ptr<iface_type>       shared_iface    ;

    typedef std::unordered_map
    <key_type, mapped_type, hash_type, equal_type, allocator_type> map_type;

    typedef mapped_type&                          reference             ;
    typedef mapped_type const&                    const_reference       ;
    typedef mapped_type*                          pointer               ;
    typedef mapped_type const*                    const_pointer         ;
    typedef map_type::iterator                    iterator              ;
    typedef map_type::const_iterator              const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> reverse_const_iterator;

    class plugin_pointer
    {
    public:
        typedef plugin_vars const* const_plugin_pointer;

        constexpr const_plugin_pointer operator -> () const noexcept
        { return &_M_ptr->second; }

        constexpr shared_iface iface () const
        { return std::static_pointer_cast<iface_type> (_M_ptr->second.iface); }

    private:
        constexpr plugin_pointer (const_reference ref) noexcept
        : _M_ptr (&ref)
        { }

        plugin_pointer () = delete;

        template <typename, typename, typename>
        friend class plugin_manager;

    private:
        const_pointer _M_ptr;
    };

    inline constexpr static auto const plugin_main = "plugin_main";

    plugin_manager (plugin_manager&&) = default;
    plugin_manager& operator = (plugin_manager&&) = default;

    void release_all ()
    { _M_gPluginMap.clear (); }

    consteval bool empty () const noexcept
    { return _M_gPluginMap.empty (); }

    constexpr allocator_type get_allocator () const noexcept
    { return _M_gPluginMap.get_allocator (); }

    constexpr loader_type const* loader (const_key& path) const
    { return &_M_gPluginMap[path].first; }

    constexpr plugin_pointer plugin (const_key& path) const
    { return _M_gPluginMap[path]; }

    plugin_manager (allocator_type const& ator = allocator_type ())
    : _M_gPluginMap(ator)
    { }

    template <non_void_t... Args>
    bool load_plugin (const_key& path, memory::memory_resource* rc = nullptr, Args&&... args)
    {
        static_assert (!are_any_of_type_v<traits_enum_t::reference, Args...>,
                "References are not a 'C' concept!");
        static_assert (!are_any_of_type_v<traits_enum_t::class_type, Args...>,
                "Class instances are not allowed as C function arguments!");

        loader_type loader (path);

        if (!loader.is_attached () || !loader.contains (plugin_main)) return false;

        auto plugin = loader[plugin_main](ret<plugin_vars*>,
                                          rc == nullptr ? get_allocator ().resource () : rc,
                                          std::forward<Args> (args)...);

        for (auto& pair : _M_gPluginMap)
        {
            if (std::strcmp(pair.second.second.provides, plugin->provides) == 0) return false;
        }

        _M_gPluginMap.emplace (path, std::make_pair (std::move (loader), std::move(*plugin)));
        return true;
    }

    constexpr bool is_registered (const_key& path) const noexcept
    {
        return _M_gPluginMap.find (path) != _M_gPluginMap.end ();
    }

    constexpr void release_plugin (const_key& path)
    {
        auto it = _M_gPluginMap.find (path);
        if  (it != _M_gPluginMap.end ()) _M_gPluginMap.erase (it);
    }

private:
    map_type mutable _M_gPluginMap;
};

// =========================================================

} } // namespace Process

#endif // __cplusplus
#endif // CPPUAL_PROCESS_PLUGIN_H_
