#include<Wire.h>
#include <HardwareSerial.h>
#include <Arduino.h>

#define magAdd      0x0D
#define DDRY        0x06
#define magSense    3000.0
#define alpha       .98
// 0x09 --> control register 1  set to 0b00011101 
// 0x0A --> control register 2  set to 0b10000000  for soft reset 
// 0x0B --> set/reset period register set to 0b00000001 recommended
// 0x06 --> data ready register (read only) if 0 no new data , if 1 data is availble

int16_t mx,my,mz;
float x,y,z;

void setup() {
  
  Serial.begin(115200);
  Wire.begin();

  Wire.beginTransmission(magAdd);
  Wire.write(0x0B);                          //set reset period
  Wire.write(0b00000001);               
  Wire.endTransmission();

  Wire.beginTransmission(magAdd);
  Wire.write(0x0A);                      
  Wire.write(0b10000000);                     //soft reset
  Wire.endTransmission();           
  delay(100);

  Wire.beginTransmission(magAdd);
  Wire.write(0x09);                //configuration register 
  Wire.write(0b00011101);          //continuous , 200Hz , 8G , 512 sample 
  Wire.endTransmission();
}

void loop() { 

  
  Wire.beginTransmission(magAdd);
  Wire.write(DDRY);
  Wire.endTransmission();
  if(Wire.requestFrom(magAdd,1) == 1){
    int8_t check = Wire.read();
    if(check & 0b00000001){
  Wire.beginTransmission(magAdd);
  Wire.write(0x00);
  Wire.endTransmission(true);
  if(Wire.requestFrom(magAdd,6) == 6){                       //return LSB first then MSB
    mx = ( Wire.read() | (int16_t)Wire.read() << 8 );
    my = ( Wire.read() | (int16_t)Wire.read() << 8 ); 
    mz = ( Wire.read() | (int16_t)Wire.read() << 8 );

      x = mx/magSense;
      y = my/magSense;
      z = mz/magSense;

        Serial.print("Mx ");
        Serial.print(x);               //yaw movement
        Serial.print("  | My ");
        Serial.print(y);
        Serial.print("  | Mz ");
        Serial.print(z);
        Serial.println("");         
        
             }
    }
    else{
      Serial.println("Data register not ready");
    }
  }
delay(10);
}
