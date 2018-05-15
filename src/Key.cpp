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

#include "Key.h"

#include <string.h>
#include <Printable.h>

Key::Key() {
  memset(data,0,8);
}

size_t Key::printTo(Print& p) const {
  size_t n = 0;
  for (uint8_t i = 0; i < 7; i++) {
    if (data[i] < 0x10) {
      n += p.print('0');
      n += p.print(data[i], HEX);
    } else {
      n += p.print(data[i], HEX);    
    }
    n += p.print(':');
  }
  if (data[7] < 0x10) {
    n += p.print('0');
    n += p.print(data[7], HEX);
  } else {
    n += p.print(data[7], HEX);    
  }
  return n;
}

bool Key::fromString(const char* key)
{
  if (strlen(key) != 23)
    return false;
  
  char temp[3] = {'\0','\0','\0'};
  for (byte i = 1; i < 9; i++) {
    temp[0] = key[i*3 - 3];
    temp[1] = key[i*3 - 2];
    data[i-1] = strtol(temp, NULL, 16);
  }
  return true;
}
