#ifndef MOTORS_H
#define MOTORS_H

#define MOTOR_L_FORWARD_PIN 11
#define MOTOR_L_BACKWARD_PIN 3
#define MOTOR_R_FORWARD_PIN 10
#define MOTOR_R_BACKWARD_PIN 9

#define NORMAL_SPEED 120
#define PRECISE_SPEED 50
#define ROTATION_RATIO 0.3
#define ACCELERATION_FACTOR 10

#include "motor.h"

template <typename type>
type sign(type value) {
  return type((value>0) - (value<0));
}

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

  void setPrecise(const bool& precise);
  const bool& precise() const;

  void setDirection(char direction);

  void write();

private:
  char _direction;
  char _targetSpeed;
  char _acceleration;
  char _speed;
  char _angular;
  bool _spinning;
  bool _precise;
};

#endif
