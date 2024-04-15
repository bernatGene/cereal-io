#include <array>
#include "Cereal.h"
#include "Morse.h"

Cereal cereal(1024);
Morse morse(LED_BUILTIN);

void setup()
{
  // put your setup code here, to run once:
  digitalWrite(LED_BUILTIN, LOW);
}

uint read;

void loop()
{
  // put your main code here, to run repeatedly:
  int available = Serial.available();
  if (available > 0)
  {
    uint8_t buffer[available];
    Serial.readBytes(buffer, available);
    for (int i = 0; i < available; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    morse.dash();
  }
  else {
      digitalWrite(LED_BUILTIN, LOW);
  }
  // cereal.sendFloat(2, 2.2);
  delay(5);
}
