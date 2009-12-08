

#ifndef ORG_ESB_UTIL_TIME_H
#define ORG_ESB_UTIL_TIME_H

#include <string>

namespace org{
namespace esb{
namespace util{
  /**
   * This class holds a time.
   */
  class Time
  {
      short hour;
      unsigned short minute;
      unsigned short second;
      unsigned short millis;

    public:
      /// Initializes the Time-object with empty values.
      Time()
        : hour(-1),
          minute(0),
          second(0),
          millis(0)
          { }

      /// Initializes the Time-object with the given values.
      /// No range-checks are done.
      Time(unsigned short hour_,
           unsigned short minute_,
           unsigned short second_,
           unsigned short millis_ = 0)
        : hour(hour_),
          minute(minute_),
          second(second_),
          millis(millis_)
          { }

      /// Returns the hour-part of the Time.
      unsigned short getHour() const     { return static_cast<unsigned short>(hour); }
      /// Returns the minute-part of the Time.
      unsigned short getMinute() const   { return minute; }
      /// Returns the second-part of the Time.
      unsigned short getSecond() const   { return second; }
      /// Returns the millisecond-part of the Time.
      unsigned short getMillis() const   { return millis; }

      bool isNull() const  { return hour < 0; }

      /// Sets the time.
      /// No range-checks are done.
      void set(unsigned short hour_,
               unsigned short minute_,
               unsigned short second_,
               unsigned short millis_ = 0)
      {
        hour = static_cast<short>(hour_);
        minute = minute_;
        second = second_;
        millis = millis_;
      }

      /// Returns the time in ISO-format (hh:mm:ss.hhh)
      std::string getIso() const;
      /// Interprets the passed string as a time-string in ISO-format (hh:mm:ss.hhh) and returns a Time-object.
      /// When the string is not in ISO-format, a exception of type tntdb::TypeError is thrown.
      static Time fromIso(const std::string& s);

      bool operator== (const Time& dt) const
      { return hour == dt.hour
            && minute == dt.minute
            && second == dt.second
            && millis == dt.millis; }

      bool operator!= (const Time& dt) const
        { return !operator==(dt); }

      bool operator< (const Time& dt) const
      { return hour < dt.hour
            || hour == dt.hour
             && ( minute < dt.minute
              || minute == dt.minute
               && ( second < dt.second
                 || second == dt.second
                  && millis < dt.millis
                  )
                ); }

      bool operator> (const Time& dt) const
        { return dt < *this; }

      bool operator<= (const Time& dt) const
        { return !(*this > dt); }

      bool operator>= (const Time& dt) const
        { return !(*this < dt); }
  };
}
}
}
#endif // TNTDB_TIME_H

