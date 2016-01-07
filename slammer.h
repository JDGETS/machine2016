#ifndef SLAMMER_H
#define SLAMMER_H

#define SLAMMER_SLAMMING_SPEED 255
#define SLAMMER_RETRACTING_SPEED 70
#define SLAMMER_COOLDOWN 10
#define DOWN_SWITCH A1
#define UP_SWITCH A0

class Slammer
{
public:
  Slammer(Adafruit_DCMotor *motor) {
    _motor = motor;
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
        if(_ticks >= 6000000) {
          _down_flag = true;
        } else {
          slam();
          moving = true;
          _up_flag = false;
          }
      }

      if(!_activated && !_up_flag) {
        if(_ticks >= 600000) {
          _up_flag = true;
        } else {
          retract();
          moving = true;
          _down_flag = false;
        }
      }
    }

    if(!moving) {
      _motor->run(RELEASE);
    }

    _ticks++;
  }

  void slam() {
    _motor->run(FORWARD);
    _motor->setSpeed(SLAMMER_SLAMMING_SPEED);
  }

  void retract() {
    _motor->run(BACKWARD);
    _motor->setSpeed(SLAMMER_RETRACTING_SPEED);
  }

private:
  bool _up_flag;
  bool _down_flag;
  bool _activated;
  int _ticks;
  int _cooldown;

  Adafruit_DCMotor *_motor;
};

#endif
