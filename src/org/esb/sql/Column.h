#ifndef ORG_ESB_SQL_COLUMN_H
#define ORG_ESB_SQL_COLUMN_H
#include "SqlException.h"

#include <mysql/mysql.h>
#include <string>
#include <sstream>

namespace org{
namespace esb{
namespace sql{

class Column {
  public:
    Column(MYSQL_FIELD * field, MYSQL_BIND & bind);
    Column(MYSQL_BIND & bind);
    ~Column();
    std::string getString();
    void setString(const char *);
    void setString(const std::string&);
    bool getBool();
    int getInt();
    float getFloat();
    double getDouble();
    bool isNull();
    std::string getBlob();
    void setBlob(const std::string &);
  private:
    MYSQL_BIND & bind;
    char * buffer;
    void reserve(unsigned long size);
    unsigned long length;
    my_bool is_null;
    my_bool error;
//    int getInteger();
    template <typename int_type>
    int_type getInteger2();
    
    template <typename int_type>
    int_type getInteger();
    
    template <typename float_type>
    float_type getFloat();

};

}}}

#endif

