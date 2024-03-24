#include <Arduino_LSM6DS3.h>
#include "Morse.h"
#include <array>
#include "Cereal.h"

Morse morse(LED_BUILTIN);

Cereal cereal(9600);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  morse.begin();
}

template <typename T> void printBinWithLeading(Print &out, T n){
    for(byte i = sizeof(n) * 8 - 1; i--;){
    out.print(bitRead(n, i));
   
    if(i == 0){
      break;
    }
  }
}

float x, y, z; 

void printBytes(std::array<uint8_t, 10> datagram) {
  for (int i = 0; i < 10; i++)
  {
    Serial.print(i);
    Serial.print("\t");
    // Serial.print(datagram[i], BIN);
    printBinWithLeading(Serial, datagram[i]);
    Serial.print("\t\t");
    Serial.print("\t\t");
    Serial.print(datagram[i], HEX);
    Serial.print("\t\t");
    Serial.print(char(datagram[i]));
    Serial.print("\t\t");
    Serial.print(uint8_t(datagram[i]));
    Serial.println();
  }
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

  }
  int value = 255;
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.println(z);
  Serial.print(33.33, BIN);
  Serial.println();
  morse.dot();
  morse.dot();
  morse.dot();
  morse.dash();
  Serial.println("Sending datagram 1");
  std::array<uint8_t, 10> datagram = cereal.toDatagram(0, 'I', value);
  printBytes(datagram);
  cereal.sendInt(0, value);
  Serial.println();

  value = 255 + 512;
  Serial.println("Sending datagram 2");
  datagram = cereal.toDatagram(1, 'I', value);
  printBytes(datagram);
  cereal.sendInt(1, value);
  Serial.println();

  value = 255 + 512 + (1 << 25);
  Serial.println("Sending datagram 3");
  datagram = cereal.toDatagram(2, 'I', value);
  printBytes(datagram);
  cereal.sendInt(2, value);
  Serial.println();


  float float_value = 3.321;
  Serial.println("Sending datagram 4");
  datagram = cereal.toDatagram(2, 'I', float_value);
  printBytes(datagram);
  cereal.sendFloat(2, float_value);
  Serial.println();

  delay(100);
}
