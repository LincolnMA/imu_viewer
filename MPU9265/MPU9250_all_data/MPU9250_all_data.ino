#include "MPU9250_WE.h"
#include <Wire.h>
#define MPU9250_ADDR 0x68

MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);
float g = 0,b = 0,a = 0;//gamma, beta and alpha 
float wg = 0,wb = 0,wa = 0;//angular velocity (global frame)
float p = 0,q = 0,r = 0;//angular velocity (local frame)
float dt = 10/1000.0;
float M[3][3] = {{1,sin(wg)*cos(wb),cos(wg)*tan(wb)},
                {0,cos(wg),-sin(wg)},
                {0,sin(wg)/cos(wb),cos(wg)/cos(wb)}};


void setup() {
  Serial.begin(115200);
  Wire.begin();
  if(!myMPU9250.init()){
    Serial.println("MPU9250 does not respond");
  }
  else{
    Serial.println("MPU9250 is connected");
  }
  if(!myMPU9250.initMagnetometer()){
    Serial.println("Magnetometer does not respond");
  }
  else{
    Serial.println("Magnetometer is connected");
  }

  Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
  delay(1000);
  myMPU9250.autoOffsets();
  Serial.println("Done!");
  myMPU9250.enableGyrDLPF();
  
  myMPU9250.setGyrDLPF(MPU9250_DLPF_6);

  myMPU9250.setSampleRateDivider(5);

  myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);

  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);

  myMPU9250.enableAccDLPF(true);

  myMPU9250.setAccDLPF(MPU9250_DLPF_6);

  myMPU9250.setMagOpMode(AK8963_CONT_MODE_100HZ);
  delay(200);
}

void loop() {
  
  xyzFloat gyr = myMPU9250.getGyrValues();
  p = gyr.x;
  q = gyr.y;
  r = gyr.z;
  wg = M[0][0]*p + M[0][1]*q + M[0][2]*r; 
  wb = M[1][0]*p + M[1][1]*q + M[1][2]*r;
  wa = M[2][0]*p + M[2][1]*q + M[2][2]*r;
  g = g + dt*wg;
  b = b + dt*wb;
  a = a + dt*wa;
  Serial.print(g);
  Serial.print(':');
  Serial.print(b);
  Serial.print(':');
  Serial.println(a);

  delay(10);
}
