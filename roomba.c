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

}

int main() {

	char input;	
	while(1) {
		input = getche();
		//printf("%c\n",input);
		if(input == 'q')
			return 0;
	}
	

}
