/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Cereal_h
#define Cereal_h

#include "Arduino.h"
#include <array>
#include <string>
#include <typeinfo>

class Cereal
{
public:
  Cereal(int baudRate);
  void begin();
  void sendInt(int channel, int value);
  void sendFloat(int channel, float value);
  template <typename T>
  std::array<uint8_t, 10> toDatagram(char channel, char type, T value)
  {
    std::array<uint8_t, 10> datagram;
    datagram[0] = (uint8_t)'B';
    datagram[1] = (uint8_t)'B';
    datagram[2] = channel;
    datagram[3] = type;
    memcpy(&datagram[4], &value, sizeof(value));
    datagram[8] = (uint8_t)'E';
    datagram[9] = (uint8_t)'E';
    return datagram;
  }

private:
  int _baudRate;
};

#endif