/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_PROCESS_MODULE_H_
#define CPPUAL_PROCESS_MODULE_H_
#ifdef __cplusplus

#include <memory>
#include <functional>
#include <unordered_map>
#include <cppual/types.h>
#include <cppual/flags.h>
#include <cppual/signal.h>
#include <cppual/noncopyable.h>

using std::string;
using std::shared_ptr;
using std::unordered_map;

namespace cppual { namespace Process {

class Module final : public NonCopyable
{
public:
    typedef int (DLCALL * function_type)();

    enum Flag
    {
        AbsolutePath = 1 << 1, // explicitly provide full path
        AddExt       = 1 << 2  // add library extension (ex. ".so" or ".dll")
    };

    enum class ResolvePolicy : unsigned char
    {
        Static = 0, // use as data file or load as static library
        Immediate,  // resolve everything ot load
        Lazy  // don't resolve any object or function references
    };

    typedef BitSet<Module::Flag> Flags;

    Module () = default;
    Module (Module&&) = default;
    Module& operator = (Module&&) = default;
    bool attach () noexcept;
    void detach () noexcept;

    Module (cchar*        path,
            bool          attach = true,
            ResolvePolicy policy = ResolvePolicy::Lazy,
            Flags         flags  = AddExt) noexcept;

    string const& path () const noexcept { return m_gLibPath; }
    void*         handle () const noexcept { return m_pHandle; }
    ResolvePolicy policy () const noexcept { return m_eResolve; }
    bool          is_attached () const noexcept { return m_pHandle; }

    bool contains (string const& gName) const noexcept
    { return get_address (gName.c_str ()); }

    bool contains (cchar* pName) const noexcept
    { return get_address (pName); }

    ~Module ()
    { if (m_eResolve != ResolvePolicy::Static) detach (); }

    template <typename T>
    T* get (cchar* pName) const noexcept
    { return static_cast<T*> (get_address (pName)); }

    template <typename TRet, typename... TArgs>
    bool get (cchar* pName, TRet(*& fn)(TArgs...)) const
    {
        typedef TRet (* Func)(TArgs...);
        function_type func = get_function (pName);
        return fn = reinterpret_cast<Func> (func);
    }

    template <typename TRet, typename... TArgs>
    TRet call (cchar* pName, TArgs&&... args) const
    {
        typedef TRet (* Func)(TArgs...);
        function_type func = get_function (pName);

        if (!func) std::bad_function_call ();
        return (*reinterpret_cast<Func> (func))(std::forward<TArgs> (args)...);
    }

private:
    void*         get_address  (cchar* name) const noexcept;
    function_type get_function (cchar* name) const noexcept;

private:
    void*          m_pHandle;
    string const  m_gLibPath;
    ResolvePolicy m_eResolve;
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

template <typename Iterface,
          typename Allocator =
          std::allocator<std::pair<const string, std::pair<Module, Plugin> > >
          >
class PluginManager
{
public:
    typedef typename Allocator::size_type  size_type;
    typedef typename Allocator::value_type pair_type;
    typedef string                         key_type;
    typedef string const                   const_key;
    typedef std::hash<key_type>            hash_type;
    typedef std::equal_to<key_type>        equal_type;
    typedef std::pair<Module, Plugin>      value_type;
    typedef Iterface                       iface_type;
    typedef Allocator                      allocator_type;

    typedef unordered_map
    <key_type, value_type, hash_type, equal_type, allocator_type> map_type;

    PluginManager () = default;
    PluginManager (const_key& modules_dir, allocator_type& = allocator_type ());
    PluginManager (PluginManager&&);
    PluginManager& operator = (PluginManager&&);

    bool load (const_key& modules_dir);
    bool load_module (const_key& path);
    bool is_registered (const_key& name) noexcept;
    void release_module (const_key& name);

    void release ()                { m_gModuleMap.clear ();        }
    bool empty   () const noexcept { return m_gModuleMap.empty (); }

    Module* module (const_key& module_name) const noexcept
    { return &m_gModuleMap[module_name].first; }

    key_type description (const_key& module_name) const noexcept
    { return m_gModuleMap[module_name].second.desc; }

    int version (const_key& module_name) const noexcept
    { return m_gModuleMap[module_name].second.version; }

    iface_type* construct (const_key& plugin_name) const noexcept
    { return static_cast<iface_type*> (m_gModuleMap[plugin_name].second.iface); }

private:
    mutable map_type m_gModuleMap;
};

} } // namespace Process

#endif // __cplusplus
#endif // CPPUAL_PROCESS_MODULE_H_
