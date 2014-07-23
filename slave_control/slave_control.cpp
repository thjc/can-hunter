#include <ZumoMotors.h>

/*
* This example uses the ZumoMotors library to drive each motor on the Zumo
* forward, then backward. The yellow user LED is on when a motor should be
* running forward and off when a motor should be running backward. If a
* motor on your Zumo has been flipped, you can correct its direction by
* uncommenting the call to flipLeftMotor() or flipRightMotor() in the setup()
* function.
*/

#define LED_PIN 13

ZumoMotors motors;

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    // uncomment one or both of the following lines if your motors' directions need to be flipped
    //motors.flipLeftMotor(true);
    //motors.flipRightMotor(true);

    Serial.begin(9600);  // start serial for output
    Serial.println("!Setup");
    Serial.setTimeout(-1);
}

void loop()
{
    if (Serial.available())
    {
        char buffer[32];
        int ret = Serial.readBytesUntil('@',buffer,32);
        if (ret > 0)
        {
            ret = Serial.readBytes(buffer,5);
            if (ret == 5)
            {
                digitalWrite(LED_PIN, HIGH);
                
                int left_speed = -2*(buffer[1]);
                int right_speed = -2*(buffer[3]);
                if ('-' == buffer[0])
                {
                    left_speed *= -1;
                }
                if ('-' == buffer[2])
                {
                    right_speed *= -1;
                }
                int duration = 100*(buffer[4] - '0');
                
                motors.setLeftSpeed(left_speed);
                motors.setRightSpeed(right_speed);
                Serial.setTimeout(duration);
            }
            else
            {
                motors.setLeftSpeed(0);
                motors.setRightSpeed(0);
                digitalWrite(LED_PIN, LOW);
                Serial.setTimeout(-1);
                Serial.println("!Done");
            }
        }
    }    
}
