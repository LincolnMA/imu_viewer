void setup() {
  Serial.begin(9600);
}

void loop() {
  for(float i = 0;i < 2*PI;i+=PI/100){
    for(float j = 0;j < 2*PI;j+=PI/100){
      for(float k = 0;k < 2*PI;k+=PI/100){
        Serial.print(i);
        Serial.print(':');
        Serial.print(j);
        Serial.print(':');
        Serial.println(k);
        delay(50);
      }
    }
  }

}
