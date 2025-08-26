#include <Arduino.h>
#include<Wire.h>
#include<math.h>

#define add           0x68
#define accSense      16384.0
#define gyroSense     131.0
#define deg           (180.0/PI) 
int16_t ax,ay,az,gx,gy,gz;


void setup() {
Serial.begin(115200);
Wire.begin();

Wire.beginTransmission(add);
Wire.write(0x6B);
Wire.write(0x00);
Wire.endTransmission(false);


}

void loop() {
  Wire.beginTransmission(add);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  if(Wire.requestFrom(add,14) == 14){
    ax = (int16_t)Wire.read() << 8 | Wire.read();
    ay = (int16_t)Wire.read() << 8 | Wire.read();
    az = (int16_t)Wire.read() << 8 | Wire.read();

    Wire.read(); Wire.read();

    gx = (int16_t)Wire.read() << 8 | Wire.read();
    gy = (int16_t)Wire.read() << 8 | Wire.read();
    gz = (int16_t)Wire.read() << 8 | Wire.read();

    ax = ax/accSense * deg;
    ay = ay/accSense * deg;
    az = az/accSense * deg;

    gx = gx/gyroSense;
    gy = gy/gyroSense;
    gz = gz/gyroSense;

        Serial.print("AX ");    
        Serial.print(ax);
        Serial.print("  | AY ");
        Serial.print(ay);
        Serial.print("  | AZ ");
        Serial.print(az);
        Serial.print("  | GX ");
        Serial.print(gx);
        Serial.print("  | GY ");
        Serial.print(gy);
        Serial.print("  | GZ ");
        Serial.print(gz);
        Serial.println("");    

      }

  delay(100);
}