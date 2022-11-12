#include "GyverButton.h"
#include "motorhandler.h"


MotorHandler motor(9,10);
GButton butincrease(A3);
GButton butdecrease(A4);
GButton butswitch(A5);
 
void setup() {
  Serial.begin(9600);

}

void loop() {
  
  butincrease.tick();
  butdecrease.tick();
  butswitch.tick();

  if(butincrease.isClick())
  {
    motor.increasePWM();
    Serial.print("Increase button clicked: pwmtarget = ");
    Serial.println(motor.pwmtarget);
  }

  if(butdecrease.isClick())
  {
    motor.decreasePWM();
    Serial.print("Decrease button clicked: pwmtarget = ");
    Serial.println(motor.pwmtarget);
  }

  if(butswitch.isClick())
  {
    motor.switchPWM();
    Serial.print("Switch button clicked: pwmtarget = ");
    Serial.println(motor.pwmtarget);
  }

  motor.adjustrotation();

  
}
