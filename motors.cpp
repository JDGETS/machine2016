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


void Motors::write() {
  _left.setSpeed(_speed);
  _right.setSpeed(_speed);

  _left.write();
  _right.write();
}
