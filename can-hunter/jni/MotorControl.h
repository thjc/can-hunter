#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <string>

class SerialMotors
{
public:
    SerialMotors(std::string device="/dev/ttyACM0");
    ~SerialMotors();

    void SetMotors(char LeftSpeed, char RightSpeed, int RunTimeMs);

    bool AllGood();

private:
    void SetupIfNeeded();

    int tty_fd;

    std::string device_name;
};

#endif
