#include "SqlUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Decimal.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/sql/ResultSetMetaData.h"
#include <string>

namespace org {
  namespace esb {
    namespace web {

      void SqlUtil::map2sql(std::string tablename, std::map<std::string, std::string> data) {
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
          if (update){
            if ((*elit).first == "id")continue;
              sql += " " + (*elit).first + "=:" + (*elit).first + ", ";
          }
          else {
            fields += (*elit).first + ", ";
            values += ":" + (*elit).first + ", ";
          }
        }
        fields = fields.substr(0, fields.length() - 2);
        values = values.substr(0, values.length() - 2);
        if (update) {
          sql = sql.substr(0, sql.length() - 2);
          sql += " where id=:id";
        }else {
          sql += "(" + fields + ") VALUES (" + values + ")";
        }
        logdebug("SQL:" << sql);
        using namespace org::esb;
        sql::Connection con(std::string(config::Config::getProperty("db.connection")));
        sql::PreparedStatement pstmt = con.prepareStatement(sql.c_str());

        elit = data.begin();
        for (; elit != data.end(); elit++) {
//          if((*elit).first!="id"){
            pstmt.setString((*elit).first, (*elit).second);
            logdebug("map2sql: "<<(*elit).first<<"="<<(*elit).second);
//          }
        }
        pstmt.execute();
      }

      void SqlUtil::sql2map(std::string tablename, int key, std::map<std::string, std::string> &data) {
        using namespace org::esb;
        std::string sql = "SELECT * FROM " + tablename + " WHERE id=:id";
        sql::Connection con(std::string(config::Config::getProperty("db.connection")));
        sql::PreparedStatement pstmt = con.prepareStatement(sql.c_str());
        pstmt.setInt("id", key);
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
