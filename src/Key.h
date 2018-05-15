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

#ifndef KEY_H
#define KEY_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <stdint.h>

class Key : public Printable {
public:
  Key();
  
  size_t printTo(Print& p) const;
  String toString() const;
  bool fromString(const char* key);
  bool operator==(const Key& key) const {
    for (uint8_t i = 0; i < 8;i++)
      if (data[i] != key.data[i]) return false;
    return true;
  }
private:
  uint8_t data[8];
  friend class IButton;
};

#endif KEY_H
