#include "ResultSet.h"
#include "sstream"
#include <iostream>
#include <list>
using namespace org::esb::sql;
using namespace std;
namespace org{
namespace esb{
namespace sql{
/*******************************************************************************************************/
class Column{
    public:
	Column(MYSQL_FIELD & field){
	    type=field.type;
	    field_size=field.length;
//	    cout << "Allocate buffersize to:"<<field_size<<endl;
	    unsigned int flag=field.flags;
	    data=new char[field_size];
	    buffer_length=field_size;
	    length=field_size;
	    is_null=1;
	    is_error=0;
	}
	enum_field_types type;
	unsigned long field_size;
	void * data;
	unsigned long buffer_length;
	unsigned long length;
	my_bool is_null;
	my_bool is_error;
};
}}}

/*******************************************************************************************************/

ResultSet::ResultSet(MYSQL_STMT & stmt):_stmt(stmt){
    _resultSetMetadata=mysql_stmt_result_metadata(&_stmt);

    if (!_resultSetMetadata){
	fprintf(stderr," mysql_stmt_result_metadata(), returned no meta information\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(&_stmt));
        exit(0);
    }

    int _columnCount=mysql_num_fields(_resultSetMetadata);
    MYSQL_FIELD * fields=mysql_fetch_fields(_resultSetMetadata);
       
    _bindColumns=new MYSQL_BIND[_columnCount];
    memset(_bindColumns, 0, sizeof(MYSQL_BIND)*_columnCount);

    for(int i = 0; i < _columnCount; i++){
	Column * col=new Column(fields[i]);
	_row.push_back(col);
	_bindColumns[i].buffer_type=col->type;
	_bindColumns[i].buffer=col->data;
	_bindColumns[i].buffer_length=col->buffer_length;
	_bindColumns[i].length=&col->length;
	_bindColumns[i].is_null=&col->is_null;
	_bindColumns[i].error=&col->is_error;

//	printf("Field %u is %s\n", i, fields[i].name);
    }
    
    if (mysql_stmt_bind_result(&_stmt, _bindColumns)){
        fprintf(stderr, " mysql_stmt_bind_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(&_stmt));
        exit(0);
    }
}

/*******************************************************************************************************/
bool ResultSet::next(){

	int ret=mysql_stmt_fetch(&_stmt);
	if(ret==MYSQL_DATA_TRUNCATED){
	    allocateBigResult();
	}
	return ret!=MYSQL_NO_DATA;
}

/*******************************************************************************************************/
void ResultSet::allocateBigResult(){
    std::cout << "Allocate big result"<<std::endl;
}

/*******************************************************************************************************/
string ResultSet::getString(int col){//return static_cast<const char*>(_bindColumns[col].buffer);
    string ret;
    MYSQL_BIND bind=_bindColumns[col];
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
//          ret.assign(Date(ts->year, ts->month, ts->day).getIso());
          break;
        }

        case MYSQL_TYPE_TIME:
        {
          MYSQL_TIME* ts = static_cast<MYSQL_TIME*>(bind.buffer);
//          ret.assign(Time(ts->hour, ts->minute, ts->second).getIso());
          break;
        }

        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_TIMESTAMP:
        {
          MYSQL_TIME* ts = static_cast<MYSQL_TIME*>(bind.buffer);
//          ret.assign(Datetime(ts->year, ts->month, ts->day,
//                          ts->hour, ts->minute, ts->second, ts->second_part).getIso());
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
                s << getInteger<unsigned int>(bind);
              else
                s << getInteger<int>(bind);
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

    //        default:
    //          log_error("type-error in getString, type=" << bind.buffer_type);
    //          throw TypeError("type-error in getString");
          }
          ret.assign(s.str());
        }
      }
    return ret;
}

/*******************************************************************************************************/
	string ResultSet::getString(string col){return getString(getColumnIndex(col));}


/*******************************************************************************************************/
    template <typename int_type>
    int_type ResultSet::getInteger(const MYSQL_BIND& bind)
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
          std::string data(static_cast<char*>(bind.buffer), *bind.length);
