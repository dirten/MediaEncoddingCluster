#ifndef ORG_ESB_WEB_DBTABLEMODEL_H
#define ORG_ESB_WEB_DBTABLEMODEL_H
#include "org/esb/db/hivedb.hpp"
#include <Wt/WStandardItemModel>
#include <Wt/WContainerWidget>
#include <vector>
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
namespace org{
  namespace esb{
    namespace web{
      template <typename T>
      class DbTableModel:public Wt::WStandardItemModel{
      public:
        DbTableModel(Wt::WContainerWidget* parent=0): 
            Wt::WStandardItemModel(0, 0, parent), _dbCon("mysql",org::esb::config::Config::getProperty("db.url")){
              std::vector<T> data=litesql::select<T>(_dbCon).all();
              std::vector<litesql::FieldType> fields;
              T tmp_model(_dbCon);
              tmp_model.getFieldTypes(fields);
              LOGDEBUG("values:"<<tmp_model.table__);

              insertColumns(0, fields.size());
              std::vector<litesql::FieldType>::iterator fieldit=fields.begin();
              for(int a = 0;fieldit!=fields.end();fieldit++, a++){                
                LOGDEBUG("setting headername:"<<(*fieldit).name());
                if (!setHeaderData(a, (*fieldit).name())) {
                  std::cout << "Failed" << std::endl;
                }
              }
              std::string query="SELECT * from ";
              query+=tmp_model.table__;
              litesql::Records recs = _dbCon.query(query);
              int a=0;
              for (litesql::Records::iterator it = recs.begin(); it != recs.end(); it++, a++){
                if (rowCount() <= a)
                  insertRow(rowCount());
                for (int b = 0; b < (*it).size(); b++)
                  if (!setData(a, b, (*it)[b])) {
                    //          if(!setData(a,b,boost::any(result.getString(b)))){
                    //          if(!setData(a,b,boost::any(new string("test")))){
                    std::cout << "set data Failed" << std::endl;
                  }else {
                  }
              }

            }


            void clear();
            void load();
      private:
        db::HiveDb _dbCon;
      };
    }}}
#endif


