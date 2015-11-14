#define DEBUG

#ifdef DEBUG
#define PS2X_DEBUG
#define PS2X_COM_DEBUG
#endif

#include "PS2X_lib.h"
#include "motors.h"
#include <TaskScheduler.h>
#include <Servo.h>

/*Define des servo*/
#define servo_pin_SF 13
/******************/

#define 5VOLT            6

// Controls
#define SPINNING_BUTTON PSB_CROSS
#define MOVE_FORWARD PSB_R2
#define MOVE_BACKWARD PSB_R1

#define PS2_DAT        6
#define PS2_CMD        7
#define PS2_SEL        8
#define PS2_CLK        9
#define PS2_PRESSURES  false
#define PS2_RUMBLE     false

PS2X ps2x;
Scheduler scheduler;
Motors motors;

Servo servo_test;
bool flip_done = false;

Task taskReadGamepad(30, -1, &handleReadGamepad);
Task taskStopSpinning(300, 1, &handleStopSpinning);

void handleReadGamepad() {
  ps2x.read_gamepad();

  if(ps2x.ButtonPressed(PSB_SQUARE)) {
    flip_done = !flip_done;
  }

  if(flip_done == false) {
    servo_test.write(0);
  } else {
    servo_test.write(180);
  }

  if(false && ps2x.ButtonPressed(SPINNING_BUTTON) && !motors.spinning()) {
    motors.setSpinning(true);
    taskStopSpinning.enable();
  }

  char speed = 0;
  byte angular = ps2x.Analog(PSS_LX);

  byte x = ps2x.Analog(PSS_LX);
  byte y = ps2x.Analog(PSS_LY);

  if(ps2x.Button(MOVE_FORWARD)) {
    speed = 127;
  } else if(ps2x.Button(MOVE_BACKWARD)) {
    speed = -127;
  }

  motors.setSpeed(speed);
  motors.setAngular(angular - 128);

  motors.write();
}

void handleStopSpinning() {
  motors.setSpinning(false);
}

void setup() {
  pinMode(5VOLT, OUTPUT);
  digitalWrite(5VOLT, HIGH);
  Serial.begin(115200);

  // Wait a bit for the controller to connect
  delay(300);

  // Configure the scheduler
  scheduler.init();
  scheduler.addTask(taskReadGamepad);
  // scheduler.enableAll();
  taskReadGamepad.enable();

  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT,
                      PS2_PRESSURES, PS2_RUMBLE);

  /*Set-up d'un servo*/
  servo_test.attach(servo_pin_SF);
}

void loop() {
  scheduler.execute();
}
