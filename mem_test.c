
#include "mem_test.h"
#include <sys/ioctl.h>

int mem_test(int fd)
{
	int mem_id_1 = 0;
	int mem_id_2 = 0;
	int mem_id_3 = 0;
	int mem_id_4 = 0;
	int i = 0;
	int ret = 0;
	
	mvp_ioctl_args_s args[4];
	
    args[0].in_args = 256;
	args[0].out_data = &mem_id_1;
	args[1].in_args = 1024;
	args[1].out_data = &mem_id_2;
	args[2].in_args = 4096;
	args[2].out_data = &mem_id_3;
	args[3].in_args = 8192;
	args[3].out_data = &mem_id_4;

	for(i=0; i<4; i++){
		ret = ioctl(fd, DRM_IOCTL_MVP_ALLOC_DEV_MEM, &args[i]);
		if(ret < 0){
			printf("%s, %d, mem alloc failed, i = %d\n", __FUNCTION__, __LINE__, i);
			return ret;
		}
	}

	for(i=0; i<4; i++){
		ret = ioctl(fd, DRM_IOCTL_MVP_FREE_DEV_MEM, args[i].out_data);
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

		ret = ioctl(fd, DRM_IOCTL_MVP_FREE_DEV_MEM, args[i].out_data);
		if(ret < 0){
			printf("%s, %d, mem free failed, i = %d\n", __FUNCTION__, __LINE__, i);
			return ret;
		}
	}
	return 0;
}


