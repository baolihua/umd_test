
#include "mem_test.h"
#include <sys/ioctl.h>

int mem_test(int fd)
{
	int i = 0;
	int ret = 0;
	
	mvp_alloc_args_s args[4];
	
    args[0].size = 256;
	args[1].size = 1024;
	args[2].size = 4096;
	args[3].size = 8192;

	for(i=0; i<4; i++){
		ret = ioctl(fd, DRM_IOCTL_MVP_ALLOC_DEV_MEM, &args[i]);
		if(ret < 0){
			printf("%s, %d, mem alloc failed, i = %d\n", __FUNCTION__, __LINE__, i);
			return ret;
		}
	}

	for(i=0; i<4; i++){
		ret = ioctl(fd, DRM_IOCTL_MVP_FREE_DEV_MEM, &args[i].handle);
		if(ret < 0){
			printf("%s, %d, mem free failed, i = %d\n", __FUNCTION__, __LINE__, i);
			return ret;
		}
	}

	for(i=0; i<4; i++){
		ret = ioctl(fd, DRM_IOCTL_MVP_ALLOC_DEV_MEM, &args[i]);
		if(ret < 0){
			printf("%s, %d, mem alloc failed, i = %d\n", __FUNCTION__, __LINE__, i);
			return ret;
		}

		ret = ioctl(fd, DRM_IOCTL_MVP_FREE_DEV_MEM, &args[i].handle);
		if(ret < 0){
			printf("%s, %d, mem free failed, i = %d\n", __FUNCTION__, __LINE__, i);
			return ret;
		}
	}
	return 0;
}


