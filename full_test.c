#include "full_test.h"
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

int full_test(int fd)
{
	int mem_size = 1024 * 1024;
	int str_len;
	int ret = 0;
	int size = 1024 * 64;

	/*mem*/
	mvp_alloc_args_s arg_0;
	mvp_alloc_args_s arg_1;
	arg_0.size = mem_size;
	arg_1.size = mem_size;
	/*event*/
	int event_id = 0;
	event_info events;

    mem_copy_args_t mem_arg;

	mem_copy_args_t mem_arg_dev_to_dev;

	mem_copy_args_t mem_arg_dev_to_host;
	
	char str[]="hello, mvp, the is my full test---------------------------------------";
	
	char *src = malloc(mem_size);
	char *dst = malloc(mem_size);

	if(!src || !dst){
		printf("%s, %d, malloc mem failed!\n", __FUNCTION__, __LINE__);
	}

	printf("%s, %d, src = %p\n", __FUNCTION__, __LINE__, src);
	printf("%s, %d, dst = %p\n", __FUNCTION__, __LINE__, dst);
	
	str_len = strlen(str);
	memcpy(src, str, str_len);
	memset(src + str_len, 'a', size); 
	memset(src + str_len + 1*size, 'b', size); 
	memset(src + str_len + 2*size, 'c', size); 
	memset(src + str_len + 3*size, 'd', size); 
	memset(src + str_len + 4*size, 'e', size); 
	memset(src + str_len + 5*size, 'f', size); 
	memset(src + str_len + 6*size, 'g', size);
	memset(src + str_len + 7*size, '1', size); 
	memset(src + str_len + 8*size, '2', size); 
	memset(src + str_len + 9*size, '3', size); 
	memset(src + str_len + 10*size, '4', size); 
	memset(src + str_len + 11*size, '5', size); 
	memset(src + str_len + 12*size, '6', size);
	memset(src + str_len + 13*size, '7', size); 
	memset(src + str_len + 14*size, '8', size); 
	memset(src + str_len + 15*size, '9', mem_size-str_len - 15*size);

	memset(dst, '0', mem_size);
	

	ret = ioctl(fd, DRM_IOCTL_MVP_ALLOC_DEV_MEM, &arg_0);
	if(ret < 0){
		printf("%s, %d, mem alloc failed\n", __FUNCTION__, __LINE__);
		return ret;
	}

	printf("%s, %d, arg_0.handle = %d\n", __FUNCTION__, __LINE__, arg_0.handle);

	ret = ioctl(fd, DRM_IOCTL_MVP_ALLOC_DEV_MEM, &arg_1);
	if(ret < 0){
		printf("%s, %d, mem alloc failed\n", __FUNCTION__, __LINE__);
		return ret;
	}
	
	printf("%s, %d, arg_1.handle = %d\n", __FUNCTION__, __LINE__, arg_1.handle);

	ret = ioctl(fd, DRM_IOCTL_MVP_CREATE_EVENT, &event_id);
	if(ret < 0){
	   printf("%s, %d, create event failed!\n", __FUNCTION__, __LINE__);
	   return -1;  
	}

	printf("%s, %d, event_id = %d\n", __FUNCTION__, __LINE__, event_id);

	mem_arg.src_mem = src;
	mem_arg.dst_mem = &arg_0.handle;
	mem_arg.event.event_id = event_id;
	mem_arg.size = mem_size;
	/*host to dev*/
	ret = ioctl(fd, DRM_IOCTL_MVP_COPY_HOST_TO_DEV, &mem_arg);
	if(ret < 0){
		printf("%s, %d, alloc mem failed!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	/*wait*/
	ret = ioctl(fd, DRM_IOCTL_MVP_WAIT_EVENT_DONE, &event_id);
	if(ret < 0){
		printf("%s, %d, event done failed!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	/*set event new status*/
	events.event_id = event_id;
	events.event_status = 1;
	ret = ioctl(fd, DRM_IOCTL_MVP_SET_EVENT_STATUS, &events);
	if(ret < 0){
		printf("%s, %d, copy dev to dev failed!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	/*dev to dev*/
	mem_arg_dev_to_dev.src_mem = &arg_0.handle;
	mem_arg_dev_to_dev.dst_mem = &arg_1.handle;
	mem_arg_dev_to_dev.event.event_id = event_id;
	mem_arg_dev_to_dev.size = mem_size;
	ret = ioctl(fd, DRM_IOCTL_MVP_COPY_DEV_TO_DEV, &mem_arg_dev_to_dev);
	if(ret < 0){
		printf("%s, %d, copy dev to dev failed!\n", __FUNCTION__, __LINE__);
		return -1;
	}


	/*wait*/
	ret = ioctl(fd, DRM_IOCTL_MVP_WAIT_EVENT_DONE, &event_id);
	if(ret < 0){
		printf("%s, %d, event done failed!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	/*set event new status*/
	events.event_id = event_id;
	events.event_status = 1;
	ret = ioctl(fd, DRM_IOCTL_MVP_SET_EVENT_STATUS, &events);
	if(ret < 0){
		printf("%s, %d, copy dev to dev failed!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	/*dev to host*/
	mem_arg_dev_to_host.src_mem = &arg_1.handle;
	mem_arg_dev_to_host.dst_mem = dst;
	mem_arg_dev_to_host.event.event_id = event_id;
	mem_arg_dev_to_host.size = mem_size;
	ret = ioctl(fd, DRM_IOCTL_MVP_COPY_DEV_TO_HOST, &mem_arg_dev_to_host);
	if(ret < 0){
		printf("%s, %d, copy dev to dev failed!\n", __FUNCTION__, __LINE__);
		return -1;
	 }

	/*wait*/
	ret = ioctl(fd, DRM_IOCTL_MVP_WAIT_EVENT_DONE, &event_id);
	if(ret < 0){
		printf("%s, %d, event done failed!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	/*compare data*/
	ret = strncmp(src, dst, mem_size);
	if(ret){
		printf("%s, %d, failed!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	/*free mem*/
	ret = ioctl(fd, DRM_IOCTL_MVP_FREE_DEV_MEM, &arg_0.handle);
	if(ret < 0){
		printf("%s, %d, mem free failed, i = %d\n", __FUNCTION__, __LINE__, i);
		return ret;
	}

	ret = ioctl(fd, DRM_IOCTL_MVP_FREE_DEV_MEM, &arg_1.handle);
	if(ret < 0){
		printf("%s, %d, mem free failed, i = %d\n", __FUNCTION__, __LINE__, i);
		return ret;
	}
	/*free event*/
	ret = ioctl(fd, DRM_IOCTL_MVP_EVENT_DESTROY, &event_id);
	if(ret < 0){
	   printf("%s, %d, free event failed!\n", __FUNCTION__, __LINE__);
	   return -1;  
	}
	return 0;
}





