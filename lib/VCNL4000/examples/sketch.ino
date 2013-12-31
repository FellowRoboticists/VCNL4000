// -*- c -*-
// Example sketch for talking to the VCNL4000 i2c proximity/light sensor
// Written by Adafruit! Public domain.
//
// Class organization: Copyright (c) 2013 Dave Sieh
// See LICENSE.txt for details.
//
// To use: Connect VCC to 3.3-5V (5V is best if it is available), GND to
//         ground, SCL to i2c clock (on classic arduinos, Analog 5), SDA
//         to i2c data (on classic arduinos Analog 4). The 3.3v pin is
//         an ouptut if you need 3.3V
// This sensor is 5V compliant so you can use it with 3.3 or 5V micros

#include <VCNL4000.h>

VCNL4000 proximitySensor;
boolean state = false;

void setup() {
  Serial.begin(9600);

  proximitySensor.begin();
}

void loop() {
  uint16_t distance = proximitySensor.readProximity();
  uint16_t ambient = proximitySensor.readAmbient();

  Serial.print("Distance to target: ");
  Serial.println(distance);
  Serial.print("Ambient light level: ");
  Serial.println(ambient);

  delay(100);
}
