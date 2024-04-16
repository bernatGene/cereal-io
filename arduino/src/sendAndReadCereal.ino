#include <Servo.h>
#include "Cereal.h"

Cereal cereal(9600);
Servo servo;


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  cereal.begin();
  servo.attach(9);
  // Init fase
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(LED_BUILTIN, (i % 20) == 0);
    servo.write(i%20);
    delay(30);
  }
}

float x, y, z = 0;
long count = 0;

void loop()
{
  count += 1;
  count %= 10;
  cereal.sendInt(0, count);

  x += .1;
  y += .002;
  z += .003;

  cereal.sendFloat(1, x);
  cereal.sendFloat(2, y);
  cereal.sendFloat(3, z);
  digitalWrite(LED_BUILTIN, (count % 2) == 0);
  cereal.passiveListen(500);
  int32_t readVal = cereal.readInt(1);
  float readValFloat = cereal.readFloat(2);
  // servo.write(readVal);
  servo.write(readValFloat);
}
