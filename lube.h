#ifndef LUBE_H
#define LUBE_H

#define LUBE_LIMIT_SWITCH A2

class Lube
{
public:
  Lube(Adafruit_DCMotor *motor) {
    _motor = motor;
  }

  void setup() {
    pinMode(LUBE_LIMIT_SWITCH, INPUT);
  }

  void spray() {
    if(digitalRead(LUBE_LIMIT_SWITCH) == LOW) {
      _motor->setSpeed(255);
      _motor->run(FORWARD);
    } else {
      stop();
    }
  }

  void reset() {
    _motor->setSpeed(255);
    _motor->run(BACKWARD);
  }

  void stop() {
    _motor->run(RELEASE);
  }

private:
  Adafruit_DCMotor *_motor;
};

#endif
