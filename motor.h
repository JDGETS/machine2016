#ifndef MOTOR_H
#define MOTOR_H

#include <arduino.h>

#define MOTOR_L_FORWARD_PIN 1
#define MOTOR_L_BACKWARD_PIN 2
#define MOTOR_R_FORWARD_PIN 3
#define MOTOR_R_BACKWARD_PIN 4

// Motor class that controls a single motor with two PWM signals plugged on the
// provided pins for forward and backward movements.
class Motor
{
public:
  Motor(byte pin_forward, byte pin_backward);

  void setSpeed(byte speed);

  void write();

private:
  byte _speed;
  byte _pin_forward;
  byte _pin_backward;
};

#endif
