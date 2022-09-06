#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<linux/input.h>

#define DEV_KEY "/dev/input/event4"
#define DEV_MOUSE "/dev/input/event6"

int main() {
	int fd = -1, ret = 0;
	struct input_event ev;

	memset(&ev, 0, sizeof(struct input_event));
	fd = open(DEV_KEY, O_RDONLY);
	if (fd < 0) {
		printf("Fail to open %s", DEV_KEY);
		return -1;
	}

	while(1) {
		ret = read(fd, &ev, sizeof(struct input_event));
		if(ret < 0) {
			break;
		}
		printf("type:%d code:%d value:%d \n", ev.type, ev.code, ev.value);
	};
	close(fd);
	return 0;
}
