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
 *memory
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_COMPUTE_BACKEND_OPENCL_H_
#define CPPUAL_COMPUTE_BACKEND_OPENCL_H_
#ifdef __cplusplus

#include <cppual/types>
#include <cppual/string>
#include <cppual/concepts>
#include <cppual/resource>
#include <cppual/containers>
#include <cppual/noncopyable>
#include <cppual/compute/object.h>
#include <cppual/compute/device.h>
#include <cppual/compute/backend_iface.h>

#define CL_TARGET_OPENCL_VERSION 200

#include <type_traits>
#include <exception>
#include <stdexcept>

#include <CL/cl.h>
#include <CL/cl_ext.h>

/*
 * API Typesafe Encapsulation Guide Lines
 *
 * API calls should always start with root namespace "::";
 * define all API typedefs in a consistent non-conflicting way;
 * for integer bit fields use enums;
 * for object encapsulation use template specialization,
 * constexpr & constexpr functions as much as possible;
 * if the api requires some initialization then use STL containers + cppual polymorphic allocators;
 * if possible prefer stack allocators (cpu stack memory);
 * define all class typedefs according to STL (ex. typedef T value_type);
 * maintain strict typesafety;
 *
 */

// =========================================================

namespace cppual::compute::cl {

// =========================================================

typedef string                  string_type         ;
typedef ::cl_platform_info      platform_info_type  ;
typedef ::cl_device_info        device_info_type    ;
typedef ::cl_device_type        device_cat_type     ;
typedef ::cl_mem_info           memory_info_type    ;
typedef ::cl_context_info       context_info_type   ;
typedef ::cl_kernel_info        kernel_info_type    ;
typedef ::cl_program_info       program_info_type   ;
typedef ::cl_event_info         event_info_type     ;
typedef ::cl_command_queue_info cmd_queue_info_type ;
typedef ::cl_sampler_info       sampler_info_type   ;
typedef ::cl_image_info         image_info_type     ;

// =========================================================

typedef std::remove_pointer_t<::cl_platform_id  > platform_id_type;
typedef std::remove_pointer_t<::cl_device_id    > device_id_type  ;
typedef std::remove_pointer_t<::cl_event        > event_type      ;
typedef std::remove_pointer_t<::cl_kernel       > kernel_type     ;
typedef std::remove_pointer_t<::cl_program      > program_type    ;
typedef std::remove_pointer_t<::cl_context      > context_type    ;
typedef std::remove_pointer_t<::cl_mem          > memory_type     ;
typedef std::remove_pointer_t<::cl_command_queue> queue_type      ;
typedef std::remove_pointer_t<::cl_sampler      > sampler_type    ;

// =========================================================

template <resource_type> class resource_object;

typedef resource_object<resource_type::instance   > platform_object;
typedef resource_object<resource_type::device     > device_object  ;
typedef resource_object<resource_type::buffer     > buffer_object  ;
typedef resource_object<resource_type::image      > image_object   ;
typedef resource_object<resource_type::context    > context_object ;
typedef resource_object<resource_type::source_code> cmd_object     ;
typedef resource_object<resource_type::program    > cmd_seq_object ;
typedef resource_object<resource_type::queue      > queue_object   ;
typedef resource_object<resource_type::event      > event_object   ;
typedef resource_object<resource_type::sampler    > sampler_object ;

// =========================================================

enum
{
    success             = CL_SUCCESS                         ,
    true_value          = CL_TRUE                            ,
    false_value         = CL_FALSE                           ,

    max_units           = CL_DEVICE_MAX_COMPUTE_UNITS        ,
    max_dimensions      = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS ,
    max_group_size      = CL_DEVICE_MAX_WORK_GROUP_SIZE      ,
    max_item_count      = CL_DEVICE_MAX_WORK_ITEM_SIZES      ,
    global_mem_size     = CL_DEVICE_GLOBAL_MEM_SIZE          ,
    cache_size          = CL_DEVICE_GLOBAL_MEM_CACHE_SIZE    ,
    cache_line_size     = CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
    const_mem_size      = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE ,
    local_mem_size      = CL_DEVICE_LOCAL_MEM_SIZE           ,

