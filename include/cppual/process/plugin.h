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
#include <cppual/interface>
#include <cppual/containers>
#include <cppual/noncopyable>
#include <cppual/memory_allocator>

#include <memory>
#include <cstring>
#include <version>
#include <string_view>

// =========================================================

namespace cppual::process {

// =========================================================

class SHARED_API dyn_loader : public non_copyable
{
public:
    typedef dyn_loader                self_type    ;
    typedef string                    string_type  ;
    typedef fstring_view              string_view  ;
    typedef resource_handle           handle_type  ;
    typedef void *                    pointer      ;
    typedef function_proxy<self_type> fn_proxy_type;
    typedef cchar*                    char_ptr     ;

    using function_type = call_ret_t (DLCALL*)();

    typedef enum class resolve_policy : u8
    {
        //! don't resolve any object or function references
        lazy,
        //! use as data file or load as a static library
        statically,
        //! resolve everything on load
        immediate
    }
    const const_resolve_policy;

    dyn_loader () = default;
    dyn_loader (self_type&&) noexcept;
    self_type& operator = (self_type&&) noexcept;
    bool attach () noexcept;
    void detach () noexcept;

    dyn_loader (string_view const& path,
                bool               attach = true,
                resolve_policy     policy = resolve_policy::lazy) noexcept;

    static string_view extension () noexcept;

    consteval string_view    path        () const noexcept { return _M_gLibPath; }
    constexpr pointer        handle      () const noexcept { return _M_pHandle ; }
    constexpr resolve_policy policy      () const noexcept { return _M_eResolve; }
    constexpr bool           is_attached () const noexcept { return _M_pHandle ; }

    constexpr ~dyn_loader ()
    { if (_M_eResolve != resolve_policy::statically) detach (); }

    constexpr bool contains (string_view const& gName) const noexcept
    { return get_address (gName) != nullptr; }

    consteval fn_proxy_type operator [] (string_view const& fn_name) const noexcept
    { return  fn_proxy_type (*this, fn_name); }

    template <non_function T = void>
    constexpr T* import (string_view const& pName) const
    { return static_cast<T*> (get_address (pName)); }

    template <static_function F = void(*)()>
    constexpr F import (string_view const& pName) const
    { return direct_cast<F> (get_function (pName)); }

    template <string_view K, typename R = void, typename... Args>
    constexpr R invoke (Args&&... args) const
    { return (*direct_cast<R(*)(Args...)> (get_function (K.data ())))(std::forward<Args> (args)...); }

private:
    pointer       get_address  (string_view const& name) const;
    function_type get_function (string_view const& name) const;

private:
    handle_type    _M_pHandle  { };
    string_type    _M_gLibPath { };
    resolve_policy _M_eResolve { };
};

// =========================================================

extern "C" typedef struct SHARED_API plugin_vars
{
    typedef plugin_vars self_type;

    cchar*                name     { };
    cchar*                provides { };
    cchar*                desc     { };
    uint                  verMajor { };
    uint                  verMinor { };
    u64                   priority { };
    std::shared_ptr<void> iface    { };
    dyn_array<cchar*>     required { };

    constexpr plugin_vars (self_type const&)           = default;
    constexpr self_type& operator = (self_type const&) = default;

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

    constexpr plugin_vars (self_type&& obj)
    : name     (obj.name),
      provides (obj.provides),
      desc     (obj.desc),
      verMajor (obj.verMajor),
      verMinor (obj.verMinor),
      iface    (std::move (obj.iface   )),
      required (std::move (obj.required))
    { }

    constexpr self_type& operator = (self_type&& obj)
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
}
const const_plugin_vars;

typedef is_movable_t<dyn_loader>                  loader_type;
typedef std::pair<loader_type const, plugin_vars> plugin_pair;

// =========================================================

template <non_void Interface,
          allocator_like A = memory::allocator
          <std::pair<loader_type::string_type const, plugin_pair>>
          >
class SHARED_API plugin_manager : public non_copyable
{
public:
    typedef plugin_manager<Interface, A>      self_type     ;
    typedef memory::allocator_traits<A>       traits_type   ;
    typedef traits_type::allocator_type       allocator_type;
    typedef traits_type::size_type            size_type     ;
    typedef string                            string_type   ;
    typedef string_type                       key_type      ;
    typedef key_type const                    const_key     ;
    typedef plugin_pair                       mapped_type   ;
    typedef mapped_type const                 const_mapped  ;
    typedef std::pair<const_key, plugin_pair> value_type    ;
    typedef std::string_view                  string_view   ;
    typedef std::hash<key_type>               hash_type     ;
    typedef std::equal_to<key_type>           equal_type    ;
    typedef Interface                         iface_type    ;
    typedef std::shared_ptr<iface_type>       shared_iface  ;

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

        template <non_void, allocator_like>
        friend class plugin_manager;

    private:
        const_pointer _M_ptr;
    };

    inline constexpr static cchar* plugin_main = "plugin_main";

    constexpr plugin_manager (plugin_manager&&) = default;
    constexpr plugin_manager& operator = (plugin_manager&&) = default;

    constexpr void release_all ()
    { _M_gPluginMap.clear (); }

    constexpr bool empty () const noexcept
    { return _M_gPluginMap.empty (); }

    constexpr allocator_type get_allocator () const noexcept
    { return _M_gPluginMap.get_allocator (); }

    constexpr loader_type const* loader (const_key& path) const
    { return &_M_gPluginMap[path].first; }

    constexpr plugin_pointer plugin (const_key& path) const
    { return _M_gPluginMap[path]; }

    constexpr plugin_manager (allocator_type const& ator = allocator_type ())
    : _M_gPluginMap (ator)
    { }

    template <typename... Args>
    constexpr bool load_plugin (const_key& path, memory::memory_resource* rc = nullptr, Args&&... args)
    {
        static_assert (!are_any_of_type_v<traits_enum_t::reference, Args...>,
                "references are not a 'C' concept!");
        static_assert (!are_any_of_type_v<traits_enum_t::structure, Args...>,
                "class instances are not allowed as 'C' function arguments!");

        loader_type loader (path);

        if (!loader.is_attached () || !loader.contains (plugin_main)) return false;

        plugin_vars* plugin = loader[plugin_main](ret<plugin_vars*>,
                                                  rc == nullptr ? get_allocator ().resource () : rc,
                                                  std::forward<Args> (args)...);

        for (value_type& pair : _M_gPluginMap)
        {
            if (string_view (pair.second.second.provides) == string_view (plugin->provides)) return false;
        }

        _M_gPluginMap.emplace (path, std::make_pair (std::move (loader), std::move (*plugin)));
        return true;
    }

    constexpr bool is_registered (const_key& path) const noexcept
    {
        return _M_gPluginMap.find (path) != _M_gPluginMap.end ();
    }

    constexpr void release_plugin (const_key& path)
    {
        auto it  = _M_gPluginMap.find (path);
        if  (it != _M_gPluginMap.end ()) _M_gPluginMap.erase (it);
    }

private:
    map_type mutable _M_gPluginMap;
};

// =========================================================

} // namespace Process

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_PROCESS_PLUGIN_H_
