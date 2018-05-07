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

#include <memory>
#include <functional>
#include <unordered_map>
#include <cppual/types.h>
#include <cppual/flags.h>
#include <cppual/signal.h>
#include <cppual/resource.h>

namespace cppual { namespace Process {

class DynLoader : public NonCopyable
{
public:
    typedef int (DLCALL * function_type)();
    typedef std::string   string_type;
    typedef Handle        handle_type;

    enum Flag
    {
        AbsolutePath = 1 << 0, // explicitly provide full path
        AddExt       = 1 << 1  // add library extension (ex. ".so" or ".dll")
    };

    enum class ResolvePolicy : unsigned char
    {
        Static,    // use as data file or load as static library
        Immediate, // resolve everything ot load
        Lazy       // don't resolve any object or function references
    };

    typedef BitSet<DynLoader::Flag> Flags;

    DynLoader () = default;
    DynLoader (DynLoader&&) = default;
    DynLoader& operator = (DynLoader&&) = default;
    bool attach () noexcept;
    void detach () noexcept;

    DynLoader (string_type   path,
               bool          attach = true,
               ResolvePolicy policy = ResolvePolicy::Lazy,
               Flags         flags  = AddExt) noexcept;

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
    TRet call (string_type pName, TArgs&&... args) const
    {
        typedef TRet (* Func)(TArgs...);
        function_type func = get_function (pName.c_str ());

        if (!func) std::bad_function_call ();
        return (*reinterpret_cast<Func> (func))(std::forward<TArgs> (args)...);
    }

private:
    void*         get_address  (string_type::const_pointer name) const noexcept;
    function_type get_function (string_type::const_pointer name) const noexcept;

private:
    handle_type       m_pHandle;
    string_type const m_gLibPath;
    ResolvePolicy     m_eResolve;
};

// =========================================================

struct Plugin
{
    cchar* name;
    cchar* provides;
    cchar* desc;
    void * iface;
    int    version;
};

template <typename Interface,
          typename Allocator =
          std::allocator<std::pair<const string, std::pair<DynLoader, Plugin> > >
          >
class PluginManager
{
public:
    typedef typename Allocator::size_type  size_type;
    typedef typename Allocator::value_type pair_type;
    typedef std::string                    key_type;
    typedef std::string const              const_key;
    typedef std::hash<key_type>            hash_type;
    typedef std::equal_to<key_type>        equal_type;
    typedef std::pair<DynLoader, Plugin>   value_type;
    typedef Interface                      iface_type;
    typedef Allocator                      allocator_type;

    typedef std::unordered_map
    <key_type, value_type, hash_type, equal_type, allocator_type> map_type;

    PluginManager () = default;
    PluginManager (const_key& plugins_dir, allocator_type& = allocator_type ());
    PluginManager (PluginManager&&);
    PluginManager& operator = (PluginManager&&);

    bool load           (const_key& plugins_dir);
    bool load_plugin    (const_key& path);
    bool is_registered  (const_key& name) noexcept;
    void release_plugin (const_key& name);

    void release ()                { m_gPluginMap.clear ();        }
    bool empty   () const noexcept { return m_gPluginMap.empty (); }

    DynLoader* plugin (const_key& plugin_name) const noexcept
    { return &m_gPluginMap[plugin_name].first; }

    key_type description (const_key& plugin_name) const noexcept
    { return m_gPluginMap[plugin_name].second.desc; }

    int version (const_key& plugin_name) const noexcept
    { return m_gPluginMap[plugin_name].second.version; }

    iface_type* construct (const_key& plugin_name) const noexcept
    { return static_cast<iface_type*> (m_gPluginMap[plugin_name].second.iface); }

private:
    mutable map_type m_gPluginMap;
};

} } // namespace Process

#endif // __cplusplus
#endif // CPPUAL_PROCESS_PLUGIN_H_
