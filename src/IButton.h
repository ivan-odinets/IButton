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

#ifndef IBUTTON_H
#define IBUTTON_H

#include <OneWire.h>
#include "Key.h"

#ifndef RESISTOR
  #define RESISTOR 2080 //Ohm
#endif

#include "CyfralKey.h"

//Uncomment this to enable debug output
//#define DEBUG

class IButton : public OneWire {
public:
  IButton(byte pin) : OneWire(pin), m_pin(pin) {}
  
  bool keyConnected();
  bool readKey(Key* buffer);
  void writeKey(Key* buffer);

  bool readCyfral(CyfralKey* k);
  
  uint16_t getResistance() const;
private:
  byte m_pin;
  
  //For reading/writting rw1990
  void writeByte(byte);
  void writeBit(bool);
  
  //For reading/writing cyfral
  static inline bool LOW_RESISTANCE(int reading);
  static inline bool toLogical(uint8_t timing);
};

#endif //IBUTTON_H
