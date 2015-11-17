#include "motors.h"

Motors::Motors()
  : _left(MOTOR_L_FORWARD_PIN, MOTOR_L_BACKWARD_PIN),
    _right(MOTOR_R_FORWARD_PIN, MOTOR_R_BACKWARD_PIN) {

}

void Motors::setSpeed(char speed) {
  _speed = speed;
}

const char &Motors::speed() const {
  return _speed;
}

void Motors::setAngular(char angular) {
  _angular = constrain(angular, -127, 127);
}

const char &Motors::angular() const {
  return _angular;
}

void Motors::setSpinning(bool enabled) {
  _spinning = enabled;
}

const bool &Motors::spinning() const {
  return _spinning;
}

void Motors::write() {

  _currentSpeed = 0;
  if(_spinning) {
    _left.setSpeed(255); //Was 255
    
    _right.setSpeed(0);
  } else {
    // full left: -127
    // full right: 127

    Serial.print((int)_angular);
    if(_speed == 0) {
      _left.setSpeed();
      _right.setSpeed(constrain(-_angular, -127, 127)); 
      // ^ constrain to prevent wrapping from 128 to -127
    } else {
      if(_speed > 0) {
        if(_angular < 0) {
          _left.setSpeed(_speed + _angular);
          _right.setSpeed(_speed);
        } else {

          //Go Forward
          //Serial.print("Front\n");
          
          _left.setSpeed(_speed);
          _right.setSpeed(_speed - _angular);
        }
      } else {
        if(_angular < 0) {
          _left.setSpeed(_speed - _angular);
          _right.setSpeed(_speed);
        } else {

          //Go Backward
          //Serial.print("Back\n");
          _left.setSpeed(_speed);
          _right.setSpeed(_speed + _angular);
        }
      }

    }
  }

#ifdef DEBUG
  Serial.print("left: ");
  Serial.print(_left.speed(), DEC);
  Serial.print(" right: ");
  Serial.print(_right.speed(), DEC);
  Serial.print("\n");
#endif

  _left.write();
  _right.write();
}
