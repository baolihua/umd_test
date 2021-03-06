/*
 * command_queue.h
 *
 *  Created on: Oct 30, 2018
 *      Author: jiqianxiang
 */

#ifndef MVP_DRIVERS_COMMAND_QUEUE_H_
#define MVP_DRIVERS_COMMAND_QUEUE_H_

#include "mvp_event.h"
#include "mvp_ioctl.h"

typedef enum{
    CMD_GET_DEVICE = 0,
    CMD_GET_DEVICE_COUNT = 1,
    CMD_GET_DEVICE_ATTIBUTES = 2,
    CMD_ALLOC_DEV_MEM = 3,
    CMD_CREATE_EVENT = 4,
    CMD_COPY_HOST_TO_DEV = 5,
    CMD_COPY_DEV_TO_HOST = 6,
    CMD_COPY_DEV_TO_DEV = 7,
    CMD_FREE_DEV_MEM = 8,
    CMD_WAIT_EVENT_DONE = 9,
    CMD_EVENT_DESTORY = 10,
    CMD_LAUNCH_KERNEL = 11,
    CMD_MAP_DEVICE_ADDRESS = 12,
}cmd_type;

typedef void*  mvp_device_ptr_t;

typedef enum{
    USER_SYNC = 0,
    USER_ASYNC = 1,
}user_sync_t;


typedef enum kernel_event_status_s{
    K_EVENT_STATUS_PENDING = 1,
    K_EVENT_STATUS_RUNNIG = 2,
    K_EVENT_STATUS_FINISH = 3,
}kernel_event_status_t;

typedef struct kernel_event {
	//struct mutex _mutex;
	//pthread_cond_t _cond;
    kernel_event_status_t event_status;
} kernel_event_t;



typedef void* mvp_address_t;

typedef enum mvp_user_event_status_s{
    MVP_USER_EVENT_STATUS_PENDING = 0,
    MVP_USER_EVENT_STATUS_RUNNIG = 1,
    MVP_USER_EVENT_STATUS_FINISH = 2,
}mvp_user_event_status_t;

typedef enum mvp_user_event_type_s{
    MVP_USER_EVENT_DEFAULT = 0,
}mvp_user_event_type_t;


typedef struct mvp_user_event_s{
    mvp_user_event_status_t event_status;
    mvp_user_event_type_t event_type;
    uint32_t event_id;
} mvp_user_event_t;


typedef struct{
    mvp_address_t src_mem;
    mvp_address_t dst_mem;
    size_t size;
    user_sync_t sysc_flag;
    mvp_user_event_t event;
}mem_copy_args_t;


extern int command_buf_get(void **data);
extern void command_buf_put(void *data);

extern void command_queue_init(void);
extern void command_queue_uinit(void);

#endif /* MVP_DRIVERS_COMMAND_QUEUE_H_ */
