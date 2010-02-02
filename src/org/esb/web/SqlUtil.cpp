#include "SqlUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/sql/ResultSetMetaData.h"
#include "org/esb/util/StringUtil.h"
#include <string>

namespace org {
  namespace esb {
    namespace web {

      int64_t SqlUtil::map2sql(std::string tablename, std::map<std::string, std::string> data) {
        bool update = atoi(data["id"].c_str()) > 0 ? true : false;
        std::string sql;
        std::string fields;
        std::string values;
        if (update)
          sql += "UPDATE " + tablename + " SET ";
        else
          sql += "INSERT INTO " + tablename;
        std::map<std::string, std::string>::iterator elit = data.begin();
        for (; elit != data.end(); elit++) {
          if ((*elit).first == "id")continue;
          /**
           * special handling for fields that starts with an underscore
           * this fields will be ignored to save to DB
           */
          if ((*elit).first.c_str()[0] == '_')continue;
          if (update) {
            sql += " " + (*elit).first + "=:" + (*elit).first + ", ";
          } else {
            fields += (*elit).first + ", ";
            values += ":" + (*elit).first + ", ";
          }
        }
        fields = fields.substr(0, fields.length() - 2);
        values = values.substr(0, values.length() - 2);
        if (update) {
          sql = sql.substr(0, sql.length() - 2);
          sql += " where id=:id";
        } else {
          sql += "(" + fields + ") VALUES (" + values + ")";
        }
        LOGDEBUG("org.esb.web.SqlUtil", "SQL:" << sql);
        using namespace org::esb;
        sql::Connection con(std::string(config::Config::getProperty("db.connection")));
        sql::PreparedStatement pstmt = con.prepareStatement(sql.c_str());

        elit = data.begin();
        for (; elit != data.end(); elit++) {
          //          if((*elit).first!="id"){
          if ((*elit).second.length() > 0)
            pstmt.setString((*elit).first, (*elit).second);
          else
            pstmt.setNull((*elit).first);
          LOGDEBUG("org.esb.web.SqlUtil", "map2sql: " << (*elit).first << "=" << (*elit).second << ":");
          //          }
        }
        pstmt.execute();

        return update ? atoi(data["id"].c_str()) : pstmt.getLastInsertId();
      }

      void SqlUtil::sql2map(std::string tablename, int key, std::map<std::string, std::string> &data) {
        return sql2map(tablename,"id", org::esb::util::StringUtil::toString(key), data);
      }
      
      void SqlUtil::sql2map(std::string tablename,std::string keyfield, std::string key, std::map<std::string, std::string> &data) {
        using namespace org::esb;
        std::string sql = "SELECT * FROM " + tablename + " WHERE "+keyfield+"=:id";
        sql::Connection con(std::string(config::Config::getProperty("db.connection")));
        sql::PreparedStatement pstmt = con.prepareStatement(sql.c_str());
        pstmt.setString("id", key);
        sql::ResultSet rs = pstmt.executeQuery();
        if (rs.next()) {
          sql::ResultSetMetaData * rsmd = rs.getResultSetMetaData();
          int colCount = rsmd->getColumnCount();
          for (int a = 0; a < colCount; a++) {
            data[rsmd->getColumnName(a)] = rs.getString(a);
          }
        }
      }
    }
  }
}
