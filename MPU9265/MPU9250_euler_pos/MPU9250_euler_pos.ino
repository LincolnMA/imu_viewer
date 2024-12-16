#include "MPU9250_WE.h"
#include <Wire.h>
#define MPU9250_ADDR 0x68

MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);
float g = 0,b = 0,a = 0;//gamma, beta and alpha 
float g2 = 0,b2 = 0,a2 = 0;//gamma, beta and alpha 
float wg = 0,wb = 0,wa = 0;//angular velocity (global frame)
float p = 0,q = 0,r = 0;//angular velocity (local frame)
float dt;//delta t
float mag_g;
unsigned long t_ini = 0,t_fim = 0;//marcação de tempo
float gx[2] = {0,0},gy[2] = {0,0},gz[2] = {0,0};
float temp_gx,temp_gy,temp_gz;
float vx[2] = {0,0},vy[2] = {0,0},vz[2] = {0,0};
float dx = 0,dy = 0,dz = 0;
float G = 9.807;
unsigned long count = 0;
int i;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  if(!myMPU9250.init()){
    //Serial.println("MPU9250 does not respond");
  }
  else{
    //Serial.println("MPU9250 is connected");
  }
  if(!myMPU9250.initMagnetometer()){
    //Serial.println("Magnetometer does not respond");
  }
  else{
    //Serial.println("Magnetometer is connected");
  }

  //Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
  delay(1000);
  myMPU9250.autoOffsets();
  //Serial.println("Done!");
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
  xyzFloat gValue = myMPU9250.getGValues();
  //gama e beta do acelerômetro
  g2 = atan2(-gValue.z,gValue.y);
  b2 = atan2(gValue.y*cos(g2) - gValue.z*sin(g2),-gValue.z);
  //construção da matriz de rotação
  float sa = sin(a);
  float ca = cos(a);
  float sb = sin(b);
  float cb = cos(b);
  float sg = sin(g);
  float cg = cos(g);                 
  float M[3][3] = {{ca*cb,ca*sb*sg - sa*cg,ca*sb*cg + sa*sg},
                   {sa*cb,sa*sb*sg + ca*cg,sa*sb*cg - ca*sg},
                   {-sb  ,cb*sg          ,cb*cg}};
  //convertendo graus/s para radianos/s
  p = gyr.x*PI/180;
  q = gyr.y*PI/180;
  r = gyr.z*PI/180;
  //multiplicando pela matriz de rotação
  wg = M[0][0]*p + M[0][1]*q + M[0][2]*r; 
  wb = M[1][0]*p + M[1][1]*q + M[1][2]*r;
  wa = M[2][0]*p + M[2][1]*q + M[2][2]*r;
  temp_gx = M[0][0]*gValue.x + M[0][1]*gValue.y + M[0][2]*gValue.z; 
  temp_gy = M[1][0]*gValue.x + M[1][1]*gValue.y + M[1][2]*gValue.z;
  temp_gz = M[2][0]*gValue.x + M[2][1]*gValue.y + M[2][2]*gValue.z - 1;//subtraindo 1G do componente z
  gx[0] = abs(gx[0] - temp_gx) < 0.1 ? temp_gx*.8 : temp_gx;
  gy[0] = abs(gy[0] - temp_gy) < 0.1 ? temp_gy*.8 : temp_gy;
  gz[0] = abs(gz[0] - temp_gz) < 0.1 ? temp_gz*.8 : temp_gz;
  //calculando período de amostragem
  t_fim = millis();f
  dt = (t_fim - t_ini)/1000.0;
  t_ini = t_fim;
  //magnitude da gravidade
  mag_g = sqrt(gValue.x*gValue.x + gValue.y*gValue.y + gValue.z*gValue.z);
  //integral simples
  g = g + dt*wg;
  b = b + dt*wb;
  a = a + dt*wa;
  //troca de referencia
  //g = mag_g >1.1? g : g2;
  //b = mag_g >1.1? b : b2;
  //integral por trapézios para obtenção de velocidades
  vx[0] = vx[1] + (gx[0] + gx[1])*G*dt/2;
  vy[0] = vy[1] + (gy[0] + gy[1])*G*dt/2;
  vz[0] = vz[1] + (gz[0] + gz[1])*G*dt/2;
  //integral por trapézios para obtenção de espaço percorrido
  dx = dx + (vx[0] + vx[1])*dt/2;
  dy = dy + (vy[0] + vy[1])*dt/2;
  dz = dz + (vz[0] + vz[1])*dt/2;
  //atualizando valores antigos pelos novos
  gx[1] = gx[0];
  gy[1] = gy[0];
  gz[1] = gz[0];
  vx[1] = vx[0];
  vy[1] = vy[0];
  vz[1] = vz[0];
  //controle de uso de print
  count++;
  if(count%25==0){
    /*
    Serial.print(g);
    Serial.print(':');
    Serial.print(b);
    Serial.print(':');
    Serial.print(a);
    Serial.print(':');
    Serial.print(dx);
    Serial.print(':');
    Serial.print(dy);
    Serial.print(':');
    Serial.println(dz);
    */
    //Serial.print("gravidade:");
    //Serial.print(mag_g);
    //Serial.print(",");
    //Serial.print("gamma:");
    Serial.print(g);
    Serial.print(":");
    //Serial.print("beta:");
    Serial.print(b);
    Serial.print(":");
    //Serial.print("alpha:");
    Serial.println(a);

  }
}
