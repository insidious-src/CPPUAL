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

#include "cldevice.h"

namespace cppual { namespace Compute { namespace CL {

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

constexpr Compute::DeviceType convert (Type type) noexcept
{
    return type == CPU         ? Compute::DeviceType::CPU         :
           type == GPU         ? Compute::DeviceType::GPU         :
           type == Accelerator ? Compute::DeviceType::GPGPU       :
           type == Custom      ? Compute::DeviceType::Specialized :
                                 Compute::DeviceType::Any         ;
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

class Platform final : public Interface<ResourceType::Instance>
{
public:
    typedef std::size_t               size_type          ;
    typedef vector<platform_type*>    platform_ids_vector;
    typedef Device::device_ids_vector device_ids_vector  ;

    enum Info
    {
        Name    = CL_PLATFORM_NAME   ,
        Vendor  = CL_PLATFORM_VENDOR ,
        Profile = CL_PLATFORM_PROFILE,
        Version = CL_PLATFORM_VERSION
    };

    Platform () = delete;

    string info          (Info);
    bool   has_extension (string_type const&);

    static size_type count () noexcept
    { return all ().size  (); }

    inline size_type device_count () const noexcept
    { return m_devices.size (); }

    static platform_ids_vector& all ()
    {
        static auto inst = get_platforms ();
        return inst;
    }

    device_ids_vector const& devices () const noexcept
    {
        return m_devices;
    }

private:
    device_ids_vector m_devices;

    Platform (pointer _handle) noexcept
    : Interface(_handle),
      m_devices (get_object_info<device_ids_vector>(::clGetDeviceIDs, handle(), Type::Any))
    { }

    static platform_ids_vector get_platforms ()
    {
        return get_object_info<platform_ids_vector>(::clGetPlatformIDs);
    }

    friend class Device;
};

// =========================================================

Device::string_type Device::name() const
{
    return get_info<string_type>(DeviceInfoName);
}

Device::string_type Device::vendor() const
{
    return get_info<string_type>(DeviceInfoVendor);
}

Device::Profile Device::profile() const
{
    return get_info<string_type>(DeviceInfoProfile) == "EMBEDDED_PROFILE" ?
                Profile::Embedded : Profile::Full;
}

Backend Device::backend() const
{
    return Backend::OpenCL;
}

device_ils Device::supportedILs() const
{
    supported_ils_type cl_ils (get_info<::cl_device_exec_capabilities>(DeviceInfoExecCapabilities));
    device_ils ils;

    if (cl_ils.test(SupportedILs::Native)) ils += IL::Native;
    if (cl_ils.test(SupportedILs::OpenCL)) ils += IL::OpenCL;
    if (supports_extension("cl_khr_spir")) ils += IL::SPIR  ;

    return ils;
}

DeviceType Device::devType() const
{
    return convert(Type(get_info<device_category_type>(DeviceInfoType)));
}

Version Device::version() const
{
    auto const version_str = get_info<string_type>(DeviceInfoVersion);

    Version version;
    istringstream stream(version_str);

    // skip 'OpenCL ' :: assign major :: skip '.' :: assign minor
    ((stream.ignore(7) >> version.major).ignore(1)) >> version.minor;

    return version;
}

Device::size_type Device::cacheSize() const
{
    return get_info<size_type>(DeviceInfoCacheSize);
}

Device::size_type Device::cacheLineSize() const
{
    return get_info<size_type>(DeviceInfoCacheLineSize);
}

Device::size_type Device::localMemorySize() const
{
    return get_info<size_type>(DeviceInfoLocalMemSize);
}

Device::size_type Device::constMemorySize() const
{
    return get_info<size_type>(DeviceInfoConstMemSize);
}

Device::size_type Device::globalMemorySize() const
{
    return get_info<size_type>(DeviceInfoGlobalMemSize);
}

u32 Device::computeUnits() const
{
    return get_info<u32>(DeviceInfoMaxComputeUnits);
}

u64 Device::maxMemoryAllocSize() const
{
    return get_info<u64>(DeviceInfoMaxMemAllocSize);
}

vector<string_type> Device::extensions() const
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

bool Device::supports_extension(string_type const& name) const
{
    auto const ext_vector = extensions();

    return std::find(ext_vector.cbegin(), ext_vector.cend(), name) != ext_vector.cend();
}

Device::device_ids_vector Device::get_device_ids()
{
    typedef Platform::platform_ids_vector platform_ids_vector;

    platform_ids_vector const& platform_ids = Platform::all();
    device_ids_vector          device_ids;

    for (auto i = 0U; i < platform_ids.size(); ++i)
    {
        auto const platform = Platform(platform_ids[i]);

        for (auto n = 0U; n < platform.count(); ++n)
        {
            device_ids.emplace_back(platform.devices()[i]);
        }
    }

    return device_ids;
}

Device::device_vector Device::get_devices(Memory::MemoryResource& rc)
{
    device_vector devices;
    auto const    device_ids = Device::get_device_ids();

    if (device_ids.empty()) return devices;

    devices.reserve(device_ids.size());

    for (auto i = 0U; i < device_ids.size(); ++i)
        devices.emplace_back(Memory::allocate_shared<Device, IDevice>(&rc, device_ids[i]));

    return devices;
}

Device::size_type Device::count()
{
    return Device::get_device_ids().size();
}

// =========================================================

} } } // namespace CL
