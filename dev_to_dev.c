
#include "dev_to_dev.h"
#include <sys/ioctl.h>

#define DEV_TO_DEV_DEBUG

int dev_to_dev_test(int fd)
	{
		int ret = 0;
		
		unsigned int out_1;
		unsigned int out_2;
		mvp_ioctl_args_s args_1;
		args_1.in_args = 4096;
		args_1.out_data = &out_1;
		
		mvp_ioctl_args_s args_2;
		args_2.in_args = 4096;
		args_2.out_data = &out_2;
		
		unsigned int event_id;


		
		mem_copy_args_t mem_args;
	
		ret = ioctl(fd, DRM_IOCTL_MVP_ALLOC_DEV_MEM, &args_1);
		if(ret < 0){
		   printf("%s, %d, alloc mem 1 failed!\n", __FUNCTION__, __LINE__);
		   return -1;
		}
		
		ret = ioctl(fd, DRM_IOCTL_MVP_ALLOC_DEV_MEM, &args_2);
		if(ret < 0){
		   printf("%s, %d, alloc mem 2 failed!\n", __FUNCTION__, __LINE__);
		   return -1;
		}
		
#ifdef DEV_TO_DEV_DEBUG
		printf("args_1.out_data = 0x%x\n",*((unsigned int *)args_1.out_data));
		printf("args_2.out_data = 0x%x\n",*((unsigned int *)args_2.out_data));
#endif
		/*create event*/
		ret = ioctl(fd, DRM_IOCTL_MVP_CREATE_EVENT, &event_id);
		if(ret < 0){
		   printf("%s, %d, alloc mem failed!\n", __FUNCTION__, __LINE__);
		   return -1;
		}
	
#ifdef DEV_TO_DEV_DEBUG
		printf("event_id = %d\n",event_id);
#endif
	
	   /*test the device copy to device*/
	   mem_args.src_mem = &out_1;
	   mem_args.dst_mem = &out_2;
	   mem_args.event_id = event_id;
	   mem_args.size = 35;
	   
#ifdef DEV_TO_DEV_DEBUG
	   printf("src_mem = %s\n", (char *)(mem_args.src_mem));   
	   printf("dst_mem = %d\n", *(int *)(mem_args.dst_mem));   
	   printf("event_id = %d\n", mem_args.event_id);   
	   printf("size = %ld\n", mem_args.size);	
	   printf("&mem_args = %p\n", &mem_args);	
	   printf("src_mem = %p\n", mem_args.src_mem);	 
	   printf("dst_mem = %p\n", mem_args.dst_mem);	
#endif
	   
	   ret = ioctl(fd, DRM_IOCTL_MVP_COPY_DEV_TO_DEV, &mem_args);
	   if(ret < 0){
		   printf("%s, %d, copy dev to dev failed!\n", __FUNCTION__, __LINE__);
		   return -1;
		}
#ifdef DEV_TO_DEV_DEBUG
		printf("%s, %d, copy host to device, start to wait!\n", __FUNCTION__, __LINE__);
#endif  
	
	   /*test the wait queue*/
	   ret = ioctl(fd, DRM_IOCTL_MVP_WAIT_EVENT_DONE, &event_id);
	   if(ret < 0){
		   printf("%s, %d, event done failed!\n", __FUNCTION__, __LINE__);
		   return -1;
	   }
	   
#ifdef DEV_TO_DEV_DEBUG
	   printf("%s, %d, wait queue finished, start to release event !\n", __FUNCTION__, __LINE__);
#endif  
		ret = ioctl(fd, DRM_IOCTL_MVP_EVENT_DESTROY, &event_id);
		if(ret < 0){
		   printf("%s, %d, destroy event failed!\n", __FUNCTION__, __LINE__);
		   return -1;
		}
		if(args_1.out_data){
		  ret = ioctl(fd, DRM_IOCTL_MVP_FREE_DEV_MEM, args_1.out_data);
			if(ret < 0){
			   printf("%s, %d, free buffer failed!\n", __FUNCTION__, __LINE__);
			   return -1;
			}
		}
		
		if(args_1.out_data){
		  ret = ioctl(fd, DRM_IOCTL_MVP_FREE_DEV_MEM, args_1.out_data);
			if(ret < 0){
			   printf("%s, %d, free buffer failed!\n", __FUNCTION__, __LINE__);
			   return -1;
			}
		}
		
		return 0;
	}


