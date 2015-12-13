#ifndef MOTORS_H
#define MOTORS_H

#define MOTOR_L_FORWARD_PIN 5
#define MOTOR_L_BACKWARD_PIN 4
#define MOTOR_R_FORWARD_PIN 3
#define MOTOR_R_BACKWARD_PIN 2

#define MAX_SPEED 127
#define NORMAL_SPEED 90
#define ROTATION_RATIO 0.3
#define DAMP 5.0

#include "motor.h"

class Motors
{
public:
  Motors();
  Motor _left;
  Motor _right;

  void setTargetSpeed(char speed);
  const char &targetSpeed() const;
  
  void setSpeed(char speed);
  const char &speed() const;

  void setAngular(char angular);
  const char &angular() const;

  void setSpinning(bool enabled);
  const bool &spinning() const;

  void setBoost(const bool &boost);
  const bool &boost() const;

  void setDirection(char direction);

  void write();

private:
  char _direction;
  char _targetSpeed;
  char _speed;
  char _angular;
  bool _spinning;
  bool _boost;
};

#endif
