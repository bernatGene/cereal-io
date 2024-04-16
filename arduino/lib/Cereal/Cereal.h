/*
  Cereal.h - Library for "shared-memory-like" serial comm.
  Created by Bernat Gené Skrabec, April, 2024.
  Released into the public domain.
*/
#ifndef Cereal_h
#define Cereal_h

#include "Arduino.h"

struct dataGram
{
  char start1;
  char start2;
  uint8_t CH;
  char DT;
  union
  {
    int32_t dataInt;
    float dataFloat;
  } data;
  char end1;
  char end2;
};

class Cereal
{
public:
  Cereal(int baudRate);
  void begin();
  void sendInt(int channel, int32_t value);
  int readInt(int channel);
  float readFloat(int channel);
  void sendFloat(int channel, float value);
  void readCereal();
  void passiveListen(int miliseconds);
  int osc = 0;
  dataGram floatToDatagram(char channel, char type, float value)
  {
    dataGram datagram;
    datagram.start1 = (uint8_t)'B';
    datagram.start2 = (uint8_t)'B';
    datagram.CH = channel;
    datagram.DT = type;
    datagram.data.dataFloat = value;
    datagram.end1 = (uint8_t)'E';
    datagram.end2 = (uint8_t)'E';
    return datagram;
  }
  dataGram intToDatagram(char channel, char type, int32_t value)
  {
    dataGram datagram;
    datagram.start1 = (uint8_t)'B';
    datagram.start2 = (uint8_t)'B';
    datagram.CH = channel;
    datagram.DT = type;
    datagram.data.dataInt = value;
    datagram.end1 = (uint8_t)'E';
    datagram.end2 = (uint8_t)'E';
    return datagram;
  }

private:
  int _baudRate;
  int32_t _intChannels[128];
  float _floatChannels[128];
  char buffer[8];
  size_t buffer_i = 0;
};

#endif