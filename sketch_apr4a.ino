#include <MPU6050.h>
#include <Mouse.h>
#include <Wire.h>
#include <Keyboard.h>
#include <SoftwareSerial.h>

int leftbutton = 4;
int zerobutton = 8;

#define MPU6050_INT_PIN 7
#define MPU6050_INT digitalPinToInterrupt(MPU6050_INT_PIN)
#define RxD 0
#define TxD 1


SoftwareSerial blueToothSerial(RxD, TxD);


const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

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

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  Mouse.begin();
  Keyboard.begin();
  Serial.println("Mouse Start!!");
  pinMode(leftbutton, INPUT_PULLUP);
  pinMode(zerobutton, INPUT_PULLUP);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
 setupBlueToothConnection(); 
}   

void loop(){
  if(blueToothSerial.available()) {
    char recieved;
    recieved = blueToothSerial.read();
    

    
      //We are in a mode to control the mouse
      
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
      AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
      AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
      int16_t gyroX, gyroZ;
      int Sensitivity = 300;
      gyroX = GyX / Sensitivity / 1.1 * -1;
      gyroZ = GyZ / Sensitivity * -1;
      int a=5;
      Mouse.move(gyroZ/a, gyroX/a);
      

    }
    

      if (digitalRead(leftbutton)==0) {
        Mouse.click(MOUSE_LEFT);
        delay(500);
      }
       
      if (digitalRead(zerobutton)==0) {
        Keyboard.write(' ');
        delay(500);
      }
    }
  


