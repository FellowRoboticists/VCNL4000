#include <Arduino.h>
#include <Wire.h>
#include <pspc_support.h>
#include "VCNL4000.h"

VCNL4000::VCNL4000() {
}

void VCNL4000::begin() {
  Wire.begin();

  uint8_t rev = read8(VCNL4000_PRODUCTID);
  
  if ((rev & 0xF0) != 0x10) {
    Serial.println(P("Sensor not found :("));
    while (1);
  }

  write8(VCNL4000_IRLED, 20);        // set to 20 * 10mA = 200mA
  Serial.print(P("IR LED current = "));
  Serial.print(read8(VCNL4000_IRLED) * 10, DEC);
  Serial.println(P(" mA"));

  //write8(VCNL4000_SIGNALFREQ, 3);
  Serial.print(P("Proximity measurement frequency = "));
  uint8_t freq = read8(VCNL4000_SIGNALFREQ);
  if (freq == VCNL4000_3M125) Serial.println(P("3.125 MHz"));
  if (freq == VCNL4000_1M5625) Serial.println(P("1.5625 MHz"));
  if (freq == VCNL4000_781K25) Serial.println(P("781.25 KHz"));
  if (freq == VCNL4000_390K625) Serial.println(P("390.625 KHz"));
  
  write8(VCNL4000_PROXINITYADJUST, 0x81);
  Serial.print(P("Proximity adjustment register = "));
  Serial.println(read8(VCNL4000_PROXINITYADJUST), HEX);
  
  // arrange for continuous conversion
  //write8(VCNL4000_AMBIENTPARAMETER, 0x89);

}

uint16_t VCNL4000::readProximity() {
  write8(VCNL4000_COMMAND, VCNL4000_MEASUREPROXIMITY);
  while (1) {
    uint8_t result = read8(VCNL4000_COMMAND);
    //Serial.print(P("Ready = 0x")); Serial.println(result, HEX);
    if (result & VCNL4000_PROXIMITYREADY) {
      return read16(VCNL4000_PROXIMITYDATA);
    }
    delay(10);
  }
}

uint16_t VCNL4000::readAmbient() {
  write8(VCNL4000_COMMAND, VCNL4000_MEASUREAMBIENT);
  while (1) {
    uint8_t result = read8(VCNL4000_COMMAND);
    //Serial.print(P("Ready = 0x")); Serial.println(result, HEX);
    if (result & VCNL4000_AMBIENTREADY) {
      return read16(VCNL4000_AMBIENTDATA);
    }
    delay(10);
  }
}

// Read 1 byte from the VCNL4000 at 'address'
uint8_t VCNL4000::read8(uint8_t address)
{
  uint8_t data;

  Wire.beginTransmission(VCNL4000_ADDRESS);
#if ARDUINO >= 100
  Wire.write(address);
#else
  Wire.send(address);
#endif
  Wire.endTransmission();

  delayMicroseconds(170);  // delay required

  Wire.requestFrom(VCNL4000_ADDRESS, 1);
  while(!Wire.available());

#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}


// Read 2 byte from the VCNL4000 at 'address'
uint16_t VCNL4000::read16(uint8_t address)
{
  uint16_t data;

  Wire.beginTransmission(VCNL4000_ADDRESS);
#if ARDUINO >= 100
  Wire.write(address);
#else
  Wire.send(address);
#endif
  Wire.endTransmission();

  Wire.requestFrom(VCNL4000_ADDRESS, 2);
  while(!Wire.available());
#if ARDUINO >= 100
  data = Wire.read();
  data <<= 8;
  while(!Wire.available());
  data |= Wire.read();
#else
  data = Wire.receive();
  data <<= 8;
  while(!Wire.available());
  data |= Wire.receive();
#endif
  
  return data;
}

// write 1 byte
void VCNL4000::write8(uint8_t address, uint8_t data)
{
  Wire.beginTransmission(VCNL4000_ADDRESS);
#if ARDUINO >= 100
  Wire.write(address);
  Wire.write(data);  
#else
  Wire.send(address);
  Wire.send(data);  
#endif
  Wire.endTransmission();
}
