#define DEBUG

#ifdef DEBUG
#define PS2X_DEBUG
#define PS2X_COM_DEBUG
#endif

#include "PS2X_lib.h"
#include <TaskScheduler.h>
#include <Servo.h>

/*Define des servo*/
#define servo_pin_SF 13
/******************/

#define PS2_DAT        6
#define PS2_CMD        7
#define PS2_SEL        8
#define PS2_CLK        9
#define PS2_PRESSURES  false
#define PS2_RUMBLE     false

PS2X ps2x;
Scheduler scheduler;

Servo servo_test;
bool flip_done = false;

Task taskReadGamepad(30, -1, &handleReadGamepad);
void handleReadGamepad() {
  ps2x.read_gamepad();

  if(ps2x.ButtonPressed(PSB_SQUARE)) {
    Serial.println("Square just pressed");
  }

  if(ps2x.ButtonPressed(PSB_SQUARE)){
    flip_done = !flip_done;
  }

  if(flip_done == false){
    servo_test.write(0);
  }else{
    servo_test.write(180);
  }


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
