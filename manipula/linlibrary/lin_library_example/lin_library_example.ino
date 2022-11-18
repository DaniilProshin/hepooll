#include "lin.h"

int lin_rxpin = 4;
int lin_txpin = 2;
int sleep_pin = 8;
int baudrate = 9600;
byte ident = 0xA3;
byte data_size = 5;
byte data[] = {0,0,0,0,0};
byte output_size = 5;
byte output[] = {11,22,33,44,55};
LIN lin(lin_rxpin,lin_txpin);


void setup() {
  // put your setup code here, to run once:
  lin.begin(baudrate);
  Serial.begin(9600);
  pinMode(sleep_pin,OUTPUT);
  digitalWrite(sleep_pin,HIGH);
}

void loop() {
  Serial.print("Sending message: ");
  for(int i = 0;i < output_size;++i)
  {
    Serial.print(output[i]);
    Serial.print("  ");
  }
  Serial.println();
  lin.write(ident,output,output_size);
  delay(10);
  lin.writeRequest(ident);
  lin.readResponse(data,data_size);
  Serial.print("received response: ");
  for(int i = 0;i < data_size;++i)
  {
    Serial.print(data[i]);
    Serial.print("  ");
  }
  Serial.println();
  delay(1000);
}
