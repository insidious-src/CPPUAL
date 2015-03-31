/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#ifndef KHRONOS_VULKAN_H_
#define KHRONOS_VULKAN_H_

#ifdef __cplusplus
#	include <cstdlib>
#	include <cstdint>
extern "C" {
#else
#	include <stdint.h>
#	include <stdlib.h>
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#	define VK_API_CALL __stdcall
#	define VK_CALLBACK __stdcall
#else
#	define VK_API_CALL
#	define VK_CALLBACK
#endif

struct  _VK_INSTANCE;
struct  _VK_DEVICE;
typedef _VK_INSTANCE*  VK_INSTANCE;
typedef _VK_DEVICE*    VK_DEVICE;
typedef uint32_t       VK_PHYSICAL_GPU;
typedef void*          VK_OBJECT;
typedef VK_OBJECT      VK_GPU_MEMORY;
typedef VK_OBJECT      VK_QUEUE;
typedef VK_OBJECT      VK_CMD_BUFFER;
typedef VK_OBJECT      VK_SHADER;
typedef VK_OBJECT      VK_PIPELINE;
typedef VK_OBJECT      VK_DYNAMIC_VP_STATE_OBJECT;
typedef VK_OBJECT      VK_DYNAMIC_DS_STATE;
typedef VK_OBJECT      VK_IMAGE;
typedef VK_OBJECT      VK_BUFFER;
typedef VK_OBJECT      VK_RENDER_PASS;
typedef VK_OBJECT      VK_EVENT;
typedef size_t         VK_SIZE;
typedef int            VK_RESULT;
typedef unsigned       VK_UINT;
typedef unsigned short VK_ENUM;
typedef char           VK_CHAR;
typedef uint32_t       VK_UINT32;

typedef void* (VK_CALLBACK* VK_ALLOC_FUNCTION)(VK_SIZE size, VK_SIZE alignment, VK_ENUM allocType);
typedef void  (VK_CALLBACK* VK_FREE_FUNCTION )(void* pMem);

enum VK_DEVICE_TYPE
{
	VK_DEVICE_GPU         = 1 << 0,
	VK_DEVICE_CPU         = 1 << 1,
	VK_DEVICE_ACCELERATOR = 1 << 2,
	VK_DEVICE_CUSTOM      = 1 << 3
};

enum VK_QUEUE_TYPE
{
	VK_QUEUE_COMPUTE   = 1 << 0,
	VK_QUEUE_GRAPHICS  = 1 << 1,
	VK_QUEUE_DMA       = 1 << 2,
	VK_QUEUE_UNIVERSAL = 1 << 3
};

enum
{
	VK_INFO_TYPE_IMAGE_MEMORY_REQUIREMENTS,
	VK_GPU_INFO_WHATEVER,
	VK_PIPELINE_BIND_POINT_GRAPHICS
};

struct VK_APPLICATION_INFO
{
	VK_CHAR const* pAppName;
	VK_UINT32      appVersion;
	VK_CHAR const* pEngineName;
	VK_UINT32      engineVersion;
	VK_UINT32      apiVersion;
};

struct VK_ALLOC_CALLBACKS
{
	VK_ALLOC_FUNCTION pfnAlloc;
	VK_FREE_FUNCTION  pfnFree;
};


struct VK_SOME_GPU_INFO_STRUCTURE
{
};

struct VK_GPU_COMPATIBILITY_INFO
{
};

struct VK_DEVICE_CREATE_INFO
{
};

struct VK_CMD_BUFFER_CREATE_INFO
{
};

struct VK_CMD_BUFFER_BEGIN_INFO
{
};

struct VK_SHADER_CREATE_INFO
{
};

struct VK_GRAPHICS_PIPELINE_CREATE_INFO
{
};

struct VK_DYNAMIC_VP_STATE_CREATE_INFO
{
};

struct VK_DYNAMIC_DS_STATE_CREATE_INFO
{
};

struct VK_IMAGE_CREATE_INFO
{
};

struct VK_BUFFER_CREATE_INFO
{
};

struct VK_IMAGE_MEMORY_REQUIREMENTS
{
};

struct VK_MEMORY_ALLOC_INFO
{
	VK_SIZE size;
	VK_SIZE alignment;
	VK_ENUM flags;
	VK_UINT heapCount;
	VK_UINT heaps[5];
	VK_ENUM memPriority;
};

struct VK_RENDER_PASS_CREATE_INFO
{
};

struct VK_RENDER_PASS_BEGIN
{
	VK_RENDER_PASS pass;
};

struct VK_EVENT_CREATE_INFO
{
};

struct VK_IMAGE_MEMORY_BARRIER
{
};

struct VK_PIPELINE_BARRIER
{
	VK_UINT32                num;
	VK_IMAGE_MEMORY_BARRIER* barrier;
};

struct VK_MEMORY_OPEN_INFO
{
};

// instance and device management
extern bool      VK_API_CALL vkCreateInstance  (VK_APPLICATION_INFO const*, VK_ALLOC_CALLBACKS const*, VK_INSTANCE*);
extern VK_RESULT VK_API_CALL vkCreateDevice    (uint32_t, VK_DEVICE_CREATE_INFO const*, VK_DEVICE*);
extern void      VK_API_CALL vkDestroyDevice   (VK_DEVICE);
extern void      VK_API_CALL vkDestroyInstance (VK_INSTANCE);

// gpu management
extern VK_RESULT VK_API_CALL vkEnumerateGpus (VK_INSTANCE, size_t max_num, size_t* num, VK_PHYSICAL_GPU* gpus);
extern VK_RESULT VK_API_CALL vkGetGpuInfo    (uint32_t gpu_id, uint16_t type, size_t** sizes, VK_SOME_GPU_INFO_STRUCTURE** infos);
extern VK_RESULT VK_API_CALL vkGetMultiGpuCompatibility (uint32_t, uint32_t, VK_GPU_COMPATIBILITY_INFO*);

// command execution
extern VK_RESULT VK_API_CALL vkCreateCommandBuffer (VK_DEVICE, VK_CMD_BUFFER_CREATE_INFO const*, VK_CMD_BUFFER*);
extern VK_RESULT VK_API_CALL vkBeginCommandBuffer  (VK_CMD_BUFFER, VK_CMD_BUFFER_BEGIN_INFO*);
extern VK_RESULT VK_API_CALL vkEndCommandBuffer    (VK_CMD_BUFFER);
extern VK_RESULT VK_API_CALL vkCmdEnqueue          (VK_CMD_BUFFER, ...);

// shader execution
extern VK_RESULT VK_API_CALL vkCreateShader (VK_DEVICE, VK_SHADER_CREATE_INFO*, VK_SHADER*);

// queue management
extern VK_RESULT VK_API_CALL vkGetDeviceQueue          (VK_DEVICE, uint32_t, uint32_t, VK_QUEUE*);
extern VK_RESULT VK_API_CALL vkQueueSubmit             (VK_QUEUE, size_t num, VK_CMD_BUFFER* buffers, VK_OBJECT fence);
extern VK_RESULT VK_API_CALL vkQueueAddMemReference    (VK_QUEUE, VK_OBJECT mem);
extern VK_RESULT VK_API_CALL vkQueueRemoveMemReference (VK_QUEUE, VK_OBJECT mem);
extern VK_RESULT VK_API_CALL vkQueueSignalSemaphore    (VK_QUEUE, VK_OBJECT semaphore);
extern VK_RESULT VK_API_CALL vkQueueWaitSemaphore      (VK_QUEUE, VK_OBJECT semaphore);

// pipeline management
extern VK_RESULT VK_API_CALL vkCreateGraphicsPipeline (VK_DEVICE, VK_GRAPHICS_PIPELINE_CREATE_INFO*, VK_PIPELINE*);
extern VK_RESULT VK_API_CALL vkStorePipeline          (VK_PIPELINE, size_t* sizes, VK_OBJECT data);
extern VK_RESULT VK_API_CALL vkLoadPipeline           (VK_DEVICE, size_t size, VK_OBJECT data, VK_PIPELINE*);
extern VK_RESULT VK_API_CALL vkCmdPipelineBarrier     (VK_CMD_BUFFER, VK_PIPELINE_BARRIER*);

// surface depth management
extern VK_RESULT VK_API_CALL vkCreateDynamicViewportState (VK_DEVICE, VK_DYNAMIC_VP_STATE_CREATE_INFO*, VK_DYNAMIC_VP_STATE_OBJECT*);
extern VK_RESULT VK_API_CALL vkCreateDynamicDepthStencilState (VK_DEVICE, VK_DYNAMIC_DS_STATE_CREATE_INFO*, VK_DYNAMIC_DS_STATE*);

// resource management
extern VK_RESULT  VK_API_CALL vkCreateImage   (VK_DEVICE, VK_IMAGE_CREATE_INFO*, VK_IMAGE*);
extern VK_RESULT  VK_API_CALL vkCreateBuffer  (VK_DEVICE, VK_BUFFER_CREATE_INFO*, VK_BUFFER*);
extern void       VK_API_CALL vkDestroyObject (VK_OBJECT);

// memory management
extern VK_RESULT VK_API_CALL vkGetObjectInfo    (VK_IMAGE, uint16_t type, size_t sizes, VK_IMAGE_MEMORY_REQUIREMENTS*);
extern VK_RESULT VK_API_CALL vkAllocMemory      (VK_DEVICE, VK_MEMORY_ALLOC_INFO const*, VK_GPU_MEMORY*);
extern VK_RESULT VK_API_CALL vkFreeMemory       (VK_GPU_MEMORY);
extern VK_RESULT VK_API_CALL vkBindObjectMemory (VK_IMAGE, uint32_t, VK_OBJECT, uint32_t);
extern VK_RESULT VK_API_CALL vkOpenSharedMemory (VK_DEVICE device, VK_MEMORY_OPEN_INFO const* pOpenInfo, VK_GPU_MEMORY* pMem);

// pools
extern VK_RESULT VK_API_CALL vkCreateDescriptorPool           (...);
extern VK_RESULT VK_API_CALL vkCreateDescriptorSetLayoutChain (...);
extern VK_RESULT VK_API_CALL vkCreateDescriptorSetLayout      (...);
extern VK_RESULT VK_API_CALL vkAllocDescriptorSets            (...);

// render passes
extern VK_RESULT VK_API_CALL vkCreateRenderPass      (VK_DEVICE, VK_RENDER_PASS_CREATE_INFO*, VK_RENDER_PASS*);
extern VK_RESULT VK_API_CALL vkCmdBeginRenderPass    (VK_CMD_BUFFER, VK_RENDER_PASS_BEGIN*);
extern VK_RESULT VK_API_CALL vkCmdBindPipeline       (VK_CMD_BUFFER, uint16_t type, VK_PIPELINE);
extern VK_RESULT VK_API_CALL vkCmdBindDescriptorSets (VK_CMD_BUFFER, ...);
extern VK_RESULT VK_API_CALL vkCmdDraw               (VK_CMD_BUFFER, uint32_t, uint32_t, uint32_t, uint32_t);
extern VK_RESULT VK_API_CALL vkCmdEndRenderPass      (VK_CMD_BUFFER, VK_RENDER_PASS);

// events & synchronization
extern VK_RESULT VK_API_CALL vkCreateEvent    (VK_DEVICE, VK_EVENT_CREATE_INFO*, VK_EVENT*);
extern VK_RESULT VK_API_CALL vkSetEvent       (VK_EVENT, ...);
extern VK_RESULT VK_API_CALL vkResetEvent     (VK_EVENT, ...);
extern VK_RESULT VK_API_CALL vkGetEventStatus (VK_EVENT, ...);
extern VK_RESULT VK_API_CALL vkCmdSetEvent    (VK_EVENT, ...);
extern VK_RESULT VK_API_CALL vkCmdResetEvent  (VK_EVENT, ...);
extern VK_RESULT VK_API_CALL vkCmdWaitEvents  (...);

#ifdef __cplusplus
}
#endif
#endif // KHRONOS_VULKAN_H_
