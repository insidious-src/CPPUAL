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

#ifndef CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
#define CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
#ifdef __cplusplus

#include <cppual/types>
#include <cppual/plugin>
#include <cppual/bitflags>
#include <cppual/containers>
#include <cppual/noncopyable>
#include <cppual/compute/object.h>

#include <memory>

// =========================================================

namespace cppual::compute {

// =========================================================

typedef enum device_type
{
    none        =      0,
    cpu         = 1 << 0,
    gpu         = 1 << 1,
    gpgpu       = 1 << 2,
    specialized = 1 << 3,
    any         = cpu | gpu | gpgpu | specialized
}
const const_device;

typedef enum class backend_type : u8
{
    native,
    vulkan,
    opencl,
    d3d12 ,
    metal
}
const const_backend;

typedef enum class il_type : u8
{
    none   =      0,
    native = 1 << 0,
    opencl = 1 << 1,
    glsl   = 1 << 2,
    spir   = 1 << 3,
    spirv  = 1 << 4,
    hlil   = 1 << 5
}
const const_il;

typedef enum class memory_cat : u8
{
    restricted ,
    specialized,
    global
}
const const_memory_cat;

typedef enum class memory_access : u8
{
    read_only ,
    write_only,
    read_write
}
const const_memory_access;

typedef bitset<device_type> device_types;
typedef bitset<il_type>     device_ils  ;

// =========================================================

class factory                  ;
class device_interface         ;
class context_interface        ;
class memory_interface         ;
class cmd_seq_interface        ;
class image_interface          ;
class pipeline_interface       ;
class render_pass_interface    ;
class shader_interface         ;
class descriptor_pool_interface;
class event_interface          ;
class state_interface          ;
class queue_interface          ;
class sampler_interface        ;

// =========================================================

typedef std::shared_ptr<factory>                   shared_factory        ;
typedef std::shared_ptr<device_interface>          shared_device         ;
typedef std::shared_ptr<context_interface>         shared_context        ;
typedef std::shared_ptr<memory_interface>          shared_memory         ;
typedef std::shared_ptr<cmd_seq_interface>         shared_cmd_sequence   ;
typedef std::shared_ptr<image_interface>           shared_image          ;
typedef std::shared_ptr<pipeline_interface>        shared_pipeline       ;
typedef std::shared_ptr<render_pass_interface>     shared_render_pass    ;
typedef std::shared_ptr<shader_interface>          shared_shader         ;
typedef std::shared_ptr<descriptor_pool_interface> shared_descriptor_pool;
typedef std::shared_ptr<event_interface>           shared_event          ;
typedef std::shared_ptr<state_interface>           shared_state          ;
typedef std::shared_ptr<queue_interface>           shared_queue          ;
typedef std::shared_ptr<sampler_interface>         shared_sampler        ;

// =========================================================

class SHARED_API factory : public non_copyable_virtual
{
public:
    typedef std::size_t                      size_type    ;
    typedef dyn_array<shared_device>         device_vector;
    typedef process::plugin_manager<factory> manager_type ;

    virtual device_vector          get_devices (device_types type = device_type::any) = 0;
    virtual shared_context         create_context (device_vector const& devs) = 0;

    virtual shared_memory          allocate_memory (shared_context const& context,
                                                    size_type size,
                                                    memory_access = memory_access::read_write,
                                                    memory_cat    = memory_cat::global) = 0;
    \
    virtual shared_cmd_sequence    create_cmd_sequence () = 0;
    virtual shared_image           create_image () = 0;
    virtual shared_pipeline        create_pipeline () = 0;
    virtual shared_render_pass     create_render_pass () = 0;
    virtual shared_shader          create_shader () = 0;
    virtual shared_descriptor_pool create_descriptor_pool () = 0;
    virtual shared_event           create_event () = 0;
    virtual shared_state           create_state () = 0;
    virtual shared_queue           create_queue () = 0;
    virtual shared_sampler         create_sampler () = 0;
    virtual size_type              device_count (device_types type = device_type::any) = 0;
};

// =========================================================

class SHARED_API factories : public non_copyable_virtual
{
public:
    typedef shared_factory        value_type                ;
    typedef dyn_array<value_type> vector_type               ;
    typedef vector_type&          vector_reference          ;
    typedef vector_type const&    vector_const_reference    ;
    typedef std::array<cchar*, 2> lib_vector                ;
    typedef lib_vector&           lib_vector_reference      ;
    typedef lib_vector const&     lib_vector_const_reference;
    typedef std::size_t           size_type                 ;

