#include <Column.h>
#include "org/esb/util/Decimal.h"
#include "org/esb/util/Date.h"
#include "org/esb/util/Time.h"
#include "org/esb/util/Datetime.h"
#include <iostream>
using namespace org::esb::util;
namespace org{
namespace esb{
namespace sql{

Column::Column(MYSQL_BIND & b):bind(b){
//  reserve(0);
  buffer=NULL;
}

Column::Column(MYSQL_FIELD * field, MYSQL_BIND & b):bind(b){
  name=std::string(field->name);
  bind.buffer_type=field->type?field->type:MYSQL_TYPE_VAR_STRING;
  reserve(field->length);
  length=0;
//  buffer=NULL;
}

void Column::reserve(unsigned long size)
    {
      // At least for timestamp-data metadata returns a too small size-value,
      // so we grow it to at least 64 bytes. I don't know if this is a bug in
      // mysql or here.
      if (size < 64)
        size = 64;

      if (bind.buffer_length < size)
      {
//        log_debug("grow buffer to " << size << " initial " << bind.buffer_length);
//        if(buffer)
  //        delete[] buffer;
        buffer = new char[size];
        memset(buffer,0,size);
        bind.buffer=buffer;
        bind.buffer_length = size;
        bind.length=&length;
//        length=0;
//        bind.is_null=&is_null;
        bind.error=&error;
//        is_null=0;
      }
    }

std::string Column::getName(){
  return name;
}
Column::~Column(){
      if(buffer)
        delete[] buffer;
//        delete[] static_cast<char*>(buffer);
}

bool Column::isNull()
    {
      return bind.buffer_type == MYSQL_TYPE_NULL
          || bind.is_null && *bind.is_null;
    }
    
std::string Column::getBlob()
    {
      std::string ret;
//      if (isNull(bind))
//        throw NullValue();

      switch (bind.buffer_type)
      {
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
          ret.assign(static_cast<const char*>(bind.buffer),
                             *bind.length);
          break;

        default:
//        log_error("type-error in getBlob, type=" << bind.buffer_type);
          throw SqlException("type-error in getBlob");
      }
      return ret;
    }

std::string Column::getString()
    {
      std::string ret;
//      if (isNull(bind))
//        throw NullValue();

      switch (bind.buffer_type)
      {
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
          ret.assign(static_cast<const char*>(bind.buffer),
                             *bind.length);
          break;

        case MYSQL_TYPE_DATE:
        {
          MYSQL_TIME* ts = static_cast<MYSQL_TIME*>(bind.buffer);
          ret.assign(Date(ts->year, ts->month, ts->day).getIso());
          break;
        }

        case MYSQL_TYPE_TIME:
        {
          MYSQL_TIME* ts = static_cast<MYSQL_TIME*>(bind.buffer);
          ret.assign(Time(ts->hour, ts->minute, ts->second).getIso());
          break;
        }

        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_TIMESTAMP:
        {
          MYSQL_TIME* ts = static_cast<MYSQL_TIME*>(bind.buffer);
          ret.assign(Datetime(ts->year, ts->month, ts->day,
                          ts->hour, ts->minute, ts->second, ts->second_part).getIso());
          break;
        }

        default:
        {
          std::ostringstream s;
          switch (bind.buffer_type)
          {
            case MYSQL_TYPE_TINY:
            case MYSQL_TYPE_SHORT:
            case MYSQL_TYPE_INT24:
            case MYSQL_TYPE_LONG:
              if (bind.is_unsigned)
                s << getInteger<unsigned int>();
              else
                s << getInteger<int>();
              break;

            case MYSQL_TYPE_LONGLONG:
              if (bind.is_unsigned)
                s << *static_cast<long long unsigned*>(bind.buffer);
              else
                s << *static_cast<long long int*>(bind.buffer);
              break;

            case MYSQL_TYPE_FLOAT:
              s << *static_cast<float*>(bind.buffer);
              break;

            case MYSQL_TYPE_DOUBLE:
              s << *static_cast<double*>(bind.buffer);
              break;

            default:
//              log_error("type-error in getString, type=" << bind.buffer_type);
              throw SqlException("type-error in getString");
          }
          ret.assign(s.str());
        }
      }
      return ret;
    }


bool Column::getBool(){
  return getInteger<bool>();
}

int Column::getInt(){
  return getInteger<int>();
}

float Column::getFloat(){
  return getFloat<float>();
}

double Column::getDouble(){
  return getFloat<double>();
}




    template <typename c_type>
    void Column::setValue(c_type value, enum_field_types mysql_type){
      reserve(sizeof(c_type));
      *static_cast<c_type*>(bind.buffer) = value;
      bind.buffer_type = mysql_type;
      bind.is_null = 0;
    }

