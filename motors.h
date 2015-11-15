#ifndef MOTORS_H
#define MOTORS_H

#define MOTOR_L_FORWARD_PIN 5
#define MOTOR_L_BACKWARD_PIN 4
#define MOTOR_R_FORWARD_PIN 3
#define MOTOR_R_BACKWARD_PIN 2

#include "motor.h"

class Motors
{
public:
  Motors();
  Motor _left;
  Motor _right;
  void setSpeed(char speed);
  const char &speed() const;

  void setAngular(char angular);
  const char &angular() const;

  void setSpinning(bool enabled);
  const bool &spinning() const;

  void write();

private:

  char _currentSpeed;
  char _speed;
  char _angular;
  bool _spinning;
};

#endif
