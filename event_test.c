#include "event_test.h"
#include <sys/ioctl.h>
#include "mvp_ioctl.h"

int event_test(int fd)
{
	int ret = 0;
	int i=0;
	int event_id[4] = {0};

	for(i=0; i<4; i++){
		ret = ioctl(fd, DRM_IOCTL_MVP_CREATE_EVENT, &event_id[i]);
		if(ret < 0){
		   printf("%s, %d, create event failed!\n", __FUNCTION__, __LINE__);
		   return -1;  
		}
	}

	for(i=0; i<4; i++){
		ret = ioctl(fd, DRM_IOCTL_MVP_EVENT_DESTROY, &event_id[i]);
		if(ret < 0){
		   printf("%s, %d, free event failed!\n", __FUNCTION__, __LINE__);
		   return -1;  
		}
	}

	for(i=0; i<4; i++){
		ret = ioctl(fd, DRM_IOCTL_MVP_CREATE_EVENT, &event_id[i]);
		if(ret < 0){
		   printf("%s, %d, create event failed!\n", __FUNCTION__, __LINE__);
		   return -1;  
		}

		ret = ioctl(fd, DRM_IOCTL_MVP_EVENT_DESTROY, &event_id[i]);
		if(ret < 0){
		   printf("%s, %d, free event failed!\n", __FUNCTION__, __LINE__);
		   return -1;  
		}
		
	}
	return 0;
}




