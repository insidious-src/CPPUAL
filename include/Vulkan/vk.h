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
#include <cstdlib>
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#include <stdlib.h>
#endif

struct  _VK_INSTANCE;
struct  _VK_DEVICE;
typedef _VK_INSTANCE* VK_INSTANCE;
typedef _VK_DEVICE*   VK_DEVICE;
typedef uint32_t      VK_PHYSICAL_GPU;
typedef void*         VK_OBJECT;
typedef VK_OBJECT     VK_GPU_MEMORY;
typedef VK_OBJECT     VK_QUEUE;
typedef VK_OBJECT     VK_CMD_BUFFER;
typedef VK_OBJECT     VK_SHADER;
typedef VK_OBJECT     VK_PIPELINE;
typedef VK_OBJECT     VK_DYNAMIC_VP_STATE_OBJECT;
typedef VK_OBJECT     VK_DYNAMIC_DS_STATE;
typedef VK_OBJECT     VK_IMAGE;
typedef VK_OBJECT     VK_BUFFER;
typedef VK_OBJECT     VK_RENDER_PASS;
typedef VK_OBJECT     VK_EVENT;

enum VK_DEVICE_TYPE
{
	VK_DEVICE_TYPE_GPU         = 1 << 0,
	VK_DEVICE_TYPE_CPU         = 1 << 1,
	VK_DEVICE_TYPE_ACCELERATOR = 1 << 2,
	VK_DEVICE_TYPE_CUSTOM      = 1 << 3
};

enum
{
	VK_INFO_TYPE_IMAGE_MEMORY_REQUIREMENTS,
	VK_GPU_INFO_WHATEVER,
	VK_PIPELINE_BIND_POINT_GRAPHICS
};

struct VK_APPLICATION_INFO
{
};

struct VK_ALLOC_CALLBACKS
{
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

struct VK_BUFFER_CREATE_INFO
{
};

struct VK_IMAGE_MEMORY_REQUIREMENTS
{
};

struct VK_MEMORY_ALLOC_INFO
{
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
	uint32_t                 num;
	VK_IMAGE_MEMORY_BARRIER* barrier;
};

// instance and device management
bool vkCreateInstance  (VK_APPLICATION_INFO*, VK_ALLOC_CALLBACKS*, VK_INSTANCE*);
int  vkCreateDevice    (uint32_t, VK_DEVICE_CREATE_INFO*, VK_DEVICE*);
void vkDestroyDevice   (VK_DEVICE);
void vkDestroyInstance (VK_INSTANCE);

// gpu management
int  vkEnumerateGpus (VK_INSTANCE, size_t max_num, size_t* num, VK_PHYSICAL_GPU* gpus);
int  vkGetGpuInfo    (uint32_t gpu_id, uint16_t type, size_t** sizes, VK_SOME_GPU_INFO_STRUCTURE** infos);

// command execution
int vkGetMultiGpuCompatibility (uint32_t, uint32_t, VK_GPU_COMPATIBILITY_INFO*);
int vkCreateCommandBuffer      (VK_DEVICE, VK_CMD_BUFFER_CREATE_INFO*, VK_CMD_BUFFER*);
int vkBeginCommandBuffer       (VK_CMD_BUFFER, VK_CMD_BUFFER_BEGIN_INFO*);
int vkEndCommandBuffer         (VK_CMD_BUFFER);
int vkCmdEnqueue               (VK_CMD_BUFFER, ...);

// shader exection
int vkCreateShader (VK_DEVICE, VK_SHADER_CREATE_INFO*, VK_SHADER*);

// queue management
int vkGetDeviceQueue          (VK_DEVICE, uint32_t, uint32_t, VK_QUEUE*);
int vkQueueSubmit             (VK_QUEUE, size_t num, VK_CMD_BUFFER* buffers, VK_OBJECT fence);
int vkQueueAddMemReference    (VK_QUEUE, VK_OBJECT mem);
int vkQueueRemoveMemReference (VK_QUEUE, VK_OBJECT mem);
int vkQueueSignalSemaphore    (VK_QUEUE, VK_OBJECT semaphore);
int vkQueueWaitSemaphore      (VK_QUEUE, VK_OBJECT semaphore);

// pipeline management
int vkCreateGraphicsPipeline (VK_DEVICE, VK_GRAPHICS_PIPELINE_CREATE_INFO*, VK_PIPELINE*);
int vkStorePipeline          (VK_PIPELINE, size_t* sizes, VK_OBJECT data);
int vkLoadPipeline           (VK_DEVICE, size_t size, VK_OBJECT data, VK_PIPELINE*);
int vkCmdPipelineBarrier     (VK_CMD_BUFFER, VK_PIPELINE_BARRIER*);

// surface depth management
int vkCreateDynamicViewportState (VK_DEVICE, VK_DYNAMIC_VP_STATE_CREATE_INFO*, VK_DYNAMIC_VP_STATE_OBJECT*);
int vkCreateDynamicDepthStencilState (VK_DEVICE, VK_DYNAMIC_DS_STATE_CREATE_INFO*, VK_DYNAMIC_DS_STATE*);

// resource management
int  vkCreateImage   (VK_DEVICE, VK_IMAGE_CREATE_INFO*, VK_IMAGE*);
int  vkCreateBuffer  (VK_DEVICE, VK_BUFFER_CREATE_INFO*, VK_BUFFER*);
void vkDestroyObject (VK_OBJECT);

// memory management
int vkGetObjectInfo    (VK_IMAGE, uint16_t type, size_t sizes, VK_IMAGE_MEMORY_REQUIREMENTS*);
int vkAllocMemory      (VK_DEVICE, VK_MEMORY_ALLOC_INFO*, VK_OBJECT*);
int vkBindObjectMemory (VK_IMAGE, uint32_t, VK_OBJECT, uint32_t);

// pools
int vkCreateDescriptorPool           (...);
int vkCreateDescriptorSetLayoutChain (...);
int vkCreateDescriptorSetLayout      (...);
int vkAllocDescriptorSets            (...);

// render passes
int vkCreateRenderPass      (VK_DEVICE, VK_RENDER_PASS_CREATE_INFO*, VK_RENDER_PASS*);
int vkCmdBeginRenderPass    (VK_CMD_BUFFER, VK_RENDER_PASS_BEGIN*);
int vkCmdBindPipeline       (VK_CMD_BUFFER, uint16_t type, VK_PIPELINE);
int vkCmdBindDescriptorSets (VK_CMD_BUFFER, ...);
int vkCmdDraw               (VK_CMD_BUFFER, uint32_t, uint32_t, uint32_t, uint32_t);
int vkCmdEndRenderPass      (VK_CMD_BUFFER, VK_RENDER_PASS);

// events & synchronization
int vkCreateEvent    (VK_DEVICE, VK_EVENT_CREATE_INFO*, VK_EVENT*);
int vkSetEvent       (VK_EVENT, ...);
int vkResetEvent     (VK_EVENT, ...);
int vkGetEventStatus (VK_EVENT, ...);
int vkCmdSetEvent    (VK_EVENT, ...);
int vkCmdResetEvent  (VK_EVENT, ...);
int vkCmdWaitEvents  (...);

#ifdef __cplusplus
}
#endif
#endif // KHRONOS_VULKAN_H_
