/*
 */

#include "Arduino.h"
#include "Cereal.h"
#include <string>

Cereal::Cereal(int baudRate)
{
  _baudRate = baudRate;
}

void Cereal::begin()
{
  if (!Serial)
  {
    Serial.begin(_baudRate);
  }
  else 
  {
    Serial.println("Serial already started, baud rate might not match");
  }
}

void Cereal::readCereal()
{
  if (!Serial or Serial.available() == 0)
  {
    return;
  }
}


void Cereal::sendInt(int channel, int value)
{
  std::array<uint8_t, 10> datagram = toDatagram(channel, 'I', value);
  for (int i = 0; i < 10; i++)
  {
    Serial.write(datagram[i]);
  }
}

void Cereal::sendFloat(int channel, float value)
{
  std::array<uint8_t, 10> datagram = toDatagram(channel, 'F', value);
  for (int i = 0; i < 10; i++)
  {
    Serial.write(datagram[i]);
  }
}
