#ifndef ORG_ESB_SQL_RESULTSET_H
#define ORG_ESB_SQL_RESULTSET_H

#include "my_sql.h"

#include "Statement.h"
#include "PreparedStatement.h"
#include "Row.h"
#include "ResultSetMetaData.h"

#include <boost/shared_ptr.hpp>
namespace org {
  namespace esb {
    namespace sql {
      class Column;

      class ResultSet {
      public:
        bool next();

        bool getBool(int index);
        bool getBool(std::string index);
        int getInt(int index);
        int getInt(std::string index);
        long long int getLong(int index);
        long long int getLong(std::string index);
        double getDouble(int index);
        double getDouble(std::string index);

        const std::string getString(int col);
        const std::string getString(std::string col);

        std::string getBlob(int index);
        std::string getBlob(std::string index);

        std::string getClob(int index);
        std::string getClob(std::string index);

        float getFloat(int col);
        float getFloat(std::string col);

        bool isNull(int col);
        bool isNull(std::string col);
        ResultSetMetaData * getResultSetMetaData();
        ~ResultSet();
        ResultSet(MYSQL_STMT & stmt);

      private:
//        friend class Statement;
//        friend class PreparedStatement;

        bool isBeforeFirst;
//        Row row;
        boost::shared_ptr<Row> _rowPtr;
      };
    }
  }
}
#endif

