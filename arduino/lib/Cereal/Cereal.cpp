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
  osc = osc == 99 ? 55 : 99;
  sendInt(99, osc);
  if (!Serial)
  {
    sendInt(99, 5);
    return;
  }
  int bytesToRead = Serial.available();
  if (bytesToRead == 0)
  {
    Serial.read();
    osc = osc == 1 ? 2 : 1;
    sendInt(99, osc);
    return;
  }
  sendInt(99, 2);
  uint8_t buff[bytesToRead];
  int read = Serial.readBytes(buff, bytesToRead);
  if (read > 0)
  {
    osc = osc == 4 ? 3 : 4;
    sendInt(99, osc);
    somethingCame = true;
    _handleBuffer(buff, bytesToRead);
  }
}

void Cereal::_handleBuffer(uint8_t *newBuffer, int numBytes)
{
  int totalBytes = numBytes + _broken.size();
  uint8_t buffer[totalBytes];
  for (uint i = 0; i < _broken.size(); i++)
  {
    buffer[i] = _broken[i];
  }
  memcpy(&buffer + _broken.size(), &newBuffer, numBytes);
  for (int i = 0; i < totalBytes; i++)
  {
    if (totalBytes - i < 10) // not enough left to contain a datagram
    {
      _broken.clear();
      for (int j = 0; (i + j) < totalBytes; j++)
      {
        _broken.push_back(buffer[i + j]);
      }
      return;
    }
    if (buffer[i] != (uint8_t)'B')
    { // not the start of a datagram
      continue;
    }
    bool B2 = buffer[i + 1] == (uint8_t)'B';
    bool E1 = buffer[i + 8] == (uint8_t)'E';
    bool E2 = buffer[i + 9] == (uint8_t)'E';
    if (!(B2 && E1 && E2)) // datagram is malformed; continue
    {
      continue;
    }
    char datatype = buffer[i + 3];
    uint8_t channel = buffer[i + 2];
    i += 9;
    _setChannelValue(datatype, channel, &buffer[i + 4]);
  }
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

void Cereal::sendInt(int channel, int value)
{
  std::array<uint8_t, 10> datagram = toDatagram(channel, 'I', value);
  for (int i = 0; i < 10; i++)
  {
    Serial.write(datagram[i]);
  }
}

int Cereal::readInt(int channel)
{
  return _intChannels[channel];
}

void Cereal::sendFloat(int channel, float value)
{
  std::array<uint8_t, 10> datagram = toDatagram(channel, 'F', value);
  for (int i = 0; i < 10; i++)
  {
    Serial.write(datagram[i]);
  }
}
