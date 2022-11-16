#include "GyverButton.h"
#include "motorhandler.h"
#include "lin.h"

MotorHandler motor(9,10);
GButton butincrease(A3);
GButton butdecrease(A4);
GButton butswitch(A5);

int rpmvalue = 0;
unsigned long previous_time = 0;
unsigned long current_time = 0;
unsigned long communication_period = 100;

int lin_rxpin = 2;
int lin_txpin = 3;
int baudrate = 9600;
byte ident = 0xA3;

const int input_data_size = 5;
byte input_data[input_data_size];
//input data structure:
//2 bytes - rpm value
//1 byte - pause
//1 byte - something
const int output_data_size = 5;
byte output_data[output_data_size];
//output data structure:
//1 bytes - rpm change value
//1 byte - reverse byte
//1 byte - pause
//1 byte - fault code
LIN lin(lin_rxpin,lin_txpin);

 
void setup() {
  //Serial.begin(9600); 
  lin.begin(baudrate);
}

void loop() {
  
  bool changesmade = 0;

  /*
  lin.writeRequest(ident);
  lin.readResponse(data,data_size);

  delay(1000);
  */

  butincrease.tick();
  butdecrease.tick();
  butswitch.tick();

  if(butincrease.isClick()) // increase
  {
    output_data[0] = motor.pwmtargetchangefactor;
    lin.write(ident, output_data, output_data_size);
    output_data[0] = 0;
    changesmade = 1;
    //motor.increasePWM();
    //Serial.print("Increase button clicked: pwmtarget = ");
    //Serial.println(motor.pwmtarget);
  }

  if(butdecrease.isClick()) // decrease
  {
    output_data[0] = (-1)*motor.pwmtargetchangefactor;
    lin.write(ident, output_data, output_data_size);
    output_data[0] = 0;
    changesmade = 1;
    //motor.decreasePWM();
    //Serial.print("Decrease button clicked: pwmtarget = ");
    //Serial.println(motor.pwmtarget);
  }

  if(butswitch.isClick()) // switch direction
  {
    (output_data[1])?(output_data[1] = 0):(output_data[1] = 1); 
    lin.write(ident, output_data, output_data_size);
    (output_data[1])?(output_data[1] = 0):(output_data[1] = 1); 
    changesmade = 1;
    //motor.switchPWM();
    //Serial.print("Switch button clicked: pwmtarget = ");
    //Serial.println(motor.pwmtarget);
  }

  if(butswitch.isHold())// pause
  {
    (output_data[2])?(output_data[2] = 0):(output_data[2] = 1); 
    lin.write(ident, output_data, output_data_size);
    (output_data[2])?(output_data[2] = 0):(output_data[2] = 1);
    changesmade = 1;
  }

  if(changesmade)
  {
    lin.writeRequest(ident);
    lin.readResponse(input_data,input_data_size);
    rpmvalue = input_data[0];
    rpmvalue = rpmvalue << 8;
    rpmvalue += input_data[1];

  }
  else
  {
    current_time = millis();

    if(current_time - previous_time >= communication_period)
    {
      lin.writeRequest(ident);
      lin.readResponse(input_data,input_data_size);
      previous_time = current_time;
      rpmvalue = input_data[0];
      rpmvalue = rpmvalue << 8;
      rpmvalue += input_data[1];
    }
  }
  
 
  motor.adjustrotation(rpmvalue);
  
}
