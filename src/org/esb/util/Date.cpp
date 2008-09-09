/* 
 * Copyright (C) 2005 Tommi Maekitalo
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Date.h>
#include "IllegalFormatException.h"
#include <cctype>

namespace org{
namespace esb{
namespace util{
std::string Date::getIso() const
  {
    // format YYYY-MM-DD
    //        0....+....1
    char ret[10];
    unsigned short n = year;
    ret[3] = '0' + n % 10;
    n /= 10;
    ret[2] = '0' + n % 10;
    n /= 10;
    ret[1] = '0' + n % 10;
    n /= 10;
    ret[0] = '0' + n % 10;
    ret[4] = '-';
    ret[5] = '0' + month / 10;
    ret[6] = '0' + month % 10;
    ret[7] = '-';
    ret[8] = '0' + day / 10;
    ret[9] = '0' + day % 10;

    return std::string(ret, 10);
  }

  inline unsigned short getNumber2(const char* s)
  {

    if (!isdigit(s[0])
      || !isdigit(s[1]))
      throw IllegalFormatException("Date::getNumber2");

    return (s[0] - '0') * 10
         + (s[1] - '0');
  }

  inline unsigned short getNumber4(const char* s)
  {
    if (!isdigit(s[0])
      || !isdigit(s[1])
      || !isdigit(s[2])
      || !isdigit(s[3]))
      throw IllegalFormatException("Date::getNumber4");
    return (s[0] - '0') * 1000
         + (s[1] - '0') * 100
         + (s[2] - '0') * 10
         + (s[3] - '0');
  }

  Date Date::fromIso(const std::string& s)
  {
    if (s.size() < 10
      || s.at(4) != '-'
      || s.at(7) != '-')
      throw IllegalFormatException("Date::fromIso");
    const char* d = s.data();
    return Date(getNumber4(d), getNumber2(d + 5), getNumber2(d + 8));
  }

}
}
}

