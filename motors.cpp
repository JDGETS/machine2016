#include "motors.h"

Motors::Motors()
  : _left(MOTOR_L_FORWARD_PIN, MOTOR_L_BACKWARD_PIN),
    _right(MOTOR_R_FORWARD_PIN, MOTOR_R_BACKWARD_PIN) {
}

void Motors::setTargetSpeed(char speed) {
  _targetSpeed = speed;
}

const char &Motors::targetSpeed() const {
  return _targetSpeed;
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

void Motors::setBoost(const bool &boost) {
  _boost = boost;
}

const bool &Motors::boost() const {
  return _boost;
}

void Motors::setDirection(char direction) {
  if(_boost) {
    _targetSpeed = direction * MAX_SPEED;
    _speed = _targetSpeed;
  } else {
    _targetSpeed = direction * NORMAL_SPEED;
  }

  if(direction == 0) {
    _targetSpeed = 0;
    _speed = 0;
  } else {
    if(-direction == _direction) {
      _speed = _targetSpeed;
    }
  }

  _direction = direction;
}

void Motors::write() {
  // [-127, 127] -> [0, 127] -> [1, ROTATION_RATIO]
  float ratio = 1 - abs(_angular) / 127.0 * (1 - ROTATION_RATIO);

  if(abs(_speed) < abs(_targetSpeed)) {
    _speed += _direction * ACCELERATION_FACTOR;
  } else {
    _speed = _targetSpeed;
  }

  if(_speed == 0) {
    // In-place rotation
     char dir = sign(_angular);
     
    if(_boost) {
      _left.setSpeed(dir * MAX_SPEED);
      _right.setSpeed(-dir * MAX_SPEED);
    } else {
      char rotationSpeed = map(abs(_angular), 0, 127, 0, NORMAL_SPEED);
      
      _left.setSpeed(dir * rotationSpeed);
      _right.setSpeed(-dir * rotationSpeed);
    }
    
  } else {
    // Regular movements
    
    if(_angular < 0) {
      _left.setSpeed(_speed * ratio);
      _right.setSpeed(_speed);
    } else {
      _left.setSpeed(_speed);
      _right.setSpeed(_speed * ratio);
    }
  }

#ifdef DEBUG
  Serial.print("left: ");
  Serial.print(_left.speed(), DEC);
  Serial.print(" right: ");
  Serial.print(_right.speed(), DEC);
  Serial.print(" ");
  Serial.print(_direction, DEC);
  Serial.print("\n");
#endif

  _left.write();
  _right.write();
}

