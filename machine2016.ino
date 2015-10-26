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

// Controls
#define SPINNING_BUTTON PSB_CROSS

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
Task taskStopSpinorama(300, 1, &handleStopSpinorama);

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

  if(ps2x.ButtonPressed(SPINNING_BUTTON)) {
    motors.setSpinning(true);
    taskStopSpinorama.enable();
  }

  byte speed = ps2x.Analog(PSS_LY);
  byte angular = ps2x.Analog(PSS_RX);

  motors.setSpeed(speed);
  motors.setAngular(angular);
}


void handleStopSpinorama() {
  motors.setSpinning(false);
}

void setup() {
  Serial.begin(115200);

  // Wait a bit for the controller to connect
  delay(300);

  // Configure the scheduler
  scheduler.init();
  scheduler.addTask(taskReadGamepad);
  scheduler.enableAll();


  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT,
                      PS2_PRESSURES, PS2_RUMBLE);

  /*Set-up d'un servo*/
  servo_test.attach(servo_pin_SF);

}

void loop() {
  scheduler.execute();
}
