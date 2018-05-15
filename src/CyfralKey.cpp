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

#include "CyfralKey.h"

CyfralKey::CyfralKey()
{}

size_t CyfralKey::printTo(Print& p) const {
  size_t n = 0;
  uint8_t i = 0;
  for (i = 0; i < 8*4; i+=4) {
    for (uint8_t j = 0; j < 4;j++)
      n+= p.print((data[i+j]) ? "1" : "0");

    n += p.print(":");
  }

  for (uint8_t j = 0; j < 4;j++)
    n+= p.print((data[i+j]) ? "1" : "0");
  
  return n;
}
