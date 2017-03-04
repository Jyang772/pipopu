#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>


static sig_atomic_t end = 0;

static int firstReleased = 0;

static void sighandler(int signo)
{
    end = 1;
}

int main()
{

    struct termios oldtio, curtio;
    struct sigaction sa;

    /* Save stdin terminal attributes */
    tcgetattr(0, &oldtio);

    /* Make sure we exit cleanly */
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sighandler;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    /* This is needed to be able to tcsetattr() after a hangup (Ctrl-C)
     * see tcsetattr() on POSIX
     */
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGTTOU, &sa, NULL);

    /* Set non-canonical no-echo for stdin */
    tcgetattr(0, &curtio);
    curtio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &curtio);

    /* main loop */
    while (!end) {
            struct pollfd pfds[1];
            int ret;
            char c;
	    int flag = 0;

            /* See if there is data available */
            pfds[0].fd = 0;
            pfds[0].events = POLLIN;
            ret = poll(pfds, 1, 0);

            /* Consume data */
            if (ret > 0) {
                    printf("Data available\n");
                    read(0, &c, 1);
		    //printf("%c\n",c);
 
		    switch(c) {
                        case 'w':
                                printf("Moving forward\n");
                                break;
                        case 'a':
                                printf("Moving left\n");
                                break;
                        case 'd':
                                printf("Moving right\n");
                                break;
                        case 's':
                                printf("Moving backwards\n");
                                break;
                        case 'q':
                                printf("Quitting\n");
                                flag = 1;
                                break;
                        default:
                                break;
                    }
            }
	if(flag)
		break;
    }

    /* restore terminal attributes */
    tcsetattr(0, TCSANOW, &oldtio);

    return 0;
}
