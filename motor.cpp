#include "motor.h"

Motor::Motor(byte pin_forward, byte pin_backward) {
  _pin_forward = pin_forward;
  _pin_backward = pin_backward;
}

void Motor::setSpeed(char speed) {
  _speed = constrain(speed, -127, 127);
}

void Motor::write() {
  byte forward_val = 0;
  byte backward_val = 0;

  if(_speed < 0)  {
    backward_val = _speed * -2;
  } else if(_speed > 0) {
    forward_val = _speed * 2;
  }

  analogWrite(_pin_forward, forward_val);//forward_val
  analogWrite(_pin_backward, backward_val);
}

const char &Motor::speed() const {
  return _speed;
}
