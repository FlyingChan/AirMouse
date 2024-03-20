#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 8
#define TxD 7

SoftwareSerial blueToothSerial(RxD,TxD); //Serial I/O Object for talking with the Bluetooth Shield
bool mouseMode = false; //if we are currently controlling the mouse
 
void setup() 
{ 
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection();
 
  Mouse.begin();
  Keyboard.begin(); 
} 

void loop() 
{ 
  //We have a command to read
  if(blueToothSerial.available()) {
    char recieved;
    recieved = blueToothSerial.read();
    
    if(recieved == '#') {
      //We have recieved the 'switch' character, change modes
      mouseMode = !mouseMode;
      return;
    }
    
    if (mouseMode) {
      //We are in a mode to control the mouse
      
      switch(recieved) {
        case 'u':
          Mouse.move(0, -40);
          break;
        case 'd':
          Mouse.move(0, 40);
          break;
        case 'l':
          Mouse.move(-40, 0);
          break;
        case 'r':
          Mouse.move(40, 0);
          break;
      }
    }
    else {
      //We are in keyboard mode
      Keyboard.write(recieved);
    }
  }
} 
 
void setupBlueToothConnection()
{
  blueToothSerial.begin(9600); //Set BluetoothBee BaudRate to (9600) the default baud rate is 38400 
  blueToothSerial.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
  blueToothSerial.print("\r\n+STNA=SeedBT\r\n"); //set the bluetooth name as "SeedBT"
  blueToothSerial.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
  blueToothSerial.print("\r\n+STAUTO=0\r\n"); // Auto-connection should be forbidden here
  delay(2000); // This delay is required.
  blueToothSerial.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
  
  delay(2000); // This delay is required.
  blueToothSerial.flush();
}
