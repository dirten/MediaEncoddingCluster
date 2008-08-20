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

#ifndef ORG_ESB_UTIL_DATETIME_H
#define ORG_ESB_UTIL_DATETIME_H

#include <string>

namespace org{
namespace esb{
namespace util{
  /**
   * This class holds a date and a time.
   */
  class Datetime
  {
      unsigned short year;
      unsigned short month;
      unsigned short day;
      unsigned short hour;
      unsigned short minute;
      unsigned short second;
      unsigned short millis;

    public:
      /// Initializes the Datetime-object with empty values.
      Datetime()
        : year(0),
          month(0),
          day(0),
          hour(0),
          minute(0),
          second(0),
          millis(0)
          { }

      /// Initializes the Datetime-object with the given values.
      /// No range-checks are done.
      Datetime(unsigned short year_,
               unsigned short month_,
               unsigned short day_,
               unsigned short hour_,
               unsigned short minute_,
               unsigned short second_,
               unsigned short millis_ = 0)
        : year(year_),
          month(month_),
          day(day_),
          hour(hour_),
          minute(minute_),
          second(second_),
          millis(millis_)
          { }

      /// Returns the year-part of the Datetime.
      unsigned short getYear() const     { return year; }
      /// Returns the month-part of the Datetime.
      unsigned short getMonth() const    { return month; }
      /// Returns the day-part of the Datetime.
      unsigned short getDay() const      { return day; }
      /// Returns the hour-part of the Datetime.
      unsigned short getHour() const     { return hour; }
      /// Returns the minute-part of the Datetime.
      unsigned short getMinute() const   { return minute; }
      /// Returns the second-part of the Datetime.
      unsigned short getSecond() const   { return second; }
      /// Returns the millisecond-part of the Datetime.
      unsigned short getMillis() const   { return millis; }

      bool isNull() const  { return month == 0; }

      /// Returns the date in ISO-format (yyyy-mm-dd hh:mm:ss.hhh)
      std::string getIso() const;
      /// Interprets the passed string as a datetime-string in ISO-format (yyyy-mm-dd hh:mm:ss.hhh) and returns a Datetime-object.
      /// When the string is not in ISO-format, a exception of type tntdb::TypeError is thrown.
      static Datetime fromIso(const std::string& s);

      /// Sets the date and the time.
      /// No range-checks are done.
      void set(unsigned short year_,
               unsigned short month_,
               unsigned short day_,
               unsigned short hour_,
               unsigned short minute_,
               unsigned short second_,
               unsigned short millis_ = 0)
      {
        year = year_;
        month = month_;
        day = day_;
        hour = hour_;
        minute = minute_;
        second = second_;
        millis = millis_;
      }

      bool operator== (const Datetime& dt) const
      { return year == dt.year
            && month == dt.month
            && day == dt.day
            && hour == dt.hour
            && minute == dt.minute
            && second == dt.second
            && millis == dt.millis; }

      bool operator!= (const Datetime& dt) const
        { return !operator==(dt); }

      bool operator< (const Datetime& dt) const
      { return year < dt.year
            || year == dt.year
             && ( month < dt.month
               || month == dt.month
                && ( day < dt.day
                  || day == dt.day
                   && ( hour < dt.hour
                     || hour == dt.hour
                      && ( minute < dt.minute
                        || minute == dt.minute
                         && ( second < dt.second
                           || second == dt.second
                            && millis < dt.millis
                            )
                         )
                       )
                    )
                ); }

      bool operator> (const Datetime& dt) const
        { return dt < *this; }

      bool operator<= (const Datetime& dt) const
        { return !(*this > dt); }

      bool operator>= (const Datetime& dt) const
        { return !(*this < dt); }
  };
}
}
}
#endif // TNTDB_DATETIME_H

