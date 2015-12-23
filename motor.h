#ifndef MOTOR_H
#define MOTOR_H

#include <arduino.h>

// Motor class that controls a single motor with two PWM signals plugged on the
// provided pins for forward and backward movements.
class Motor
{
public:
  Motor(byte pin_forward, byte pin_backward);

  void setSpeed(char speed);
  const char &speed() const;

  void write();

private:
  char _speed;
  byte _pin_forward;
  byte _pin_backward;
};

#endif

