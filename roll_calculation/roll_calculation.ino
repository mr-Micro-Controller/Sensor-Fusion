#include <Arduino.h>
#include<Wire.h>
#include<math.h>

#define add           0x68
#define accSense      16384.0
#define gyroSense     131.0
#define sample        800
#define deg           (180.0/PI) 
#define alpha         .97

int16_t rawax,raway,rawaz,rawgx;
float ax,ay,az,gx;
float biasGX=0;
float angleX;
float roll;
unsigned long start=0;
unsigned long stop=0;
float dt=0;

void setup() {
Serial.begin(115200);
Wire.begin();

Wire.beginTransmission(add);
Wire.write(0x6B);
Wire.write(0x00);
Wire.endTransmission(false);

for(int i=0; i<sample; i++){
  Wire.beginTransmission(add);
  Wire.write(0x43);
  Wire.endTransmission(false);
  if(Wire.requestFrom(add,2) == 2){

    rawgx = (int16_t)Wire.read() << 8 | Wire.read();

    gx = (float)rawgx/gyroSense;

    biasGX += gx;

    delay(1);
}
}
biasGX /= sample;


Wire.beginTransmission(add);
Wire.write(0x3B);
Wire.endTransmission(false);
if(Wire.requestFrom(add,6) == 6) {
  rawax = (int16_t)(Wire.read() << 8 | Wire.read());
  raway = (int16_t)(Wire.read() << 8 | Wire.read());
  rawaz = (int16_t)(Wire.read() << 8 | Wire.read());

  ax = rawax / accSense;
  ay = raway / accSense;
  az = rawaz / accSense;

  roll = atan2(ay, az) * deg;  
}


}

void loop() {
  start = millis();
  dt = (start - stop)/1000.0; 
  stop = start;
  (dt>0.5)?(dt=0.5):(dt=dt);

  Wire.beginTransmission(add);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  if(Wire.requestFrom(add,10) == 10){
    rawax = (int16_t)Wire.read() << 8 | Wire.read();
    raway = (int16_t)Wire.read() << 8 | Wire.read();
    rawaz = (int16_t)Wire.read() << 8 | Wire.read();


    Wire.read(); Wire.read();

    rawgx = (int16_t)Wire.read() << 8 | Wire.read();
 

    ax = (float)rawax/accSense;
    ay = (float)raway/accSense;
    az = (float)rawaz/accSense; 
    
    angleX = atan2(ay,az) * deg;

    gx = (float)rawgx/gyroSense;

    gx -= biasGX;

    roll = alpha * (roll + (gx*dt)) + (1.0-alpha) * angleX ;
    
Serial.print(100);
Serial.print(" Roll ");        
Serial.print(roll);
Serial.print(" ");
Serial.print(-100);
Serial.println("");

      }
  else{
    Serial.println("Data fetch error check request byte and connection");
  }

  delay(8);
}