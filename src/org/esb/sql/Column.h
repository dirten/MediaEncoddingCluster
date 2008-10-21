#ifndef ORG_ESB_SQL_COLUMN_H
#define ORG_ESB_SQL_COLUMN_H
#include "SqlException.h"
//#include "stdafx.h"   
//#include <winsock2.h>   
#include "my_sql.h"

//#include <mysql.h>
#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>

namespace org {
    namespace esb {
        namespace sql {

            class Column {
            public:
                Column(MYSQL_FIELD * field, MYSQL_BIND & bind);
                Column(MYSQL_BIND & bind);
                ~Column();
                const std::string getString();
                void setString(const char *);
                void setString(const std::string&);
                bool getBool();
                int getInt();
                void setInt(int);
                float getFloat();
                double getDouble();
                void setDouble(double);
                bool isNull();
                void setNull();
                std::string getBlob();
                void setBlob(const std::string &);
                void setBlob(const char *, int);
                std::string getName();
                std::string getTableName();
            private:
                MYSQL_BIND & bind;
                MYSQL_FIELD * _field;
//                boost::shared_ptr<char*> buffer;
                char* buffer;
                void reserve(unsigned long size);
                unsigned long length;
                my_bool is_null;
                my_bool error;
                //    int getInteger();

                template <typename int_type>
                int_type getInteger();

                template <typename float_type>
                float_type getFloat();

                template <typename c_type>
                void setValue(c_type value, enum_field_types mysql_type);

                std::string name;
            };

        }
    }
}

#endif

