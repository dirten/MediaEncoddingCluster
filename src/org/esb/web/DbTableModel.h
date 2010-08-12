#ifndef ORG_ESB_WEB_DBTABLEMODEL_H
#define ORG_ESB_WEB_DBTABLEMODEL_H
#include "org/esb/db/hivedb.hpp"
#include <Wt/WStandardItemModel>
#include <Wt/WContainerWidget>
#include <vector>
#include <list>
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
#include "ColumnConfig.h"
namespace org{
  namespace esb{
    namespace web{
//      template <typename T>
      class DbTableModel:public Wt::WStandardItemModel{
      public:
        DbTableModel( list<ColumnConfig> & cc,const litesql::Expr & expr=litesql::Expr(),Wt::WContainerWidget* parent=0);
        DbTableModel( list<ColumnConfig> & cc,const std::string & sql=std::string(),Wt::WContainerWidget* parent=0);
        void clear();
        void load();
      private:
        db::HiveDb _dbCon;
        
      };
    }}}
#endif


