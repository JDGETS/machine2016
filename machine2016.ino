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
#define ACTIVATE_BOOST PSB_L2
#define ACTIVATE_SLAMMER PSB_L1
#define ACTIVATE_LAUNCHER PSB_TRIANGLE

#define PS2_DAT        6
#define PS2_CMD        7
#define PS2_SEL        8
#define PS2_CLK        9
#define PS2_PRESSURES  false
#define PS2_RUMBLE     false

void handleReadGamepad();
void handleStopSpinning();

PS2X ps2x;
Scheduler scheduler;
Motors motors;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *slammerMotor = AFMS.getMotor(1);
Adafruit_DCMotor *launcherMotor = AFMS.getMotor(2);

Slammer slammer(slammerMotor);

Task taskReadGamepad(30, -1, &handleReadGamepad);
Task taskStopSpinning(300, 0, &handleStopSpinning);


void handleReadGamepad() {
  ps2x.read_gamepad();

  if(false && ps2x.ButtonPressed(SPINNING_BUTTON) && !motors.spinning()) {
    motors.setSpinning(true);
    taskStopSpinning.delay(300);
  }

  // Launcher
  if(ps2x.Button(ACTIVATE_LAUNCHER)) {
    launcherMotor->run(BACKWARD);
    launcherMotor->setSpeed(60);
  } else {
    launcherMotor->run(RELEASE);
  }

  // Slammer
  slammer.setActivated(ps2x.Button(ACTIVATE_SLAMMER));
  slammer.run();


  // Motors
  byte angular = ps2x.Analog(PSS_LX);

  if(ps2x.Button(MOVE_FORWARD)) {
    motors.setDirection(1);
  } else if(ps2x.Button(MOVE_BACKWARD)) {
    motors.setDirection(-1);
  } else {
    motors.setDirection(0);
  }

  motors.setBoost(ps2x.Button(ACTIVATE_BOOST));
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
  delay(300);

  // Configure the scheduler
  scheduler.init();
  scheduler.addTask(taskReadGamepad);

  taskReadGamepad.enable();

  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT,
                      PS2_PRESSURES, PS2_RUMBLE);
}

void loop() {
  scheduler.execute();
}
