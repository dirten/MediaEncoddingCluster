/* 
 * File:   Row.h
 * Author: jhoelscher
 *
 * Created on 6. November 2008, 16:12
 */

#include <map>

#include "my_sql.h"
#include "ResultSetMetaData.h"
#include "Column.h"
#ifndef _ROW_H
#define	_ROW_H
namespace org {
  namespace esb {
    namespace sql {
      class Row {
      public:
        Row(MYSQL_STMT * stmt) ;
        Column * getColumn(std::string name);
        Column * getColumn(int idx);
        ResultSetMetaData * getMetaData();
        bool next();
        ~Row();
      private:
        MYSQL_STMT * st;
        MYSQL_BIND * bind;
        MYSQL_RES * meta;
        ResultSetMetaData * rsmd;
        std::map<std::string, Column*> cols;
        std::map<std::string, Column*> fqncols;
        std::map<int, std::string> idx2name;
      };
    }
  }
}



#endif	/* _ROW_H */

