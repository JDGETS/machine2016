#include "motors.h"

Motors::Motors()
  : _left(MOTOR_L_FORWARD_PIN, MOTOR_L_BACKWARD_PIN),
    _right(MOTOR_R_FORWARD_PIN, MOTOR_R_BACKWARD_PIN) {

}

void Motors::setSpeed(unsigned char speed) {
  _speed = speed;
}

void Motors::setAngular(unsigned char angular) {
  _angular = angular;
}

void Motors::setSpinning(bool enabled) {
  _spinning = enabled;
}

void Motors::write() {
  if(_spinning) {
    _left.setSpeed(255);
    _right.setSpeed(0);
  } else {
    _left.setSpeed(_speed);
    _right.setSpeed(_speed);
  }

  _left.write();
  _right.write();
}
