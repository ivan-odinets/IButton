/*
 * ----------------------------------------------------------------------------
 *
 *  This file is part of iButton library
 * 
 *  iButton library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  iButton library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with iButton library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "IButton.h"

#ifdef DEBUG
  #define DEBUG_PREFIX "[IButtonMonitor::DEBUG] "
  #define debug(c) {Serial.print(DEBUG_PREFIX);Serial.println(c);}
#else
  #define debug(c) {}
#endif

/*
 * ----------------------------------------------------------------------------
 * Reading/writing rw1990 keys
 * 
 */

bool IButton::readKey(Key* buffer)
{
  reset();
  reset_search();
  return search(buffer->data);
}
void IButton::writeKey(Key* buffer)
{
  skip();
  reset();
  write(0xD1);

  writeBit(false);
  skip();
  reset();
  write(0xD5);
  for (byte x = 0; x < 8; x++)
    writeByte(buffer->data[x]);
  
  reset();
  write(0xD1);
  writeBit(true);
}
void IButton::writeByte(byte data) 
{
  byte data_bit;
  for (data_bit = 0; data_bit < 8; data_bit++) {
    writeBit(!(data & 1));
    data = data >> 1;
  }
}
void IButton::writeBit(bool bit)
{
  digitalWrite(m_pin,LOW);pinMode(m_pin,OUTPUT);delayMicroseconds(((bit) ? 10 : 60));
  pinMode(m_pin,INPUT);digitalWrite(m_pin,HIGH);delay(10);
}

/*
 * ----------------------------------------------------------------------------
 * Reading Cyfral keys
 * 
 * For correct reading of Cyfral keys prescaling needs to be set
 * ----------------------------------------------------------------------------
 * Prescale   ADPS2   ADPS1   ADPS0   Clock freq (MHz)   Sampling rate (KHz)
 * ----------------------------------------------------------------------------
 * 2          0       0       1       8                  615
 * 4          0       1       0       4                  307
 * 8          0       1       1       2                  153
 * 16         1       0       0       1                  76.8
 * 32         1       0       1       0.5                38.4
 * 64         1       1       0       0.25               19.2
 * 128        1       1       1       0.125              9.6
 *
 */

#ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define TIMING 50
constexpr int MIN_READING = 1023*710.0/(RESISTOR+710.0);
constexpr int MAX_READING = 1023*1335.0/(RESISTOR+1335.0);

//710  => 260
//1335 => 400
bool IButton::readCyfral(CyfralKey* k)
{
  debug("Reading Cyfral key");
  uint8_t startTime = 0;
  
  uint8_t buffer[100];
  if (analogRead(m_pin) > 1000) { //No key connected
    debug("No Cyfral key found.");
    return false;
  }
  
  //Required for propper connection
  debug("Setting ACP prescaler.");
  uint8_t ADCSRA_old = ADCSRA;
  cbi(ADCSRA,ADPS2);
  sbi(ADCSRA,ADPS1);
  cbi(ADCSRA,ADPS0);  
  uint8_t i = 0;
  do {
    while (!LOW_RESISTANCE(analogRead(A0))) {}
    startTime = micros();
    while (LOW_RESISTANCE(analogRead(A0))) {}
    buffer[i++] = micros() - startTime;
  } while (i < sizeof(buffer));
  
  //Restoring defaults
  debug("Restoring ACP prescaler.");
  ADCSRA = ADCSRA_old;
  
  #ifdef DEBUG
  String data="Recieved=";
  for (uint8_t i = 0; i < sizeof(buffer); i++) {
    data+= (toLogical(buffer[i])) ? '1' : '0';
  }
  debug(data);
  #endif
  
  int8_t startpos = -1;
  for (uint8_t i = 0; i < sizeof(buffer) - CYFRAL_SIZE;i++) { //Searching for the start sequence 0111
    if (!toLogical(buffer[i]) && toLogical(buffer[i+1])  && toLogical(buffer[i+2]) && toLogical(buffer[i+3])) {
      startpos = i;
      break;
    }
  }

  if (startpos == -1) {
    debug("No start sequence 0111 present. Aborting.");
    return false;
  }
      
  for (i = 0; i < CYFRAL_SIZE; i++)
    k->data[i] = toLogical(buffer[i+startpos]);

  debug("Cyfral successfully readed!");
  return true;
}

bool IButton::LOW_RESISTANCE(int reading) {
  return ((reading > MIN_READING) && (reading < MAX_READING));
}
bool IButton::toLogical(uint8_t timing) {
    return timing < TIMING;
}

/*
 * ----------------------------------------------------------------------------
 * Reading resistance of the key
 * 
 */

uint16_t IButton::getResistance() const
{
    float x = 1023.0 / analogRead(m_pin) - 1;
    return RESISTOR/x;
}
