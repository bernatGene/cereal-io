#include <array>
#include "Cereal.h"

Cereal cereal(1024);
// Morse morse(LED_BUILTIN);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // put your setup code here, to run once:
  digitalWrite(LED_BUILTIN, LOW);
}

uint read;
bool last = false;

void loop()
{
  cereal.readCereal();
  // put your main code here, to run repeatedly:
  if (cereal.readInt(1) > 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (cereal.somethingCame)
  {
    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(20);
      digitalWrite(LED_BUILTIN, LOW);
      delay(20);
    }
    cereal.somethingCame = false;
  }
  else
  {
    digitalWrite(LED_BUILTIN, last);
    last = !last;
  }
  delay(500);
}
