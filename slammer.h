#ifndef SLAMMER_H
#define SLAMMER_H

#define SLAMMER_SLAMMING_SPEED 100
#define SLAMMER_RETRACTING_SPEED 30
#define SLAMMER_COOLDOWN 10
#define DOWN_SWITCH A1
#define UP_SWITCH A0

class Slammer
{
public:
  Slammer(Adafruit_DCMotor *motor, const byte& ring_motor_pin) {
    _motor = motor;
    _ring_motor_pin = ring_motor_pin;
    _up_flag = false;
    _down_flag = false;
    _ticks = 0;
    _cooldown = 0;
  }

  void setup() {
    pinMode(DOWN_SWITCH, INPUT);
    pinMode(UP_SWITCH, INPUT);
  }

  void setActivated(const bool& activated) {
    if(_activated != activated) {
      Serial.print("Activated: ");
      Serial.println(activated ? "true" : "false");

      _ticks = 0;
      _cooldown = SLAMMER_COOLDOWN;
    }

    _activated = activated;
  }

  void run() {
    bool moving = false;

    if(digitalRead(DOWN_SWITCH) == HIGH) {
      Serial.println("hit down switch");
      _down_flag = true;
    }

    if(digitalRead(UP_SWITCH) == HIGH) {
      Serial.println("hit up switch");
      _up_flag = true;
    }

    if(_cooldown > 0) {
      _cooldown--;
    } else {
      if(_activated && !_down_flag) {
        if(_ticks >= 60) {
          _down_flag = true;
        } else {
          slam();
          moving = true;
          _up_flag = false;
          }
      }

      if(!_activated && !_up_flag) {
        if(_ticks >= 60) {
          _up_flag = true;
        } else {
          retract();
          moving = true;
          _down_flag = false;
        }
      }
    }

    if(_down_flag && _activate_ring_motor) {
      digitalWrite(_ring_motor_pin, HIGH);
    } else {
      digitalWrite(_ring_motor_pin, LOW);
    }

    if(!moving) {
      _motor->run(RELEASE);
    }

    _ticks++;
  }

  void slam() {
    _motor->run(BACKWARD);
    _motor->setSpeed(SLAMMER_SLAMMING_SPEED);
  }

  void retract() {
    _motor->run(FORWARD);
    _motor->setSpeed(SLAMMER_RETRACTING_SPEED);
  }

  void toggleRingMotor() {
    _activate_ring_motor = !_activate_ring_motor;
    Serial.println(_activate_ring_motor ? "ON" : "OFF");
  }

private:
  bool _up_flag;
  bool _down_flag;
  bool _activated;
  int _ticks;
  int _cooldown;
  byte _ring_motor_pin;
  bool _activate_ring_motor;

  Adafruit_DCMotor *_motor;
};

#endif
