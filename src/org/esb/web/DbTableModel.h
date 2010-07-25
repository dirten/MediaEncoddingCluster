#ifndef ORG_ESB_WEB_DBTABLEMODEL_H
#define ORG_ESB_WEB_DBTABLEMODEL_H
#include "org/esb/db/hivedb.hpp"
#include <Wt/WStandardItemModel>
#include <Wt/WContainerWidget>
#include <vector>
#include "org/esb/util/Log.h"
namespace org{
namespace esb{
namespace web{
template <typename T>
  class DbTableModel:public Wt::WStandardItemModel{
  public:
    DbTableModel(Wt::WContainerWidget* parent=0): 
        Wt::WStandardItemModel(0, 0, parent),
        _dbCon("mysql",Config::getProperty("db.url")){
          std::vector<T> data=litesql::select<T>(_dbCon).all();
          std::vector<litesql::FieldType> fields;
          T tmp_model(_dbCon);
          tmp_model.getFieldTypes(fields);
          insertColumns(0, fields.size());
          std::vector<litesql::FieldType>::iterator fieldit=fields.begin();
          for(int a = 0;fieldit!=fields.end();fieldit++, a++){
            LOGDEBUG("setting headername:"<<(*fieldit).name());
            if (!setHeaderData(a, (*fieldit).name())) {
            std::cout << "Failed" << std::endl;
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


