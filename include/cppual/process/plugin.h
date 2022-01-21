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

#ifndef CPPUAL_PROCESS_PLUGIN_H_
#define CPPUAL_PROCESS_PLUGIN_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/noncopyable.h>
#include <cppual/resource.h>
#include <cppual/concepts.h>
#include <cppual/string.h>
#include <cppual/meta.h>

#include <memory>
#include <cstring>
#include <functional>
#include <unordered_map>

namespace cppual { namespace Process {

class DynLoader : public NonCopyable
{
public:
    typedef call_ret_t (DLCALL * function_type)();
    typedef string string_type;
    typedef Handle handle_type;

    enum class ResolvePolicy : byte
    {
        Static,    // use as data file or load as static library
        Immediate, // resolve everything on load
        Lazy       // don't resolve any object or function references
    };

    DynLoader () = default;
    DynLoader (DynLoader&&) noexcept;
    DynLoader& operator = (DynLoader&&) noexcept;
    bool attach () noexcept;
    void detach () noexcept;

    DynLoader (string_type   path,
               bool          attach = true,
               ResolvePolicy policy = ResolvePolicy::Lazy) noexcept;

    static string_type extension () noexcept;

    string_type   path        () const noexcept { return m_gLibPath; }
    void*         handle      () const noexcept { return m_pHandle;  }
    ResolvePolicy policy      () const noexcept { return m_eResolve; }
    bool          is_attached () const noexcept { return m_pHandle;  }

    ~DynLoader ()
    { if (m_eResolve != ResolvePolicy::Static) detach (); }

    bool contains (string_type const& gName) const noexcept
    { return get_address (gName.c_str ()); }

    bool contains (cchar* pName) const noexcept
    { return get_address (pName); }

    template <typename T>
    T* import (string_type pName) const noexcept
    { return static_cast<T*> (get_address (pName.c_str ())); }

    template <typename TRet, typename... TArgs>
    bool import (string_type pName, TRet(*& fn)(TArgs...)) const
    {
        typedef TRet (* Func)(TArgs...);
        function_type func = get_function (pName.c_str ());
        return fn = reinterpret_cast<Func> (func);
    }

    template <typename TRet, typename... TArgs>
    TRet call (string_type pName, TArgs... args) const
    {
        auto fn = direct_cast<TRet(*)(TArgs...)> (get_function (pName.c_str ()));

        if (!fn) std::bad_function_call ();
        return fn (std::forward<TArgs> (args)...);
    }

private:
    void*         get_address  (string_type::const_pointer name) const noexcept;
    function_type get_function (string_type::const_pointer name) const noexcept;

private:
    handle_type   m_pHandle ;
    string_type   m_gLibPath;
    ResolvePolicy m_eResolve;
};

// =========================================================

struct Plugin
{
    cchar*                name     { };
    cchar*                provides { };
    cchar*                desc     { };
    uint                  verMajor { };
    uint                  verMinor { };
    u64                   priority { };
    std::shared_ptr<void> iface    { };

    inline Plugin () noexcept = default;
    inline Plugin (Plugin const&) = default;
    inline Plugin& operator = (Plugin const&) = default;

    inline Plugin (Plugin&& obj)
    : name(obj.name),
      provides(obj.provides),
      desc(obj.desc),
      verMajor(obj.verMajor),
      verMinor(obj.verMinor),
      iface(std::move(obj.iface))
    { }

    inline Plugin& operator = (Plugin&& obj)
    {
        if (this == &obj) return *this;

        name     = obj.name;
        provides = obj.provides;
        desc     = obj.desc;
        verMajor = obj.verMajor;
        verMinor = obj.verMinor;
        iface    = std::move(obj.iface);

        return *this;
    }
};

typedef std::pair<DynLoader const, Plugin> plugin_pair;

// =========================================================

template <typename Interface,
          typename Allocator = Memory::Allocator< std::pair<string const, plugin_pair> >,
          typename = typename std::enable_if<
              std::is_same<typename std::allocator_traits<Allocator>::value_type,
                           std::pair<string const, plugin_pair>
                           >{}>::type
          >
class PluginManager : public NonCopyable
{
public:
    static_assert (Memory::is_allocator<Allocator>::value, "invalid allocator object type!");