    factories (lib_vector_const_reference);

    constexpr size_type size () const noexcept
    {
        return _M_factory_vec.size ();
    }

    constexpr vector_reference get_factories () noexcept
    {
        return _M_factory_vec;
    }

public:
    static vector_reference instances          ();
    static bool             has_valid_instance () noexcept;

private:
    vector_type _M_factory_vec;
};

// =========================================================

class SHARED_API device_interface : public object<resource_type::device>
{
public:
    typedef device_interface              self_type   ;
    typedef object<resource_type::device> base_type   ;
    typedef resource_version              version_type;

    enum class profile_type : u8
    {
        full,
        embedded
    };

    virtual string_type     name                  () const = 0;
    virtual string_type     vendor                () const = 0;
    virtual profile_type    profile               () const = 0;
    virtual backend_type    backend               () const = 0;
    virtual device_ils      supported_ils         () const = 0;
    virtual device_type     dev_type              () const = 0;
    virtual version_type    version               () const = 0;
    virtual size_type       cache_size            () const = 0;
    virtual size_type       cache_line_size       () const = 0;
    virtual size_type       local_memory_size     () const = 0;
    virtual size_type       const_memory_size     () const = 0;
    virtual size_type       global_memory_size    () const = 0;
    virtual size_type       max_memory_alloc_size () const = 0;
    virtual u32             compute_units_count   () const = 0;

protected:
    using object<resource_type::device>::object;
};

// =========================================================

class SHARED_API context_interface : public object<resource_type::context>
{
protected:
    using object<resource_type::context>::object;
};

// =========================================================

class SHARED_API memory_interface : public object<resource_type::buffer>
{
public:
    typedef enum class address_space : u8
    {
        global_memory ,
        local_memory  ,
        private_memory,
        constant_memory
    }
    const const_address_space;

    typedef enum class memory_flag : u16
    {
        read_write      = 1 << 0,
        read_only       = 1 << 1,
        write_only      = 1 << 2,
        use_host_ptr    = 1 << 3,
        alloc_host_ptr  = 1 << 4,
        copy_host_ptr   = 1 << 5,
        host_write_only = 1 << 6,
        host_read_only  = 1 << 7,
        host_no_access  = 1 << 8
    }
    const const_memory_flag;

    typedef bitset<memory_flag> memory_flags;

protected:
    using object<resource_type::buffer>::object;
};

// =========================================================

class SHARED_API cmd_seq_interface : public object<resource_type::program>
{
protected:
    using object<resource_type::program>::object;
};

// =========================================================

class SHARED_API image_interface : public object<resource_type::image>
{
protected:
    using object<resource_type::image>::object;
};

// =========================================================

class SHARED_API pipeline_interface : public object<resource_type::pipeline>
{
protected:
    using object<resource_type::pipeline>::object;
};

// =========================================================

class SHARED_API render_pass_interface : public object<resource_type::render_pass>
{
protected:
    using object<resource_type::render_pass>::object;
};

// =========================================================

class SHARED_API shader_interface : public object<resource_type::source_code>
{
protected:
    using object<resource_type::source_code>::object;
};

// =========================================================

class SHARED_API descriptor_pool_interface : public object<resource_type::descriptor_pool>
{
protected:
    using object<resource_type::descriptor_pool>::object;
};

// =========================================================

class SHARED_API event_interface : public object<resource_type::event>
{
protected:
    using object<resource_type::event>::object;
};

// =========================================================

class SHARED_API state_interface : public object<resource_type::state>
{
protected:
    using object<resource_type::state>::object;
};

// =========================================================

class SHARED_API queue_interface : public object<resource_type::queue>
{
protected:
    using object<resource_type::queue>::object;
};

// =========================================================

class SHARED_API sampler_interface : public object<resource_type::sampler>
{
protected:
    using object<resource_type::sampler>::object;
};

// =========================================================

} // namespace compute

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
