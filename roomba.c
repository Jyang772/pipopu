#include "libroomba.h"
#include "conio.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int r_init(char *dev) {

	int port;	//port descriptor
	int buf;
	port = open(dev, O_RDWR | O_NOCTTY);
	
	if(port == 0) {
		printf("ERROR OPENING PORT\n");
		return 0;
	}
	buf = 128;
	write(port,&buf,1);	//Start SCI
	buf = 130;
	write(port,&buf,1);	//Enable Serial Control
	return(port);
}

int roomba_drive(int fd, int16_t speed, int16_t radius)
{
	union drivecmd
	{
		//uint16_t cmdstr[6];
		uint8_t  cmdstr[6];
		uint16_t cmdints[3];
	} cmd;
	cmd.cmdstr[0] = 0;
	cmd.cmdstr[1] = 137;
	//cmd.cmdints[1] = 200;
	cmd.cmdints[1] = htons((uint16_t)speed); //http://stackoverflow.com/questions/19207745/htons-function-in-socket-programing
	cmd.cmdints[2] = htons((uint16_t)radius);
	//printf("%02x\n",cmd.cmdstr[0]);
	//printf("%02x\n",cmd.cmdstr[1]);
	//printf("%02x\n",cmd.cmdstr[3]);
	write(fd,(cmd.cmdstr+1),5);
	return(fd);
}

int main(int argc, char **argv) {

	int fd;
	char input;	

	fd = r_init(argv[1]);
	//sleep(2);
	roomba_drive(fd,200,0x8000);	

	while(1) {
		input = getche();
		//printf("%c\n",input);
		if(input == 'q')
			return 0;
	}
	

}