    platform_extensions = CL_PLATFORM_EXTENSIONS             ,
    device_extensions   = CL_DEVICE_EXTENSIONS
};

enum class supported_ils : ::cl_device_exec_capabilities
{
    opencl = CL_EXEC_KERNEL,
    native = CL_EXEC_NATIVE_KERNEL
};

typedef bitset<supported_ils> supported_ils_type;

// =========================================================

class platform     ;
class device       ;
class program      ;
class kernel       ;
class event        ;
class context      ;
class image        ;
class memory_chunk ;
class command_queue;
class sampler      ;

// =========================================================
// OpenCL Object Concept
// =========================================================

template <typename> struct is_cl_object_helper           : public std::false_type { };
template <> struct is_cl_object_helper<platform_id_type> : public std::true_type  { };
template <> struct is_cl_object_helper<device_id_type  > : public std::true_type  { };
template <> struct is_cl_object_helper<context_type    > : public std::true_type  { };
template <> struct is_cl_object_helper<kernel_type     > : public std::true_type  { };
template <> struct is_cl_object_helper<program_type    > : public std::true_type  { };
template <> struct is_cl_object_helper<memory_type     > : public std::true_type  { };
template <> struct is_cl_object_helper<event_type      > : public std::true_type  { };
template <> struct is_cl_object_helper<queue_type      > : public std::true_type  { };
template <> struct is_cl_object_helper<sampler_type    > : public std::true_type  { };

template <non_void T>
struct is_cl_object : std::integral_constant<bool, (is_cl_object_helper<remove_cref_t<T>>::value)>
{ static_assert (is_cl_object<T>::value, "T is NOT an opencl object type!"); };

template <non_void T>
inline constexpr cbool is_cl_object_v = is_cl_object<T>::value;

template <typename T>
using CLObject = std::enable_if_t<is_cl_object_v<T>, T>;

template <typename T>
concept cl_object_t = is_cl_object_v<T>;

// =========================================================

template <resource_type = resource_type::instance>
struct conv_type { typedef platform_id_type type; };

template <> struct conv_type<resource_type::device>      { typedef device_type  type; };
template <> struct conv_type<resource_type::event>       { typedef event_type   type; };
template <> struct conv_type<resource_type::source_code> { typedef kernel_type  type; };
template <> struct conv_type<resource_type::program>     { typedef program_type type; };
template <> struct conv_type<resource_type::context>     { typedef context_type type; };
template <> struct conv_type<resource_type::buffer>      { typedef memory_type  type; };
template <> struct conv_type<resource_type::image>       { typedef memory_type  type; };
template <> struct conv_type<resource_type::queue>       { typedef queue_type   type; };

template <resource_type T>
using conv_type_t = conv_type<T>::type;

// =========================================================

class opencl_error : public std::runtime_error
{
public:
    /// Creates a new opencl_error exception object for \p error.
    explicit opencl_error (i32 error) noexcept
    : runtime_error (to_string (error)),
      _M_error      (error)
    {
    }

    /// Destroys the opencl_error object.
    ~opencl_error () noexcept
    {
    }

    /// Returns the numeric error code.
    constexpr i32 error_code () const noexcept
    {
        return _M_error;
    }

    /// Returns a string description of the error.
    cchar* error_string () const noexcept
    {
        return to_string (_M_error);
    }

    /// Returns a C-string description of the error.
    cchar* what () const noexcept
    {
        return to_string (_M_error);
    }

    /// Static function which converts the numeric opencl error code \p error
    /// to a human-readable string.
    ///
    /// For example:
    /// \code
    /// std::cout << opencl_error::to_string(CL_INVALID_KERNEL_ARGS) << std::endl;
    /// \endcode
    ///
    /// Will print "Invalid Kernel Arguments".
    ///
    /// If the error code is unknown (e.g. not a valid opencl error), a string
    /// containing "Unknown opencl Error" along with the error number will be
    /// returned.
    static cchar* to_string (i32 error);

private:
    i32 _M_error;
};

// =========================================================

struct no_platform_found : public std::exception
{
    /// Returns a string containing a human-readable error message.
    cchar* what () const noexcept
    {
        return "NO OpenCL platforms found!";
    }
};

struct no_device_found : public std::exception
{
    /// Returns a string containing a human-readable error message.
    cchar* what () const noexcept
    {
        return "NO OpenCL devices found!";
    }
};

// =========================================================

template <class Function, class Object, class AuxInfo>
struct bound_info_function
{
    bound_info_function(Function function, Object object, AuxInfo aux_info)
    : _M_function(function),
      _M_object  (object  ),
      _M_aux_info(aux_info)
    {
    }

