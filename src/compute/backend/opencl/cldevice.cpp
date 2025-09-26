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

#include "cldevice.h"

namespace cppual::compute::cl {

// =========================================================

namespace { //! optimize for internal unit usage

enum device_internal_type
{
    default_type = CL_DEVICE_TYPE_DEFAULT    ,
    cpu          = CL_DEVICE_TYPE_CPU        ,
    gpu          = CL_DEVICE_TYPE_GPU        ,
    accelerator  = CL_DEVICE_TYPE_ACCELERATOR,
    custom       = CL_DEVICE_TYPE_CUSTOM     ,
    any          = CL_DEVICE_TYPE_ALL
};

enum
{
    device_info_name               = CL_DEVICE_NAME,
    device_info_vendor             = CL_DEVICE_VENDOR,
    device_info_cat                = CL_DEVICE_TYPE,
    device_info_profile            = CL_DEVICE_PROFILE,
    device_info_version            = CL_DEVICE_VERSION,
    device_info_exec_capabilities  = CL_DEVICE_EXECUTION_CAPABILITIES,
    device_info_cache_size         = CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
    device_info_cache_line_size    = CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
    device_info_local_mem_size     = CL_DEVICE_LOCAL_MEM_SIZE,
    device_info_const_mem_size     = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
    device_info_global_mem_size    = CL_DEVICE_GLOBAL_MEM_SIZE,
    device_info_max_compute_units  = CL_DEVICE_MAX_COMPUTE_UNITS,
    device_info_max_mem_alloc_size = CL_DEVICE_MAX_MEM_ALLOC_SIZE,
    device_info_extensions         = CL_DEVICE_EXTENSIONS
};

constexpr compute::device_type convert (device_internal_type type) noexcept
{
    return type == device_internal_type::cpu         ? compute::device_type::cpu         :
           type == device_internal_type::gpu         ? compute::device_type::gpu         :
           type == device_internal_type::accelerator ? compute::device_type::gpgpu       :
           type == device_internal_type::custom      ? compute::device_type::specialized :
                                                       compute::device_type::any         ;
}

// constexpr device_internal_type convert (compute::device_type type) noexcept
// {
//    return type == compute::device_type::cpu         ? device_internal_type::cpu         :
//           type == compute::device_type::gpu         ? device_internal_type::gpu         :
//           type == compute::device_type::gpgpu       ? device_internal_type::accelerator :
//           type == compute::device_type::specialized ? device_internal_type::custom      :
//                                                       device_internal_type::any         ;
// }

} // anonymous namespace

// =========================================================

class platform final : public object<resource_type::instance>
{
public:
    typedef platform                        self_type          ;
    typedef object<resource_type::instance> base_type          ;
    typedef std::size_t                     size_type          ;
    typedef CLObject<platform_id_type>      value_type         ;
    typedef value_type*                     pointer            ;
    typedef resource_handle                 handle_type        ;
    typedef dyn_array<platform_id_type*>       platform_ids_vector;
    typedef device::device_ids_vector       device_ids_vector  ;

    enum class platform_info : platform_info_type
    {
        info_name    = CL_PLATFORM_NAME   ,
        info_vendor  = CL_PLATFORM_VENDOR ,
        info_profile = CL_PLATFORM_PROFILE,
        info_version = CL_PLATFORM_VERSION
    };

    platform () = delete;

    constexpr pointer handle () const noexcept
    { return base_type::handle<pointer> (); }

    string info (platform_info info) const
    { return get_info<string> (static_cast<platform_info_type> (info)); }

    static size_type count () noexcept
    { return all ().size (); }

    inline size_type device_count () const noexcept
    { return _M_devices.size (); }

    static platform_ids_vector& all ()
    {
        static auto inst = get_platforms ();
        return inst;
    }

    device_ids_vector const& devices () const noexcept
    {
        return _M_devices;
    }

    device_ids_vector& devices () noexcept
    {
        return _M_devices;
    }

private:
    platform (pointer platform_handle) noexcept
    : base_type  (platform_handle),
      _M_devices (get_devices  ())
    { }

    static platform_ids_vector get_platforms ()
    {
        return get_object_info<platform_ids_vector> (::clGetPlatformIDs);
    }

    template <typename T>
    T get_info (platform_info_type info) const
    {
        return get_object_info<T> (::clGetPlatformInfo, handle(), info);
    }

    device_ids_vector get_devices () const
    {
        return get_object_info<device_ids_vector> (::clGetDeviceIDs, handle(), device_internal_type::any);
    }

