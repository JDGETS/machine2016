#define DEBUG

#ifdef DEBUG
#define PS2X_DEBUG
#define PS2X_COM_DEBUG
#endif

#include "PS2X_lib.h"
#include <TaskScheduler.h>

#define PS2_DAT        6
#define PS2_CMD        7
#define PS2_SEL        8
#define PS2_CLK        9
#define PS2_PRESSURES  false
#define PS2_RUMBLE     false

PS2X ps2x;
Scheduler scheduler;

Task taskReadGamepad(30, -1, &handleReadGamepad);
void handleReadGamepad() {
  ps2x.read_gamepad();

  if(ps2x.ButtonPressed(PSB_SQUARE)) {
    Serial.println("Square just pressed");
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
}

void loop() {
  scheduler.execute();
}
