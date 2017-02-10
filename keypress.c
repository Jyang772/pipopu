#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <string.h>
#include <stdio.h>


#define GRAB 1
#define UNGRAB 0

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

int init_keyboard() {

	int fd;
	int ret;
	struct uinput_user_dev uidev;
	struct input_event     ev;

	fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd",O_WRONLY | O_NONBLOCK);
/*
	if(fd < 0)
		exit(EXIT_FAILURE);
        if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
		exit(EXIT_FAILURE);
	if(ioctl(fd,UI_SET_KEYBIT, KEY_D) < 0)
		exit(EXIT_FAILURE);
	memset(&uidev, 0, sizeof(uidev));
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1;
	uidev.id.product = 0x1;
	uidev.id.version = 1;

        if(write(fd, &uidev, sizeof(uidev)) < 0)
		exit(EXIT_FAILURE);
        if(ioctl(fd, UI_DEV_CREATE) < 0)
		exit(EXIT_FAILURE);
*/	
	memset(&ev, 0, sizeof(ev));
	ev.type = EV_KEY;
	ev.code = KEY_D;
	ev.value = 1;

	ret = write(fd, &ev, sizeof(ev));
	//if(ioctl(fd, UI_DEV_DESTROY) < 0)
	//	exit(EXIT_FAILURE);

	close(fd);

}

int main(void)
{
    const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    struct input_event ev;
    ssize_t n;
    int fd;

    fd = open(dev, O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        return EXIT_FAILURE;
    }

    read(fd,&ev,sizeof ev); //read newline from terminal being sent to shell
    ioctl(fd,EVIOCGRAB,GRAB);
    

    int flag = 0;
    while (1) {
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else
        if (n != sizeof ev) {
            errno = EIO;
            break;
        }
	

	if(ev.type == EV_KEY && ev.value == 1 || ev.value == 2)
		switch((int)ev.code) {
			case KEY_W:
				printf("Moving forward\n");
				break;
			case KEY_A:
				printf("Moving left\n");
				break;
			case KEY_D:
				printf("Moving right\n");
				break;
			case KEY_S:
				printf("Moving backwards\n");
				break;	
			case KEY_Q:
				printf("Quitting\n");
				flag = 1;	
				break;
			default:
				break;
		}
	
	if(flag)
		break;

    }

    ioctl(fd,EVIOCGRAB,UNGRAB);

    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));
    return EXIT_FAILURE;
}
