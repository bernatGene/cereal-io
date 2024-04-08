#include <Arduino_LSM6DS3.h>
#include <array>
#include "Cereal.h"

Cereal cereal(1);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(1024);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

float x, y, z; 
uint count;

void loop() {
  // put your main code here, to run repeatedly:
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  }
  count += 1;
  cereal.sendInt(0, count);

  cereal.sendFloat(1, x);
  cereal.sendFloat(2, y);
  cereal.sendFloat(3, z);
  Serial.println("Just hanging around");
  delay(5);
}