    template <class Info>
    i32 operator()(Info info, size_t input_size, cvoid* input,
                   size_t size, void* value, size_t* size_ret) const
    {
        return _M_function(
            _M_object, _M_aux_info, info,
            input_size, input, size, value, size_ret
        );
    }

    template <class Info>
    i32 operator()(Info info, size_t size, void* value, size_t* size_ret) const
    {
        return _M_function(_M_object, _M_aux_info, info, size, value, size_ret);
    }

    Function _M_function;
    Object   _M_object  ;
    AuxInfo  _M_aux_info;
};

template <class Function, class Object>
struct bound_info_function<Function, Object, void>
{
    bound_info_function(Function function, Object object)
    : _M_function(function),
      _M_object  (object  )
    {
    }

    template<class Info>
    i32 operator()(Info info, size_t size, void *value, size_t* size_ret) const
    {
        return _M_function(_M_object, info, size, value, size_ret);
    }

    Function _M_function;
    Object   _M_object  ;
};

template <class Function>
struct bound_info_function<Function, platform_id_type*, void>
{
    bound_info_function(Function function, platform_id_type* object)
    : _M_function(function),
      _M_object  (object  )
    {
    }

    template <class Info>
    i32 operator () (Info info, u32 size, device_id_type** value, u32* size_ret) const
    {
        return _M_function(_M_object, info, size, value, size_ret);
    }

    template <class Info>
    i32 operator () (Info info, std::size_t size, void* value, std::size_t* size_ret) const
    {
        return _M_function(_M_object, info, size, value, size_ret);
    }

    Function          _M_function;
    platform_id_type* _M_object  ;
};

template <class Function>
struct bound_info_function<Function, void, void>
{
    bound_info_function(Function function)
    : _M_function(function)
    {
    }

    i32 operator () (std::size_t size, void* value, std::size_t* size_ret) const
    {
        return _M_function(size, value, size_ret);
    }

    i32 operator () (u32 size, platform_id_type** value, u32* size_ret) const
    {
        return _M_function(size, value, size_ret);
    }

    Function _M_function;
};

template <class Function>
constexpr bound_info_function<Function, void, void>
bind_info_function(Function f)
{
    return bound_info_function<Function, void, void>(f);
}

template <class Function, class Object>
constexpr bound_info_function<Function, Object, void>
bind_info_function(Function f, Object o)
{
    return bound_info_function<Function, Object, void>(f, o);
}

template <class Function, class Object, class AuxInfo>
constexpr bound_info_function<Function, Object, AuxInfo>
bind_info_function(Function f, Object o, AuxInfo j)
{
    return bound_info_function<Function, Object, AuxInfo>(f, o, j);
}

//! default implementation
template <class T>
struct get_object_info_impl
{
    template<class Function, class Info>
    T operator()(Function function, Info info) const
    {
        T value;

        i32 ret = function(info, sizeof(T), &value, 0);
        if(ret != success) throw opencl_error(ret);

        return value;
    }

    template<class Function, class Info>
    T operator()(Function function, Info info,
                 const size_t input_size, const void* input) const
    {
        T value;

        i32 ret = function(info, input_size, input, sizeof(T), &value, 0);
        if(ret != success) throw opencl_error(ret);

        return value;
    }
};

//! specialization for bool
template<>
struct get_object_info_impl<bool>
{
    template<class Function, class Info>
    bool operator()(Function function, Info info) const
    {
        cl_bool value;

        i32 ret = function(info, sizeof(cl_bool), &value, 0);
        if(ret != success) throw opencl_error(ret);

        return value == CL_TRUE;
    }
};

//! specialization for string_type
template<>
struct get_object_info_impl<string_type>
{
    template <class Function, class Info>
    string_type operator()(Function function, Info info) const
    {
        std::size_t size = 0;

        i32 ret = function(info, 0, nullptr, &size);
        if(ret != success) throw opencl_error(ret);

        if(size == 0) return string_type();

        string_type value(size - 1, 0);

        ret = function(info, size, &value[0], 0);
        if(ret != success) throw opencl_error(ret);

        return value;
    }
};

//! specialization for vector<T>
template <class T>
struct get_object_info_impl< dyn_array<T> >
{
    typedef dyn_array<T> value_type;

    template <class Function>
    value_type operator()(Function function) const
    {
        size_t size = 0;

        i32 ret = function(0, 0, &size);
        if (ret != success) throw opencl_error(ret);

        if(size == 0) return value_type ();

        value_type vector (size / sizeof(T));

        ret = function(size, &vector[0], 0);
        if(ret != success) throw opencl_error(ret);

        return vector;
    }

