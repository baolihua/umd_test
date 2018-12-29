#include <stdio.h>
#include <stdint.h>
#include <fcntl.h> //open
#include <unistd.h> //close
#include "mem_test.h"
#include "event_test.h"
#include "host_to_dev.h"
#include "dev_to_dev.h"
#include "dev_to_host.h"

#define DEVICE_DIR "/dev/dri/renderD129"

int main(void)
{
	int ret = 0;
    int fd = open(DEVICE_DIR, O_RDWR);
    if(fd < 0){
        printf("open device: /dev/dri/renderD129  failed!\n");
		return -1;
    }

	ret = mem_test(fd);
	if(ret < 0){
		printf("mem test failed!\n");
	}else{
		printf("mem test succssful!\n");
	}


	ret = event_test(fd);
	if(ret < 0){
		printf("event test failed!\n");
	}else{
		printf("event test succssful!\n");
	}

	ret = host_to_dev_test(fd);
	if(ret < 0){
		printf("host_to_dev_test failed!\n");
	}else{
		printf("host_to_dev_test succssful!\n");
	}

	ret = dev_to_dev_test(fd);
	if(ret < 0){
		printf("dev_to_dev_test failed!\n");
	}else{
		printf("dev_to_dev_test succssful!\n");
	}

	ret = dev_to_host_test(fd);
	if(ret < 0){
		printf("dev_to_host_test failed!\n");
	}else{
		printf("dev_to_host_test succssful!\n");
	}
	
    close(fd);
    return 0;
}