//          log_debug("extract integer-type from decimal \"" << data << '"');
          std::istringstream in(data);
	/*
          Decimal decimal;
          decimal.read(in);
          if (in.eof() || !in.fail())
          {
            int_type ret = decimal.getInteger<int_type>();
            return ret;
          }
          */
//          log_error("type-error in getInteger, type=" << bind.buffer_type);
//          throw TypeError("type-error in getInteger");
        }
          
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        {
          std::string data(static_cast<char*>(bind.buffer), *bind.length);
//          log_debug("extract integer-type from string \"" << data << '"');
          std::istringstream in(data);
          int_type ret;
          in >> ret;
          if (in.eof() || !in.fail())
            return ret;

          // no break!!!
        }

//        default:
//          log_error("type-error in getInteger, type=" << bind.buffer_type);
//          throw TypeError("type-error in getInteger");
      }
    }

/*******************************************************************************************************/
    template <typename float_type>
    float_type ResultSet::getFloat(const MYSQL_BIND& bind)
    {

//      if (isNull(bind))
//        throw NullValue();

      switch (bind.buffer_type)
      {
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
          return getInteger<int>(bind);

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

//        default:
//          log_error("type-error in getFloat, type=" << bind.buffer_type);
//          throw TypeError("type-error in getFloat");
      }
    }





/*******************************************************************************************************/
    bool ResultSet::getBool(int index){return getInteger<bool>(_bindColumns[index]);}
    bool ResultSet::getBool(string index){return getBool(getColumnIndex(index));}

/*******************************************************************************************************/
    int ResultSet::getInt(int index){return getInteger<int>(_bindColumns[index]);}
    int ResultSet::getInt(string index){return getInt(getColumnIndex(index));}

/*******************************************************************************************************/
    unsigned ResultSet::getUnsigned(int index){return getInteger<unsigned>(_bindColumns[index]);}
    unsigned ResultSet::getUnsigned(string index){return getUnsigned(getColumnIndex(index));}

/*******************************************************************************************************/
    long ResultSet::getLong(int index){return getInteger<long>(_bindColumns[index]);}
    long ResultSet::getLong(string index){return getLong(getColumnIndex(index));}

/*******************************************************************************************************/
    int32_t ResultSet::getInt32(int index){return getInteger<int32_t>(_bindColumns[index]);}
    int32_t ResultSet::getInt32(string index){return getInt32(getColumnIndex(index));}

/*******************************************************************************************************/
    uint32_t ResultSet::getUnsigned32(int index){return getInteger<uint32_t>(_bindColumns[index]);}
    uint32_t ResultSet::getUnsigned32(string index){return getUnsigned32(getColumnIndex(index));}

/*******************************************************************************************************/
    int64_t ResultSet::getInt64(int index){return getInteger<int64_t>(_bindColumns[index]);}
    int64_t ResultSet::getInt64(string index){return getInt64(getColumnIndex(index));}

/*******************************************************************************************************/
    uint64_t ResultSet::getUnsigned64(int index){return getInteger<uint64_t>(_bindColumns[index]);}
    uint64_t ResultSet::getUnsigned64(string index){return getUnsigned64(getColumnIndex(index));}

/*******************************************************************************************************/
bool ResultSet::isNull(int col){}
bool ResultSet::isNull(string col){return isNull(getColumnIndex(col));}

    string ResultSet::getBlob(int index)
    {
//      if (isNull(bind))
//        throw NullValue();
		string ret;
		MYSQL_BIND bind=_bindColumns[index];
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
//        default:
//          log_error("type-error in getBlob, type=" << bind.buffer_type);
//          throw TypeError("type-error in getBlob");
      }
      return ret;
    }


//string ResultSet::getBlob(int col){}

string ResultSet::getBlob(string col){return getBlob(getColumnIndex(col));}
string ResultSet::getClob(int col){return getBlob(col);}
string ResultSet::getClob(string col){return getBlob(getColumnIndex(col));}

int ResultSet::getColumnIndex(string name){
	int cols=mysql_num_fields(_resultSetMetadata);
	int index=-1;
	MYSQL_FIELD *fields=mysql_fetch_fields(_resultSetMetadata);
	for (int a=0;a<cols;a++){
		if(strcasecmp(name.c_str(),fields[a].name)==0){
			index=a;
		}
	}
	return index;
}

