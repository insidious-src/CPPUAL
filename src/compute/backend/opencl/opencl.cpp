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

#include "opencl.h"

//#include <cstring>
//#include <memory>

namespace cppual::compute::cl {

// =========================================================

namespace { // optimize for internal unit usage



} // anonymous namespace

// =========================================================

cchar* opencl_error::to_string (i32 error)
{
    switch (error)
    {
    case success: return "Success";
    case CL_DEVICE_NOT_FOUND: return "Device Not Found";
    case CL_DEVICE_NOT_AVAILABLE: return "Device Not Available";
    case CL_COMPILER_NOT_AVAILABLE: return "Compiler Not Available";
    case CL_MEM_OBJECT_ALLOCATION_FAILURE: return "Memory Object Allocation Failure";
    case CL_OUT_OF_RESOURCES: return "Out of Resources";
    case CL_OUT_OF_HOST_MEMORY: return "Out of Host Memory";
    case CL_PROFILING_INFO_NOT_AVAILABLE: return "Profiling Information Not Available";
    case CL_MEM_COPY_OVERLAP: return "Memory Copy Overlap";
    case CL_IMAGE_FORMAT_MISMATCH: return "Image Format Mismatch";
    case CL_IMAGE_FORMAT_NOT_SUPPORTED: return "Image Format Not Supported";
    case CL_BUILD_PROGRAM_FAILURE: return "Build Program Failure";
    case CL_MAP_FAILURE: return "Map Failure";
    case CL_INVALID_VALUE: return "Invalid Value";
    case CL_INVALID_DEVICE_TYPE: return "Invalid Device Type";
    case CL_INVALID_PLATFORM: return "Invalid Platform";
    case CL_INVALID_DEVICE: return "Invalid Device";
    case CL_INVALID_CONTEXT: return "Invalid Context";
    case CL_INVALID_QUEUE_PROPERTIES: return "Invalid Queue Properties";
    case CL_INVALID_COMMAND_QUEUE: return "Invalid Command Queue";
    case CL_INVALID_HOST_PTR: return "Invalid Host Pointer";
    case CL_INVALID_MEM_OBJECT: return "Invalid Memory Object";
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: return "Invalid Image Format Descriptor";
    case CL_INVALID_IMAGE_SIZE: return "Invalid Image Size";
    case CL_INVALID_SAMPLER: return "Invalid Sampler";
    case CL_INVALID_BINARY: return "Invalid Binary";
    case CL_INVALID_BUILD_OPTIONS: return "Invalid Build Options";
    case CL_INVALID_PROGRAM: return "Invalid Program";
    case CL_INVALID_PROGRAM_EXECUTABLE: return "Invalid Program Executable";
    case CL_INVALID_KERNEL_NAME: return "Invalid Kernel Name";
    case CL_INVALID_KERNEL_DEFINITION: return "Invalid Kernel Definition";
    case CL_INVALID_KERNEL: return "Invalid Kernel";
    case CL_INVALID_ARG_INDEX: return "Invalid Argument Index";
    case CL_INVALID_ARG_VALUE: return "Invalid Argument Value";
    case CL_INVALID_ARG_SIZE: return "Invalid Argument Size";
    case CL_INVALID_KERNEL_ARGS: return "Invalid Kernel Arguments";
    case CL_INVALID_WORK_DIMENSION: return "Invalid Work Dimension";
    case CL_INVALID_WORK_GROUP_SIZE: return "Invalid Work Group Size";
    case CL_INVALID_WORK_ITEM_SIZE: return "Invalid Work Item Size";
    case CL_INVALID_GLOBAL_OFFSET: return "Invalid Global Offset";
    case CL_INVALID_EVENT_WAIT_LIST: return "Invalid Event Wait List";
    case CL_INVALID_EVENT: return "Invalid Event";
    case CL_INVALID_OPERATION: return "Invalid Operation";
    case CL_INVALID_GL_OBJECT: return "Invalid GL Object";
    case CL_INVALID_BUFFER_SIZE: return "Invalid Buffer Size";
    case CL_INVALID_MIP_LEVEL: return "Invalid MIP Level";
    case CL_INVALID_GLOBAL_WORK_SIZE: return "Invalid Global Work Size";
    #ifdef CL_VERSION_1_2
    case CL_COMPILE_PROGRAM_FAILURE: return "Compile Program Failure";
    case CL_LINKER_NOT_AVAILABLE: return "Linker Not Available";
    case CL_LINK_PROGRAM_FAILURE: return "Link Program Failure";
    case CL_DEVICE_PARTITION_FAILED: return "Device Partition Failed";
    case CL_KERNEL_ARG_INFO_NOT_AVAILABLE: return "Kernel Argument Info Not Available";
    case CL_INVALID_PROPERTY: return "Invalid Property";
    case CL_INVALID_IMAGE_DESCRIPTOR: return "Invalid Image Descriptor";
    case CL_INVALID_COMPILER_OPTIONS: return "Invalid Compiler Options";
    case CL_INVALID_LINKER_OPTIONS: return "Invalid Linker Options";
    case CL_INVALID_DEVICE_PARTITION_COUNT: return "Invalid Device Partition Count";
    #endif // CL_VERSION_1_2
    #ifdef CL_VERSION_2_0
    case CL_INVALID_PIPE_SIZE: return "Invalid Pipe Size";
    case CL_INVALID_DEVICE_QUEUE: return "Invalid Device Queue";
    #endif // CL_VERSION_2_0
    default:
    {
        static thread_local string_type error_str;
        stringstream ss;

        ss << "Unknown OpenCL Error (" << error << ")";
        return (error_str = ss.str ()).c_str ();
    }
    } //!< switch
}

// =========================================================

// resource_object<resource_type::context>::resource_object (pointer handle) noexcept
// : base_type (handle)
// {
//     ::clRetainContext (handle);
// }

// resource_object<resource_type::context>::~resource_object () noexcept
// {
//     if (handle ()) ::clReleaseContext (handle ());
// }

// resource_object<resource_type::source_code>::resource_object (pointer handle) noexcept
// : base_type (handle)
// {
//     ::clRetainKernel (handle);
// }

// resource_object<resource_type::source_code>::~resource_object () noexcept
// {
//     if (handle ()) ::clReleaseKernel (handle ());
// }

// resource_object<resource_type::program>::resource_object (pointer handle) noexcept
// : base_type (handle)
// {
//     ::clRetainProgram (handle);
// }

// resource_object<resource_type::program>::~resource_object () noexcept
// {
//     if (handle ()) ::clReleaseProgram (handle ());
// }

// resource_object<resource_type::queue>::resource_object (pointer handle) noexcept
// : base_type (handle)
// {
//     ::clRetainCommandQueue (handle);
// }

// resource_object<resource_type::queue>::~resource_object () noexcept
// {
//     if (handle ()) ::clReleaseCommandQueue (handle ());
// }

// resource_object<resource_type::buffer>::resource_object (pointer handle) noexcept
// : base_type (handle)
// {
//     ::clRetainMemObject (handle);
// }

// resource_object<resource_type::buffer>::~resource_object () noexcept
// {
//     if (handle ()) ::clReleaseMemObject (handle ());
// }

// resource_object<resource_type::image>::resource_object (pointer handle) noexcept
// : base_type (handle)
// {
//     ::clRetainMemObject (handle);
// }

// resource_object<resource_type::image>::~resource_object () noexcept
// {
//     if (handle ()) ::clReleaseMemObject (handle ());
// }

// resource_object<resource_type::event>::resource_object (pointer handle) noexcept
// : base_type (handle)
// {
//     ::clRetainEvent (handle);
// }

// resource_object<resource_type::event>::~resource_object () noexcept
// {
//     if (handle ()) ::clReleaseEvent (handle ());
// }

// resource_object<resource_type::sampler>::resource_object (pointer handle) noexcept
// : base_type (handle)
// {
//     ::clRetainSampler (handle);
// }

// resource_object<resource_type::sampler>::~resource_object () noexcept
// {
//     if (handle ()) ::clReleaseSampler(handle ());
// }

// =========================================================

} // namespace cl
