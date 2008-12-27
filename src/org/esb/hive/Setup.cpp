#include "Setup.h"
#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/sql/SqlException.h"
#include "org/esb/lang/Exception.h"
#include "org/esb/util/Log.h"

#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/StringTokenizer.h"

using namespace org::esb;
namespace org {
  namespace esb {
    namespace hive {

      void Setup::check() {
        logdebug("check Database Cluster Environment");

        if (!checkDatabaseExist()) {
          logerror("Database not exist");
          exit(-127);
        }

        if (!checkDatabaseVersion(string("0.0.2"))) {
//          logerror("Database Version mismatch");
          exit(-127);
        }
//        buildDatabaseModel("../sql/hive-0.0.1.sql");
      }

      void Setup::buildDatabaseModel(std::string sqlfile) {
        logdebug("loading DB model from "<<sqlfile);
        std::string sql;
        io::File f(sqlfile.c_str());
        io::FileInputStream(&f).read(sql);
        sql::Connection con(config::Config::getProperty("db.connection"));
        util::StringTokenizer st(sql,";");

		int tok=st.countTokens();
		for(int i=0;i<tok-1;i++){
          std::string t=st.nextToken();
          t=StringUtil::trim(t,*new std::string("\n"));
          std::string next=StringUtil::trim(t);
            logdebug("Create Table "<<next);
            logdebug("Create Table leng"<<next.size());
            if(next.size()>1)
                con.executeNonQuery(next);
        }
      }

      void Setup::buildDatabase(std::string dbName) {
        std::string conf = config::Config::getProperty("db.connection");
        sql::Connection con(parseConnectionString(conf, "host"), string(""), parseConnectionString(conf, "user"), parseConnectionString(conf, "password"));
        con.executeNonQuery(string("CREATE DATABASE ").append(dbName));
        buildDatabaseModel("../sql/hive-0.0.2.sql");
      }

      bool Setup::checkDatabaseExist() {
        bool result = true;
        using namespace org::esb;
        std::string conf = config::Config::getProperty("db.connection");
        try {
//          std::string conf = config::Config::getProperty("db.connection");
          sql::Connection con(parseConnectionString(conf, "host"), string(""), parseConnectionString(conf, "user"), parseConnectionString(conf, "password"));
          con.executeNonQuery(std::string("use ").append(parseConnectionString(conf, "db")));
        } catch (sql::SqlException & ex) {
			logerror(ex.what());
          if(yesNoQuestion("do you want to Create Database "+parseConnectionString(conf, "db")+" now?")){
            buildDatabase(parseConnectionString(conf, "db"));
            result = true;
          }else
          result = false;
        }
        return result;
      }

      bool Setup::checkDatabaseVersion(std::string version) {
        bool result = true;
        using namespace org::esb;
        try {
          sql::Connection con(config::Config::getProperty("db.connection"));
          sql::Statement stmt = con.createStatement("SELECT * FROM version WHERE component='database.model'");
          sql::ResultSet rs = stmt.executeQuery();
          if ((!rs.next()) || rs.getString("version") != version) {
//            logerror("Database Version mismatch");
//            logerror("you must upgrade from version "<<rs.getString("version")<< " to "<<version);
            if(yesNoQuestion("do you want to Upgrade Database from version "+rs.getString("version")+" to "+version+" now?")){
              buildDatabaseModel("../sql/hive-upgrade-"+version+".sql");
            }
            result = false;
          }
        } catch (sql::SqlException & ex) {
			logerror(ex.what());
            if(yesNoQuestion("do you want to Create Database Model now?")){
              buildDatabaseModel("../sql/hive-0.0.1.sql");
              result=true;
            }
          result = false;
        }
        return result;
      }
      
      void Setup::upgradeDatabaseModel(std::string sqlfile){
        
      }
      
      bool Setup::yesNoQuestion(std::string quest){
        bool result=false;
        std::cout << quest<< " [y/N] :";
        std::string input;
        std::cin>>input;
        if(input.size()>1){
          cout << "please type only (y/n)"<<endl;
        }else{
          std::string in=util::StringUtil::toLower(input);
          if(in=="y")
            result=true;
        }
        return result;
      }
      std::string Setup::parseConnectionString(std::string constr, std::string key_com) {
        StringTokenizer tok(constr, ":");
        if (tok.countTokens() == 2) {
          tok.nextToken();
          StringTokenizer data(tok.nextToken(), ";");
          while (data.hasMoreTokens()) {
            StringTokenizer line(data.nextToken(), "=");
            string key = line.nextToken();
            string val = line.nextToken();
            if (key.compare(key_com) == 0) {
              return val;
            }
          }
        }
        return std::string("");
      }
    }
  }
}

