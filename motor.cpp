#include "motor.h"

Motor::Motor(byte pin_forward, byte pin_backward) {
  _pin_forward = pin_forward;
  _pin_backward = pin_backward;
}

void Motor::setSpeed(byte speed) {
  _speed = speed;
}

void Motor::write() {
  byte forward_val = 0;
  byte backward_val = 0;

  if(_speed < 127)  {
    forward_val = _speed * 2;
  } else {
    backward_val = _speed * 2;
  }

  analogWrite(_pin_forward, forward_val);
  analogWrite(_pin_backward, backward_val);
}
