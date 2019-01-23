
#include <sys/ioctl.h>
#include "host_to_dev.h"

#define HOST_TO_DEV_DEBUG

int host_to_dev_test(int fd)
{
    int ret = 0;
    unsigned int out;
    mvp_alloc_args_s args;
    args.size = 4096;
    unsigned int event_id;
    mem_copy_args_t mem_args;
    char str[] = "hello, ubuntu kernel, this is mvp!";

#ifdef HOST_TO_DEV_DEBUG
    printf("mem size is : %d\n", args.size);
#endif
    ret = ioctl(fd, DRM_IOCTL_MVP_ALLOC_DEV_MEM, &args);
    if(ret < 0){
       printf("%s, %d, alloc mem failed!\n", __FUNCTION__, __LINE__);
       return -1;
    }
#ifdef HOST_TO_DEV_DEBUG
    printf("args.handle = 0x%x\n",args.handle);
#endif
    /*create event*/
    ret = ioctl(fd, DRM_IOCTL_MVP_CREATE_EVENT, &event_id);
    if(ret < 0){
       printf("%s, %d, alloc mem failed!\n", __FUNCTION__, __LINE__);
       return -1;
    }

#ifdef HOST_TO_DEV_DEBUG
    printf("event_id = %d\n",event_id);
#endif

   /*test the host copy to device*/
   mem_args.src_mem = str;
   mem_args.dst_mem = &args.handle;
   mem_args.event.event_id = event_id;
   mem_args.size = sizeof(str);
   
#ifdef HOST_TO_DEV_DEBUG
   printf("str = %p\n", str);   
   printf("dst_mem = %p\n", (mem_args.dst_mem));   
   printf("event_id = %d\n", mem_args.event.event_id);   
   printf("size = %ld\n", mem_args.size);   
   printf("&mem_args = %p\n", &mem_args);   
   printf("src_mem = %p\n", mem_args.src_mem);   
   printf("dst_mem = %p\n", mem_args.dst_mem);  
#endif
   
   ret = ioctl(fd, DRM_IOCTL_MVP_COPY_HOST_TO_DEV, &mem_args);
   if(ret < 0){
       printf("%s, %d, alloc mem failed!\n", __FUNCTION__, __LINE__);
	   return -1;
    }
#ifdef HOST_TO_DEV_DEBUG
    printf("%s, %d, copy host to device, start to wait!\n", __FUNCTION__, __LINE__);
#endif  

   /*test the wait queue*/
   ret = ioctl(fd, DRM_IOCTL_MVP_WAIT_EVENT_DONE, &event_id);
   if(ret < 0){
       printf("%s, %d, event done failed!\n", __FUNCTION__, __LINE__);
	   return -1;
   }
   
#ifdef HOST_TO_DEV_DEBUG
   printf("%s, %d, wait queue finished, start to release event !\n", __FUNCTION__, __LINE__);
#endif  
    ret = ioctl(fd, DRM_IOCTL_MVP_EVENT_DESTROY, &event_id);
    if(ret < 0){
       printf("%s, %d, destroy event failed!\n", __FUNCTION__, __LINE__);
       return -1;
    }
    if(args.handle){
      ret = ioctl(fd, DRM_IOCTL_MVP_FREE_DEV_MEM, &args.handle);
        if(ret < 0){
           printf("%s, %d, free buffer failed!\n", __FUNCTION__, __LINE__);
		   return -1;
        }
    }
	return 0;
}






