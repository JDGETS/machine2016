#ifndef MOTORS_H
#define MOTORS_H

#include "motor.h"

class Motors
{
public:
  Motors();

  void setSpeed(byte speed);

  void setAngular(byte angular);

  void write();

private:
  Motor _left;
  Motor _right;
  byte _speed;
  byte _angular;
};

#endif
