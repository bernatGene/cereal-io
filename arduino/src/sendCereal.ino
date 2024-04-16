#include <Arduino_LSM6DS3.h>
// #include <Servo.h>
#include "Cereal.h"

Cereal cereal(1024);

// Servo myservo;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // put your setup code here, to run once:

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  // myservo.attach(9);
}

float x, y, z; 
uint count;

void loop() {
  // put your main code here, to run repeatedly:
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  }
  count += 1;
  count %= 180;
  cereal.sendInt(0, count);

  cereal.sendFloat(1, x);
  cereal.sendFloat(2, y);
  cereal.sendFloat(3, z);

  // Serial.println("Just hanging around");
  // myservo.write(count);
  digitalWrite(LED_BUILTIN, (count%2) == 0);
  delay(500);
}