    template <typename int_type>
    int_type Column::getInteger()
    {
//      if (isNull(bind))
//        throw NullValue();

      switch (bind.buffer_type)
      {
        case MYSQL_TYPE_TINY:
          if (bind.is_unsigned)
            return *static_cast<unsigned char*>(bind.buffer);
          else
            return *static_cast<signed char*>(bind.buffer);

        case MYSQL_TYPE_SHORT:
          if (bind.is_unsigned)
            return *static_cast<unsigned short int*>(bind.buffer);
          else
            return *static_cast<short int*>(bind.buffer);

        case MYSQL_TYPE_INT24:
        {
          unsigned char* ptr = reinterpret_cast<unsigned char*>(bind.buffer);
          if (bind.is_unsigned)
          {
#if __BYTE_ORDER == __LITTLE_ENDIAN
            return (static_cast<int_type>(ptr[0]))
                 + (static_cast<int_type>(ptr[1]) << 8)
                 + (static_cast<int_type>(ptr[2]) << 16);
#else
            return (static_cast<int_type>(ptr[2]))
                 + (static_cast<int_type>(ptr[1]) << 8)
                 + (static_cast<int_type>(ptr[0]) << 16);
#endif
          }
          else
          {
#if __BYTE_ORDER == __LITTLE_ENDIAN
            if (ptr[2] < 128)
            {
              return static_cast<int_type>(ptr[0])
                  + (static_cast<int_type>(ptr[1]) << 8)
                  + (static_cast<int_type>(ptr[2]) << 16);
            }
            else
            {
              int32_t val;
              unsigned char* vptr = reinterpret_cast<unsigned char*>(&val);
              vptr[0] = ptr[0];
              vptr[1] = ptr[1];
              vptr[2] = ptr[2];
              vptr[3] = '\xff';
              return static_cast<int_type>(val);
            }
#else
            if (ptr[2] < 128)
            {
              return (static_cast<int_type>(ptr[2]))
                   + (static_cast<int_type>(ptr[1]) << 8)
                   + (static_cast<int_type>(ptr[0]) << 16);
            }
            else
            {
              int32_t val;
              unsigned char* vptr = reinterpret_cast<unsigned char*>(&val);
              vptr[0] = '\xff';
              vptr[1] = ptr[0];
              vptr[2] = ptr[1];
              vptr[3] = ptr[2];
              return static_cast<int_type>(val);
            }
#endif
          }
        }

        case MYSQL_TYPE_LONG:
          if (bind.is_unsigned)
            return *static_cast<unsigned int*>(bind.buffer);
          else
            return *static_cast<int*>(bind.buffer);

        case MYSQL_TYPE_LONGLONG:
          if (bind.is_unsigned)
            return *static_cast<long long unsigned*>(bind.buffer);
          else
            return *static_cast<long long int*>(bind.buffer);

        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
        {
//          throw SqlException("type-error in getInteger: Decimal not supported at the moment!!!");
    
          std::string data(static_cast<char*>(bind.buffer), *bind.length);


//          logdebug("extract integer-type from decimal \"" << data << '"');
          std::istringstream in(data);
          Decimal decimal;
          decimal.read(in);
          if (in.eof() || !in.fail())
          {
            int_type ret = decimal.getInteger<int_type>();
            return ret;
          }
//          logerror("type-error in getInteger, type=" << bind.buffer_type);
          throw SqlException("type-error in getInteger");      
        }
          
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        {
          std::string data(static_cast<char*>(bind.buffer), *bind.length);
//          logdebug("extract integer-type from string \"" << data << '"');
          std::istringstream in(data);
          int_type ret;
          in >> ret;
          if (in.eof() || !in.fail())
            return ret;

          // no break!!!
        }

        default:
//          logerror("type-error in getInteger, type=" << bind.buffer_type);
          throw SqlException("type-error in getInteger");
      }
    }
    template <typename float_type>
    float_type Column::getFloat()
    {
//      if (isNull(bind))
//        throw NullValue();

      switch (bind.buffer_type)
      {
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
          return getInteger<int>();

        case MYSQL_TYPE_FLOAT:
          return *static_cast<float*>(bind.buffer);

        case MYSQL_TYPE_DOUBLE:
          return *static_cast<double*>(bind.buffer);

        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        {
          std::string data(static_cast<char*>(bind.buffer), *bind.length);
//          log_debug("extract float-type from string \"" << data << '"');
          std::istringstream in(data);
          float_type ret;
          in >> ret;
          if (in.eof() || !in.fail())
            return ret;

          // no break!!!
        }

        default:
//          log_error("type-error in getFloat, type=" << bind.buffer_type);
          throw SqlException("type-error in getFloat");
      }
    }

void Column::setString(const char* data){
  length = ::strlen(data);
  reserve(length + 1);
  memcpy(static_cast<char*>(bind.buffer), data, length + 1);
  bind.buffer_type = MYSQL_TYPE_VAR_STRING;
}

void Column::setString(const std::string& data){
  length = data.size();
  reserve(length);
  data.copy(static_cast<char*>(bind.buffer), data.size());
  bind.buffer_type = MYSQL_TYPE_VAR_STRING;
}

void Column::setBlob(const std::string& data){
  length = data.size();
  reserve(length);
  memcpy(static_cast<char*>(bind.buffer), data.data(), length);
  bind.buffer_type = MYSQL_TYPE_BLOB;
}

void Column::setNull()
    {
//      release(bind);
      bind.buffer_type = MYSQL_TYPE_NULL;
    }

void Column::setDouble(double data){
      setValue(data, MYSQL_TYPE_DOUBLE);
}
void Column::setInt(int data)
    {
      setValue(data, MYSQL_TYPE_LONG);
      bind.is_unsigned = 0;
    }

}}}