    typedef typename std::allocator_traits<Allocator>::allocator_type allocator_type;
    typedef typename std::allocator_traits<Allocator>::size_type      size_type     ;
    typedef typename std::allocator_traits<Allocator>::value_type     pair_type     ;
    typedef string                                                    key_type      ;
    typedef string const                                              const_key     ;
    typedef std::hash<key_type>                                       hash_type     ;
    typedef std::equal_to<key_type>                                   equal_type    ;
    typedef Movable<DynLoader>                                        loader_type   ;
    typedef plugin_pair                                               value_type    ;
    typedef Interface                                                 iface_type    ;
    typedef std::shared_ptr<iface_type>                               shared_iface  ;

    typedef std::unordered_map
    <key_type, value_type, hash_type, equal_type, allocator_type> map_type;

    typedef typename map_type::mapped_type&       reference      ;
    typedef typename map_type::mapped_type const& const_reference;
    typedef typename map_type::mapped_type*       pointer        ;
    typedef typename map_type::mapped_type const* const_pointer  ;
    typedef typename map_type::iterator           iterator       ;
    typedef typename map_type::const_iterator     const_iterator ;

    class plugin_pointer
    {
    public:
        typedef Plugin const* const_plugin_pointer;

        plugin_pointer() = delete;

        constexpr const_plugin_pointer operator -> () const noexcept
        { return &_M_ref->second; }

        constexpr shared_iface interface () const
        { return std::static_pointer_cast<iface_type>(_M_ref->second.iface); }

    private:
        constexpr plugin_pointer (const_reference ref) noexcept
        : _M_ref(&ref)
        { }

        template <typename, typename, typename>
        friend class PluginManager;

    private:
        const_pointer _M_ref;
    };

    constexpr static const auto plugin_main = "plugin_main";

    PluginManager (PluginManager&&) = default;
    PluginManager& operator = (PluginManager&&) = default;

    void release_all ()
    { m_gPluginMap.clear(); }

    constexpr bool empty () const noexcept
    { return m_gPluginMap.empty (); }

    constexpr allocator_type get_allocator () const noexcept
    { return m_gPluginMap.get_allocator (); }

    loader_type const* loader (const_key& path) const
    { return &m_gPluginMap[path].first; }

    plugin_pointer plugin (const_key& path) const
    { return m_gPluginMap[path]; }

    PluginManager (allocator_type const& ator = allocator_type ())
    : m_gPluginMap(ator)
    { }

    template <typename... Args>
    bool load_plugin (const_key& path, Memory::MemoryResource* rc = nullptr, Args... args)
    {
        static_assert (!are_any_references<Args...>::value, "References are not a 'C' concept!");

        loader_type loader (path);

        if (!loader.is_attached () || !loader.contains (plugin_main)) return false;

        Plugin* plugin = loader.call<Plugin*> (plugin_main,
                                               rc == nullptr ? get_allocator().resource() : rc,
                                               std::forward<Args> (args)...);

        for (auto& pair : m_gPluginMap)
        {
            if (std::strcmp(pair.second.second.provides, plugin->provides) == 0)
                return false;
        }

        m_gPluginMap.try_emplace (path, std::make_pair (std::move (loader), std::move(*plugin)));
        return true;
    }

    bool is_registered (const_key& path) const noexcept
    {
        return m_gPluginMap.find (path) != m_gPluginMap.end ();
    }

    void release_plugin (const_key& path)
    {
        auto it = m_gPluginMap.find (path);
        if  (it != m_gPluginMap.end ()) m_gPluginMap.erase (it);
    }

private:
    mutable map_type m_gPluginMap;
};

} } // namespace Process

#endif // __cplusplus
#endif // CPPUAL_PROCESS_PLUGIN_H_
