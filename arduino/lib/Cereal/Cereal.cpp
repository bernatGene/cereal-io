#include "Arduino.h"
#include "Cereal.h"

Cereal::Cereal(int baudRate)
{
  _baudRate = baudRate;
}

void Cereal::begin()
{
  Serial.begin(_baudRate);
  int wait = 0;
  while (!Serial)
  {
    digitalWrite(LED_BUILTIN, (wait % 2) == 0);
    delay(20);
  }
  Serial.println("Serial started");
}

void Cereal::readCereal()
{
  if (!Serial)
  {
    return;
  }
  while (Serial.available() > 0 || buffer_i == 8)
  {
    char inByte;
    inByte = Serial.read();

    if (inByte != 'B' && buffer_i == 0) // wait for the start of a packet
    {
      continue;
    }
    if (buffer_i < 8) // store the packet into buffer
    {
      buffer[buffer_i++] = inByte;
    }
    else // buffer is full, let's read
    {
      bool start = (buffer[0] == 'B');
      bool end = (buffer[7] == '\n');
      if (!(start && end)) // ensure packet is well formed
      {
        buffer_i = 0;
        return;
      }
      uint8_t channel = buffer[1];
      char datatype = buffer[2];
      union // union to cast types. Assumes little-endian
      {
        int32_t ii;
        float ff;
        char data[4];
      } value;
      for (int i = 0; i < 4; i++)
      {
        value.data[i] = buffer[i + 3];
      }
      if (channel < 128) // ensure channel is valid
      {
        if (datatype == 'I')
        {
          _intChannels[channel] = value.ii;
        }
        else if (datatype == 'F')
        {
          _floatChannels[channel] = value.ff;
        }
      }
      buffer_i = 0; // reset the buffer.
    }
  }
}

void Cereal::sendInt(int channel, int32_t value)
{
  dataGram datagram = intToDatagram(channel, 'I', value);
  Serial.write(&datagram.start1, 10);
}

void Cereal::sendFloat(int channel, float value)
{
  dataGram datagram = floatToDatagram(channel, 'F', value);
  Serial.write(&datagram.start1, 10);
}

void Cereal::passiveListen(int miliseconds)
{
  unsigned long start = millis();
  while (millis() < (start + miliseconds))
  {
    readCereal();
  }
}

int Cereal::readInt(int channel)
{
  return _intChannels[channel];
}

float Cereal::readFloat(int channel)
{
  return _floatChannels[channel];
}