    template <class Function, class Info>
    value_type operator()(Function function, Info info) const
    {
        u32 size = 0;

        i32 ret = function(info, 0U, 0U, &size);
        if(ret != success) throw opencl_error(ret);

        if(size == 0) return value_type();

        value_type vector(size / sizeof(T));

        ret = function(info, size, &vector[0], 0);
        if(ret != success) throw opencl_error(ret);

        return vector;
    }

    template <class Function, class Info>
    value_type operator()(Function function, Info info,
                         const size_t input_size, cvoid* input) const
    {
        size_t size = 0;

        i32 ret = function(info, input_size, input, 0, 0, &size);
        if(ret != success) throw opencl_error(ret);

        value_type vector(size / sizeof(T));

        ret = function(info, input_size, input, size, &vector[0], 0);
        if(ret != success) throw opencl_error(ret);

        return vector;
    }
};

template <>
struct get_object_info_impl< dyn_array<platform_id_type*> >
{
    typedef dyn_array<platform_id_type*> value_type;

    template <class Function>
    value_type operator()(Function function) const
    {
        u32 size = 0;

        i32 ret = function(0, 0, &size);
        if(ret != success) throw opencl_error(ret);

        if(size == 0) return value_type();

        value_type vector(size / sizeof(platform_id_type*));

        ret = function(size, &vector[0], 0);
        if(ret != success) throw opencl_error(ret);

        return vector;
    }
};

template <>
struct get_object_info_impl< dyn_array<device_type*> >
{
    typedef dyn_array<device_type*> value_type;

    template <class Function, class Info>
    value_type operator()(Function function, Info info) const
    {
        u32 size = 0;

        i32 ret = function(info, 0, 0, &size);
        if(ret != success) throw opencl_error(ret);

        if(size == 0) return value_type();

        value_type vector(size / sizeof(device_type*));

        ret = function(info, size, &vector[0], 0);
        if(ret != success) throw opencl_error(ret);

        return vector;
    }
};

//! returns the value (of type T) from the given clGet*Info() function call.
template <class T, class Function>
constexpr T get_object_info(Function f)
{
    return get_object_info_impl<T>()(bind_info_function(f));
}

template <class T, class Function, class Object, class Info>
constexpr T get_object_info(Function f, Object o, Info i)
{
    return get_object_info_impl<T>()(bind_info_function(f, o), i);
}


template <class T, class Function, class Object, class Info, class AuxInfo>
constexpr T get_object_info(Function f, Object o, Info i, AuxInfo j)
{
    return get_object_info_impl<T>()(bind_info_function(f, o, j), i);
}

template <class T, class Function, class Object, class Info, class AuxInfo>
constexpr T get_object_info(Function f, Object o, Info i, AuxInfo j, const size_t k, cvoid* l)
{
    return get_object_info_impl<T>()(bind_info_function(f, o, j), i, k, l);
}

//! ================================================================================
//! simplified implementation for managing opencl handles & a polymorphic interface
//! ================================================================================

template <resource_type T>
class resource_object : public object<T>
{
public:
    typedef resource_object<T>       self_type  ;
    typedef object<T>                base_type  ;
    typedef CLObject<conv_type_t<T>> value_type ;
    typedef value_type*              pointer    ;
    typedef std::size_t              size_type  ;
    typedef resource_handle          handle_type;

    resource_object  () = delete ;
    ~resource_object () = default;

    constexpr resource_object (self_type&&) = default;
    constexpr self_type& operator = (self_type&&) = default;

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer> ();
    }

protected:
    constexpr explicit resource_object (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

// template <>
// class resource_object<resource_type::context> : public context_interface
// {
// public:
//     typedef resource_object<resource_type::context> self_type  ;
//     typedef context_interface                       base_type  ;
//     typedef CLObject<context_type>                  value_type ;
//     typedef value_type*                             pointer    ;
//     typedef std::size_t                             size_type  ;
//     typedef resource_handle                         handle_type;

//     resource_object  () = delete;
//     ~resource_object () noexcept;

//     constexpr resource_object (self_type&&) = default;
//     constexpr self_type& operator = (self_type&&) = default;

//     constexpr pointer handle () const noexcept
//     {
//         return base_type::handle<pointer> ();
//     }

// protected:
//     explicit resource_object (pointer handle) noexcept;
// };

// =========================================================

} // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_OPENCL_H_
