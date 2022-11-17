#include "lin.h"

int lin_rxpin = 2;
int lin_txpin = 3;
int baudrate = 9600;
byte ident = 0xA3;
LIN lin(lin_rxpin,lin_txpin);


void setup() {
  // put your setup code here, to run once:
  lin.begin(baudrate);
  Serial.begin(9600);
}

void loop() {
  byte data_size = 8;
  //byte data[data_size];
  byte data[] = {0,0,0,0,0,0,0,0};
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
