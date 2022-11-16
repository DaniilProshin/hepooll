#include <SoftwareSerial.h>



const int linRx = 2;
const int linTx = 3;
const int displayRx = 0;
const int displayTx = 0;

const byte ident = 0xA3; // Identification Byte

byte from_lin_data_size = 5; 
byte from_lin_data[5]; 
//from lin data structure:
//2 bytes - rpm change value
//1 byte - reverse byte
//1 byte - pause
//1 byte - fault code
byte from_display_data_size = 5; 
byte from_display_data[5]; 
//from display data structure:
//2 bytes - rpm value
//1 byte - pause
//1 byte - something
SoftwareSerial lin(linRx,linTx);
SoftwareSerial display(displayRx,displayTx);


int LINread(byte data[], int data_size);
int LINwriteResponse(byte data[],int data_size);

void setup() {

  lin.begin(9600);
  display.begin(9600);

}

void loop() {

  int number_of_bytes_readen = LINread(from_lin_data, from_lin_data_size);
  if(number_of_bytes_readen > 3) // receiving package from lin
  {
    display.write(from_lin_data, from_lin_data_size);
  }
  else if(number_of_bytes_readen == 2) // receiving request from lin
  {
    LINwriteResponse(from_display_data,from_display_data_size);
  }
  if(display.available())
  {
    display.readBytes(from_display_data,from_display_data_size);
  }

}

int LINread(byte data[], byte data_size){
	byte rec[data_size+3];
  lin.begin(9600);

  int bytes_avbl = lin.available();
	if(bytes_avbl) // Check if there is an event on LIN bus
  { 
    lin.readBytes(rec,data_size+3);
		for(int j=0;j<data_size;j++)
    {
			data[j] = rec[j+2];
		}
	}
  
	return bytes_avbl;
}

int LINwriteResponse(byte data[],byte data_size)
{
  byte suma = 0;
	for(int i=0;i<data_size;i++) suma = suma + data[i];
	byte checksum = 255 - suma;
	// Start interface
		lin.begin(9600); // config Serial
		lin.write(data, data_size); // write data to serial
		lin.write(checksum); // write data to serial
		lin.end(); // clear Serial config
	return 1;
}
