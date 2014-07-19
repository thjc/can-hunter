#include "MotorControl.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h> // needed for memset

SerialMotors::SerialMotors(std::string device) : device_name(device), tty_fd(0)
{
}

void SerialMotors::SetupIfNeeded()
{
    if (tty_fd <= 0)
    {
		struct termios tio;
		struct termios stdio;

		memset(&tio,0,sizeof(tio));
		tio.c_iflag=0;
		tio.c_oflag=0;
		tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
		tio.c_lflag=0;
		tio.c_cc[VMIN]=1;
		tio.c_cc[VTIME]=5;

		tty_fd=open(device_name.c_str(), O_RDWR | O_NONBLOCK);
		cfsetospeed(&tio,B9600);            // 115200 baud
		cfsetispeed(&tio,B9600);            // 115200 baud

		tcsetattr(tty_fd,TCSANOW,&tio);
    }
}

SerialMotors::~SerialMotors()
{
    if (tty_fd > 0)
    {
        close(tty_fd);
    }
}

bool SerialMotors::AllGood()
{
	return tty_fd > 0;
}

void SerialMotors::SetMotors(char LeftSpeed, char RightSpeed, int RunTimeMs){
    struct __attribute__ ((__packed__)) motor_command
    {
        char pad[8];
        char header;
        char leftDirection;
        char leftSpeed;
        char rightDirection;
        char rightSpeed;
        char duration;
        char newline;
    };
    motor_command command;
    memset(command.pad, '!', 8);
    command.header = '@';
    command.leftDirection = LeftSpeed < 0 ? '-' : '+';
    command.leftSpeed = (LeftSpeed < 0 ? -LeftSpeed : LeftSpeed);
    command.rightDirection = RightSpeed < 0 ? '-' : '+';
    command.rightSpeed = (RightSpeed < 0 ? -RightSpeed : RightSpeed);
    command.duration = '0' + (RunTimeMs/1000);
    command.newline = '\n';

    SetupIfNeeded();
    int ret = write(tty_fd,(char*) &command,sizeof(command));
    if (ret == -1) {
        tty_fd = 0;
    }
}