    friend class device;

private:
    device_ids_vector _M_devices;
};

// =========================================================

device::device (pointer handle) noexcept
: base_type (handle)
{
    ::clRetainDevice (handle);
}

device::~device () noexcept
{
    if (handle ()) ::clReleaseDevice (handle ());
}

device::string_type device::name () const
{
    return get_info<string_type> (device_info_name);
}

device::string_type device::vendor () const
{
    return get_info<string_type> (device_info_vendor);
}

device::profile_type device::profile () const
{
    return get_info<string_type> (device_info_profile)    == "EMBEDDED_PROFILE" ?
                                  profile_type::embedded : profile_type::full ;
}

backend_type device::backend () const
{
    return backend_type::opencl;
}

device_ils device::supported_ils () const
{
    supported_ils_type cl_ils (get_info<::cl_device_exec_capabilities> (device_info_exec_capabilities));
    device_ils ils;

    static const string_type ext_spir = "cl_khr_spir";

    if (cl_ils.test (supported_ils::native)) ils += il_type::native;
    if (cl_ils.test (supported_ils::opencl)) ils += il_type::opencl;
    if (supports_extension       (ext_spir)) ils += il_type::spir  ;

    return ils;
}

device_type device::dev_type () const
{
    return convert (device_internal_type (get_info<device_cat_type> (device_info_cat)));
}

device::version_type device::version () const
{
    auto const version_str = get_info<string_type> (device_info_version);

    version_type  ver;
    istringstream stream (version_str);

    //! skip 'OpenCL ' :: assign major :: skip '.' :: assign minor
    ((stream.ignore(7) >> ver.major).ignore(1)) >> ver.minor;

    return ver;
}

device::size_type device::cache_size () const
{
    return get_info<size_type> (device_info_cache_size);
}

device::size_type device::cache_line_size () const
{
    return get_info<size_type> (device_info_cache_line_size);
}

device::size_type device::local_memory_size () const
{
    return get_info<size_type> (device_info_local_mem_size);
}

device::size_type device::const_memory_size () const
{
    return get_info<size_type> (device_info_const_mem_size);
}

device::size_type device::global_memory_size () const
{
    return get_info<size_type> (device_info_global_mem_size);
}

u32 device::compute_units_count () const
{
    return get_info<u32> (device_info_max_compute_units);
}

u64 device::max_memory_alloc_size () const
{
    return get_info<u64> (device_info_max_mem_alloc_size);
}

dyn_array<string_type> device::extensions () const
{
    string_type         extension;
    string_type         extensions = get_info<string_type> (device_info_extensions);
    dyn_array<string_type> extensions_vector;

    if (extensions.empty ()) return extensions_vector;

    istringstream stream (extensions);

    auto const n = static_cast<size_type> (std::count (extensions.begin(), extensions.end(), ' '));

    extensions_vector.reserve (n + 1);

    while (!stream.eof())
    {
        std::getline(stream, extension, ' ');
        if (!extension.empty()) extensions_vector.push_back(extension);
    }

    return extensions_vector;
}

bool device::supports_extension (string_type const& name) const
{
    auto const ext_vector = extensions ();

    return std::find(ext_vector.cbegin(), ext_vector.cend(), name) != ext_vector.cend();
}

device::device_ids_vector device::get_device_ids ()
{
    auto const& platform_ids = platform::all ();
    device_ids_vector device_ids;

    for (auto i = 0U; i < platform_ids.size(); ++i)
    {
        auto const platform_inst = platform (platform_ids[i]);

        for (auto n = 0U; n < platform_inst.count(); ++n)
        {
            device_ids.emplace_back (platform_inst.devices()[i]);
        }
    }

    return device_ids;
}

device::device_ids_vector device::get_device_ids (device_vector const& devs)
{
    device_ids_vector ret_vec;

    ret_vec.reserve (devs.size ());

    for (auto i = 0U; i < devs.size (); ++i)
    {
        ret_vec.push_back (devs[i]->handle<pointer> ());
    }

    return ret_vec;
}

device::device_vector device::get_devices (memory::memory_resource& rc)
{
    device_vector devices;
    auto const    device_ids = device::get_device_ids();

    if (device_ids.empty()) return devices;

    devices.reserve (device_ids.size());

    for (auto i = 0U; i < device_ids.size(); ++i)
        devices.emplace_back(std::allocate_shared<device_interface, device> (rc, device_ids[i]));

    return devices;
}

device::size_type device::count ()
{
    return device::get_device_ids ().size ();
}

// =========================================================

} // namespace CL
