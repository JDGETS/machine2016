#include "PS2X_lib.h"
#include "motors.h"
#include <TaskScheduler.h>
#include <Servo.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include "slammer.h"

// Controls
#define SPINNING_BUTTON PSB_CROSS
#define MOVE_FORWARD PSB_R2
#define MOVE_BACKWARD PSB_R1
#define ACTIVATE_PRECISE PSB_L2
#define ACTIVATE_SLAMMER PSB_L1
#define ACTIVATE_LAUNCHER PSB_TRIANGLE
#define WRAP_VEST PSB_PAD_UP
#define UNWRAP_VEST PSB_PAD_DOWN
#define TOGGLE_RING_MOTOR PSB_SQUARE

#define PS2_DAT        6
#define PS2_CMD        7
#define PS2_SEL        8
#define PS2_CLK        2
#define PS2_PRESSURES  false
#define PS2_RUMBLE     false

void handleReadGamepad();
void handleStopSpinning();

PS2X ps2x;
Scheduler scheduler;
Motors motors;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *slammerMotor = AFMS.getMotor(2);
Adafruit_DCMotor *wrapMotor = AFMS.getMotor(3);
Adafruit_DCMotor *samFrodonMotor = AFMS.getMotor(1);

Slammer slammer(slammerMotor);

Task taskReadGamepad(30, -1, &handleReadGamepad);
Task taskStopSpinning(300, 0, &handleStopSpinning);

bool slammerState = false;
byte samFrodonSpeed = 0;

void handleReadGamepad() {
  ps2x.read_gamepad();

  // Wrapper
  if(ps2x.ButtonIsDown(WRAP_VEST)) {
    wrapMotor->run(FORWARD);
    wrapMotor->setSpeed(127);
  } else if(ps2x.ButtonIsDown(UNWRAP_VEST)) {
    wrapMotor->run(BACKWARD);
    wrapMotor->setSpeed(127);
  } else {
    wrapMotor->run(RELEASE);
  }

  // Slammer
  if(ps2x.ButtonPressed(ACTIVATE_SLAMMER))  {
    slammerState = !slammerState;
  }

  slammer.setActivated(slammerState);
  slammer.run();

  // Sam/Frodon motor
  if(ps2x.ButtonPressed(PSB_CIRCLE)) {
     samFrodonSpeed++;
  }

  samFrodonMotor->run(FORWARD);
  samFrodonMotor->setSpeed((samFrodonSpeed % 2) * 255);

  // Motors
  byte angular = ps2x.Analog(PSS_LX);

  if(ps2x.ButtonIsDown(MOVE_FORWARD)) {
    motors.setDirection(1);
  } else if(ps2x.ButtonIsDown(MOVE_BACKWARD)) {
    motors.setDirection(-1);
  } else {
    motors.setDirection(0);
  }

  motors.setPrecise(ps2x.Button(ACTIVATE_PRECISE));
  motors.setAngular(angular - 128);

  motors.write();
}

void handleStopSpinning() {
  motors.setSpinning(false);
}

void setup() {
  Serial.begin(115200);

  AFMS.begin();

  slammer.setup();

  // Wait a bit for the controller to connect
  delay(500);

  // Configure the scheduler
  scheduler.init();
  scheduler.addTask(taskReadGamepad);

  // Handle disconnected receiver
  while(ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT,
                            PS2_PRESSURES, PS2_RUMBLE) == 2) {
    delay(500);
  }

  // Start reading loop
  taskReadGamepad.enable();
}

void loop() {
  scheduler.execute();
}
