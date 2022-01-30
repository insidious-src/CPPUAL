/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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

namespace cppual { namespace compute { namespace cl {

// =========================================================

namespace { // optimize for internal unit usage

enum Type
{
    Default     = CL_DEVICE_TYPE_DEFAULT    ,
    CPU         = CL_DEVICE_TYPE_CPU        ,
    GPU         = CL_DEVICE_TYPE_GPU        ,
    Accelerator = CL_DEVICE_TYPE_ACCELERATOR,
    Custom      = CL_DEVICE_TYPE_CUSTOM     ,
    Any         = CL_DEVICE_TYPE_ALL
};

enum DeviceInfo
{
    DeviceInfoName             = CL_DEVICE_NAME,
    DeviceInfoVendor           = CL_DEVICE_VENDOR,
    DeviceInfoType             = CL_DEVICE_TYPE,
    DeviceInfoProfile          = CL_DEVICE_PROFILE,
    DeviceInfoVersion          = CL_DEVICE_VERSION,
    DeviceInfoExecCapabilities = CL_DEVICE_EXECUTION_CAPABILITIES,
    DeviceInfoCacheSize        = CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
    DeviceInfoCacheLineSize    = CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
    DeviceInfoLocalMemSize     = CL_DEVICE_LOCAL_MEM_SIZE,
    DeviceInfoConstMemSize     = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
    DeviceInfoGlobalMemSize    = CL_DEVICE_GLOBAL_MEM_SIZE,
    DeviceInfoMaxComputeUnits  = CL_DEVICE_MAX_COMPUTE_UNITS,
    DeviceInfoMaxMemAllocSize  = CL_DEVICE_MAX_MEM_ALLOC_SIZE,
    DeviceInfoExtensions       = CL_DEVICE_EXTENSIONS
};

constexpr compute::device_category convert (Type type) noexcept
{
    return type == CPU         ? compute::device_category::cpu         :
           type == GPU         ? compute::device_category::gpu         :
           type == Accelerator ? compute::device_category::gpgpu       :
           type == Custom      ? compute::device_category::specialized :
                                 compute::device_category::any         ;
}

//constexpr Type convert (Compute::DeviceType type) noexcept
//{
//    return type == Compute::DeviceType::CPU         ? CPU         :
//           type == Compute::DeviceType::GPU         ? GPU         :
//           type == Compute::DeviceType::GPGPU       ? Accelerator :
//           type == Compute::DeviceType::Specialized ? Custom      :
//                                                      Any         ;
//}

} // anonymous namespace

// =========================================================

class platform final : public interface<resource_type::instance>
{
public:
    typedef std::size_t               size_type          ;
    typedef vector<platform_type*>    platform_ids_vector;
    typedef device::device_ids_vector device_ids_vector  ;

    enum Info
    {
        Name    = CL_PLATFORM_NAME   ,
        Vendor  = CL_PLATFORM_VENDOR ,
        Profile = CL_PLATFORM_PROFILE,
        Version = CL_PLATFORM_VERSION
    };

    platform () = delete;

    string info          (Info);
    bool   has_extension (string_type const&);

    static size_type count () noexcept
    { return all ().size  (); }

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

private:
    device_ids_vector _M_devices;

    platform (pointer _handle) noexcept
    : interface(_handle),
      _M_devices (get_object_info<device_ids_vector>(::clGetDeviceIDs, handle(), Type::Any))
    { }

    static platform_ids_vector get_platforms ()
    {
        return get_object_info<platform_ids_vector>(::clGetPlatformIDs);
    }

    friend class device;
};

// =========================================================

device::string_type device::name() const
{
    return get_info<string_type>(DeviceInfoName);
}

device::string_type device::vendor() const
{
    return get_info<string_type>(DeviceInfoVendor);
}

device::profile_type device::profile() const
{
    return get_info<string_type>(DeviceInfoProfile) == "EMBEDDED_PROFILE" ?
                profile_type::embedded : profile_type::full;
}

backend_type device::backend() const
{
    return backend_type::opencl;
}

device_ils device::supported_ils() const
{
    supported_ils_type cl_ils (get_info<::cl_device_exec_capabilities>(DeviceInfoExecCapabilities));
    device_ils ils;

    if (cl_ils.test(SupportedILs::Native)) ils += il_type::native;
    if (cl_ils.test(SupportedILs::OpenCL)) ils += il_type::opencl;
    if (supports_extension("cl_khr_spir")) ils += il_type::spir  ;

    return ils;
}

device_category device::dev_type() const
{
    return convert(Type(get_info<device_category_type>(DeviceInfoType)));
}

device::version_type device::version() const
{
    auto const version_str = get_info<string_type>(DeviceInfoVersion);

    version_type version;
    istringstream stream(version_str);

    // skip 'OpenCL ' :: assign major :: skip '.' :: assign minor
    ((stream.ignore(7) >> version.major).ignore(1)) >> version.minor;

    return version;
}

device::size_type device::cache_size() const
{
    return get_info<size_type>(DeviceInfoCacheSize);
}

device::size_type device::cache_line_size() const
{
    return get_info<size_type>(DeviceInfoCacheLineSize);
}

device::size_type device::local_memory_size() const
{
    return get_info<size_type>(DeviceInfoLocalMemSize);
}

device::size_type device::const_memory_size() const
{
    return get_info<size_type>(DeviceInfoConstMemSize);
}

device::size_type device::global_memory_size() const
{
    return get_info<size_type>(DeviceInfoGlobalMemSize);
}

u32 device::compute_units() const
{
    return get_info<u32>(DeviceInfoMaxComputeUnits);
}

u64 device::max_memory_alloc_size() const
{
    return get_info<u64>(DeviceInfoMaxMemAllocSize);
}

vector<string_type> device::extensions() const
{
    string_type         extension;
    string_type         extensions = get_info<string_type>(DeviceInfoExtensions);
    vector<string_type> extensions_vector;

    if (extensions.empty()) return extensions_vector;

    istringstream stream(extensions);

    auto const n = static_cast<size_type>(std::count(extensions.begin(), extensions.end(), ' '));

    extensions_vector.reserve(n + 1);

    while (!stream.eof())
    {
        std::getline(stream, extension, ' ');
        if (!extension.empty()) extensions_vector.push_back(extension);
    }

    return extensions_vector;
}

bool device::supports_extension(string_type const& name) const
{
    auto const ext_vector = extensions();

    return std::find(ext_vector.cbegin(), ext_vector.cend(), name) != ext_vector.cend();
}

device::device_ids_vector device::get_device_ids()
{
    typedef platform::platform_ids_vector platform_ids_vector;

    platform_ids_vector const& platform_ids = platform::all();
    device_ids_vector          device_ids;

    for (auto i = 0U; i < platform_ids.size(); ++i)
    {
        auto const platform_inst = platform(platform_ids[i]);

        for (auto n = 0U; n < platform_inst.count(); ++n)
        {
            device_ids.emplace_back(platform_inst.devices()[i]);
        }
    }

    return device_ids;
}

device::device_vector device::get_devices(memory::memory_resource& rc)
{
    device_vector devices;
    auto const    device_ids = device::get_device_ids();

    if (device_ids.empty()) return devices;

    devices.reserve(device_ids.size());

    for (auto i = 0U; i < device_ids.size(); ++i)
        devices.emplace_back(memory::allocate_shared<device, device_interface>(&rc, device_ids[i]));

    return devices;
}

device::size_type device::count()
{
    return device::get_device_ids().size();
}

// =========================================================

} } } // namespace CL
