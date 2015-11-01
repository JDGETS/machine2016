#ifndef MOTORS_H
#define MOTORS_H

#include "motor.h"

class Motors
{
public:
  Motors();

  void setSpeed(byte speed);

  void setAngular(byte angular);

  void setSpinning(bool enabled);
  const bool &spinning() const;

  void write();

private:
  Motor _left;
  Motor _right;
  byte _speed;
  byte _angular;
  bool _spinning;
};

#endif
