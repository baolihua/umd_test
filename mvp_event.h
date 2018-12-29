/*
  mvp driver event
*/

#ifndef __MVP_EVENT__H__
#define __MVP_EVENT__H__

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/sched.h>


/* this only keep the same with userspace*/
typedef enum mvp_event_status_s{
    MVP_EVENT_STATUS_PENDING = 0,
    MVP_EVENT_STATUS_RUNNIG = 1,
    MVP_EVENT_STATUS_FINISH = 2,
}mvp_event_status_t;

typedef enum mvp_event_type_s{
    MVP_EVENT_DEFAULT = 0,
}mvp_event_type_t;


typedef struct mvp_event_s{
    mvp_event_status_t event_status;
    mvp_event_type_t event_type;
    unsigned int event_id;
} mvp_event_t;





#endif
