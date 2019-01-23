/*
 * Copyright (c) 2017-2018, MVP CORPORATION. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of MVP CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __MVP_IOCTL__H__
#define __MVP_IOCTL__H__

#include <linux/ioctl.h>
#include <linux/types.h>
#include <stdint.h>
#include "mvp_event.h"
#include "command_queue.h"

#if !defined(__KERNEL__)
#define __user
#endif

typedef struct attributes{
	size_t max_work_group_size;
    size_t max_work_item_sizes[3];
    size_t al_mem_size;
    size_t max_compute_units;
    size_t error_correction_support;
    size_t host_unified_memory;
    size_t max_constant_buffer_size;
    size_t max_clock_frequency;
    size_t mem_base_addr_align;
	/* Get GPU architecture name */
	int sm_maj; 
	int sm_min;
	size_t memfree; 
	size_t memtotal;
}mvp_attributes_t;

typedef struct ioctl_args{
	__u32 size;
    __u32 handle;
}mvp_alloc_args_s;


struct pocl_context {
  size_t work_dim;
  size_t num_groups[3];
  size_t group_id[3];
  size_t global_offset[3];
  size_t local_size[3];
  char *printf_buffer;
  size_t *printf_buffer_position;
  size_t printf_buffer_capacity;
};

typedef void (*workgroup) (void **, struct pocl_context *);
typedef workgroup mvp_kernel_func_t;

typedef struct{
    size_t num_groups_x;
    size_t num_groups_y;
    size_t num_groups_z;
    size_t local_x;
    size_t local_y;
    size_t local_z;
    mvp_event_t event;
    mvp_kernel_func_t kernel_func;
    void ** kernel_args;
    int args_num;
}launch_kernel_args_t;


typedef struct {
    size_t    type;
    size_t num_groups_x;
    size_t num_groups_y;
    size_t num_groups_z;
    size_t local_x;
    size_t local_y;
    size_t local_z;
    mvp_event_t event;
    mvp_kernel_func_t kernel_func;
    void ** kernel_args;
    int kernel_args_num;
    struct pocl_context pc;
}mvp_cmd_launch_kernel_t;

typedef struct map_addr{
	void *in_map_addr;
	void *out_map_addr;
}mvp_map_addr;

typedef struct{
	unsigned int event_id;
	unsigned int event_status;
}event_info;

typedef struct get_dev_paddr{
	void* handle;
	void* paddr;
}get_dev_paddr_t;


//just for debug tools
typedef struct dump_reg_mem{
  unsigned int start_addr;
  unsigned int size;
  void* user_addr;
  unsigned char type; //0 for mem dump , 1 for reg dump
}dump_reg_mem_t;


#define DRM_MVP_GET_DEVICE_COUNT		    0x00
#define DRM_MVP_GET_DEVICE_ATTRIBUTES		0x01
#define DRM_MVP_ALLOC_DEV_MEM       		0x02
#define DRM_MVP_FREE_DEV_MEM        		0x03
#define DRM_MVP_COPY_HOST_TO_DEV            0x04
#define DRM_MVP_COPY_DEV_TO_HOST            0x05
#define DRM_MVP_COPY_DEV_TO_DEV             0x06
#define DRM_MVP_CREATE_EVENT                0x07
#define DRM_MVP_WAIT_EVENT_DONE             0x08
#define DRM_MVP_EVENT_DESTROY               0x09
#define DRM_MVP_LAUNCH_KERNEL               0x0a
#define DRM_MVP_MAP_DEVICE_ADDRESS          0x0b
#define DRM_MVP_GET_DEVICE_PADDR            0x0c
#define DRM_MVP_DUMP_REG_MEM                0x1d
#define DRM_MVP_EVENT_GET_STATUS            0x1e
#define DRM_MVP_EVENT_SET_STATUS            0x1f

#define DRM_IOCTL_MVP_GET_DEVICE_COUNT       DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_GET_DEVICE_COUNT, int)
#define DRM_IOCTL_MVP_GET_DEVICE_ATTRIBUTES  DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_GET_DEVICE_ATTRIBUTES, mvp_attributes_t)
#define DRM_IOCTL_MVP_ALLOC_DEV_MEM          DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_ALLOC_DEV_MEM, mvp_alloc_args_s)
#define DRM_IOCTL_MVP_FREE_DEV_MEM           DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_FREE_DEV_MEM, int)
#define DRM_IOCTL_MVP_COPY_HOST_TO_DEV       DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_COPY_HOST_TO_DEV, mem_copy_args_t)
#define DRM_IOCTL_MVP_COPY_DEV_TO_HOST       DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_COPY_DEV_TO_HOST, mem_copy_args_t)
#define DRM_IOCTL_MVP_COPY_DEV_TO_DEV        DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_COPY_DEV_TO_DEV, mem_copy_args_t)
#define DRM_IOCTL_MVP_CREATE_EVENT           DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_CREATE_EVENT, unsigned int)
#define DRM_IOCTL_MVP_WAIT_EVENT_DONE        DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_WAIT_EVENT_DONE, unsigned int)
#define DRM_IOCTL_MVP_EVENT_DESTROY          DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_EVENT_DESTROY, unsigned int)
#define DRM_IOCTL_MVP_LAUNCH_KERNEL          DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_LAUNCH_KERNEL, launch_kernel_args_t)
#define DRM_IOCTL_MVP_MAP_DEVICE_ADDRESS     DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_MAP_DEVICE_ADDRESS, mvp_map_addr)
#define DRM_IOCTL_MVP_GET_DEVICE_PADDR       DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_GET_DEVICE_PADDR, get_dev_paddr_t)
#define DRM_IOCTL_MVP_DUMP_REG_MEM           DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_DUMP_REG_MEM, dump_reg_mem_t)
#define DRM_IOCTL_MVP_GET_EVENT_STATUS       DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_EVENT_GET_STATUS, event_info)
#define DRM_IOCTL_MVP_SET_EVENT_STATUS       DRM_IOWR(DRM_COMMAND_BASE + DRM_MVP_EVENT_SET_STATUS, event_info)
#endif

