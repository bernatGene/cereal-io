/*
 */

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
  int bytesToRead = Serial.available();
  if (bytesToRead == 0)
  {
    Serial.read();
    return;
  }
  uint8_t buff[bytesToRead];
  int read = Serial.readBytes(buff, bytesToRead);
  if (read > 0)
  {
    _handleBuffer(buff, bytesToRead);
  }
}

void Cereal::_handleBuffer(uint8_t *newBuffer, int numBytes)
{
  int totalBytes = numBytes + _brokenBits;
  char *buffer;
  buffer = (char *)malloc(totalBytes);
  // uint8_t buffer[totalBytes];
  // for (uint i = 0; i < _brokenBits; i++)
  // {
  //   buffer[i] = _broken[i];
  // }
  // memcpy(&buffer + _broken.size(), &newBuffer, numBytes);
  // for (int i = 0; i < totalBytes; i++)
  // {
  //   if (totalBytes - i < 10) // not enough left to contain a datagram
  //   {
  //     _broken.clear();
  //     for (int j = 0; (i + j) < totalBytes; j++)
  //     {
  //       _broken.push_back(buffer[i + j]);
  //     }
  //     return;
  //   }
  //   if (buffer[i] != (uint8_t)'B')
  //   { // not the start of a datagram
  //     continue;
  //   }
  //   bool B2 = buffer[i + 1] == (uint8_t)'B';
  //   bool E1 = buffer[i + 8] == (uint8_t)'E';
  //   bool E2 = buffer[i + 9] == (uint8_t)'E';
  //   if (!(B2 && E1 && E2)) // datagram is malformed; continue
  //   {
  //     continue;
  //   }
  //   char datatype = buffer[i + 3];
  //   uint8_t channel = buffer[i + 2];
  //   i += 9;
  //   _setChannelValue(datatype, channel, &buffer[i + 4]);
  // }
  free(buffer);
}

void Cereal::_setChannelValue(char datatype, uint8_t channel, uint8_t *data)
{
  if (channel > 128)
  {
    return; // bad channel
  }
  if (datatype == 'I')
  {
    union
    {
      int value;
      const uint8_t *p;
    };
    p = data;
    _intChannels[channel] = value;
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
    delay(5);
  }
}

int Cereal::readInt(int channel)
{
  return _intChannels[channel];
}
