#include <map>
#include <string>
#include <iostream>
#ifdef MSVC
#include <my_global.h>
#endif

//#include <my_global.h>
#include <mysql.h>

//#include "Column.h"
#include "ResultSetMetaData.h"
#include "SqlException.h"
#include "Row.h"

namespace org {
  namespace esb {
    namespace sql {

        Row::Row(MYSQL_STMT * stmt) : st(stmt) {
          //      std::cerr<<"Create Row Object"<<std::endl;

          rsmd = new ResultSetMetaData(stmt);
          int count = rsmd->getColumnCount();
          bind = new MYSQL_BIND[count];
          memset(bind, 0, sizeof (MYSQL_BIND) * count);

          for (int a = 0; a < count; a++) {
            MYSQL_FIELD * f=rsmd->getColumn(a);
            Column * col = new Column(f, bind[a]);
            cols[std::string(f->name)] = col;
            fqncols[std::string(f->table).append(".").append(f->name)] = col;
            idx2name[a] = std::string(f->table).append(".").append(f->name);
//            delete f;
          }
          if (mysql_stmt_bind_result(stmt, bind)) {
            throw SqlException(std::string("failed while bind the result: ").append(mysql_stmt_error(stmt)));
          }
          if (mysql_stmt_store_result(stmt)) {
            fprintf(stderr, " mysql_stmt_store_result() failed\n");
            fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
            exit(0);
          }
        }

        Column * Row::getColumn(std::string name) {
          Column * col = cols[name];
          if (col == NULL)col = fqncols[name];
          if (col == NULL)throw SqlException(std::string("Column with name:").append(name).append(" not found"));
          return col;
        }

        Column * Row::getColumn(int idx) {
          return getColumn(idx2name[idx]);
        }

        ResultSetMetaData * Row::getMetaData() {
          return rsmd;
        }

        bool Row::next() {
          int res = mysql_stmt_fetch(st);
          if (res != 0 && res != MYSQL_NO_DATA && res != MYSQL_DATA_TRUNCATED) {
            throw SqlException(std::string("failed while fetch the data: ").append(mysql_stmt_error(st)));

            //          throw SqlException(mysql_stmt_error(st));
          }
          if (res == MYSQL_DATA_TRUNCATED) {

          }

          return res != MYSQL_NO_DATA;
        }

        Row::~Row() {
          std::cerr<<"delete Row Object"<<std::endl;
          delete rsmd;
          //      mysql_free_result(meta);
          std::map<std::string, Column*>::iterator it = cols.begin();
          for (; it != cols.end(); it++) {
            //        std::cerr << "Delete Column:"<<(*it).second->getName()<<std::endl;
//            delete ;
            delete static_cast<Column*>((*it).second);
          }
          delete []bind;
          cols.clear();
          fqncols.clear();
          idx2name.clear();
          std::cerr<<"Row Object deleted "<<std::endl;

        }
/*
      private:
        MYSQL_STMT * st;
        MYSQL_BIND * bind;
        MYSQL_RES * meta;
        ResultSetMetaData * rsmd;
        std::map<std::string, Column*> cols;
        std::map<std::string, Column*> fqncols;
        std::map<int, std::string> idx2name;
 */
//      };
    }
  }
}


