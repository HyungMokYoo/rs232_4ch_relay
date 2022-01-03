#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#define		OPEN		1
#define		CLOSE		2
#define		TOGGLE		3
#define		MOMENTARY	4

#define COM3    "/dev/ttyUSB0"

static int fd3;
struct termios newtio3;

char read_status[8] = \
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAB};

char relay_open[4][8] = {
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x01, 0x01, 0xAD},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x02, 0x01, 0xAE},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x03, 0x01, 0xAF},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x04, 0x01, 0xB0}
};

char relay_close[4][8] = {
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x01, 0x02, 0xAE},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x02, 0x02, 0xAF},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x03, 0x02, 0xB0},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x04, 0x02, 0xB1}
};

char relay_toggle[4][8] = {
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x01, 0x03, 0xAF},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x02, 0x03, 0xB0},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x03, 0x03, 0xB1},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x04, 0x03, 0xB2}
};

char momentary[4][8] = {
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x01, 0x04, 0xB0},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x02, 0x04, 0xB1},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x03, 0x04, 0xB2},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x04, 0x04, 0xB3}
};

char relay_interlock[4][8] = {
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x01, 0x05, 0xB1},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x02, 0x05, 0xB2},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x03, 0x05, 0xB3},
	{0x55, 0x56, 0x00, 0x00, 0x00, 0x04, 0x05, 0xB4}
};

int serial_init(void)
{
	fd3 = open(COM3, O_RDWR | O_NOCTTY);

	newtio3.c_cflag = B9600;
	newtio3.c_cflag |= CS8;
	newtio3.c_cflag |= CLOCAL;
	newtio3.c_cflag |= CREAD;
	newtio3.c_iflag = 0;
	newtio3.c_oflag = 0;
	newtio3.c_lflag = 0;
	newtio3.c_cc[VTIME] = 0;
	newtio3.c_cc[VMIN] = 1;

	tcflush(fd3, TCIFLUSH);
	tcsetattr(fd3, TCSANOW, &newtio3);

	return 0;
}

int ctrl_relay(int relay_no, int ctrl)
{
	// cheking "relay_no"
	if((relay_no <= 0) || (relay_no > 4))
	{
		puts("relay_no is wroing. !!\n");
		return -1;
	}

	switch (ctrl)
	{
		case OPEN:
			write(fd3, relay_open[relay_no - 1], 8); break;
		case CLOSE:
			write(fd3, relay_close[relay_no - 1], 8); break;
		case TOGGLE:
			write(fd3, relay_toggle[relay_no - 1], 8); break;
		case MOMENTARY:
			write(fd3, momentary[relay_no - 1], 8); break;
		default:
			// cheking "ctrl"
			puts("ctrl number is wrong. !!\n"); return -1;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int bytes, i;

	serial_init();

    if(argc==1) 
	{
        printf("USEAGE: ./rs232_relay [PowerOn|PowerOff|Reboot]\n");
		return 0;
	}
    else if(argc==2) 
    { 
		if(0 == strcmp(argv[1], "PowerOn"))
		{
			ctrl_relay(1, MOMENTARY);
			sleep(1);
		}
		else if(0 == strcmp(argv[1], "PowerOff"))
		{
			ctrl_relay(1, OPEN);
			sleep(3);
			ctrl_relay(1, CLOSE);
		}
		else if(0 == strcmp(argv[1], "Reboot"))
		{
			ctrl_relay(2, MOMENTARY);
			sleep(1);
		}
		else
		{
			printf("Argument is wrong!!\n");
			printf("USEAGE: ./rs232_relay [PowerOn|PowerOff|Reboot]\n");
		}
    } 
	else if( argc > 2 )
	{
		printf("Too many arguments supplied.\n");
	}
	else
	{
		printf("One argument expected.\n");
	}

#if 0
	for(i=0;i<4;i++)
	{
		bytes = write(fd3, relay_open[i], 8);
		sleep(1);
	}

	for(i=0;i<4;i++)
	{
		bytes = write(fd3, relay_close[i], 8);
		sleep(1);
	}

	for(i=0;i<4;i++)
	{
		bytes = write(fd3, momentary[i], 8);
		sleep(1);
	}
	ctrl_relay(1, OPEN); sleep(1);
	ctrl_relay(2, OPEN); sleep(1);
	//ctrl_relay(3, OPEN); sleep(1);
	//ctrl_relay(4, OPEN); sleep(1);

	ctrl_relay(1, CLOSE); sleep(1);
	ctrl_relay(2, CLOSE); sleep(1);
	//ctrl_relay(3, CLOSE); sleep(1);
	//ctrl_relay(4, CLOSE); sleep(1);

	ctrl_relay(1, MOMENTARY); sleep(1);
	ctrl_relay(2, MOMENTARY); sleep(1);
	//ctrl_relay(3, MOMENTARY); sleep(1);
	//ctrl_relay(4, MOMENTARY); sleep(1);
#endif
	close(fd3);

	return 0;
}
