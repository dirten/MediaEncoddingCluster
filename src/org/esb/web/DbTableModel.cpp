#include "DbTableModel.h"

namespace org{
  namespace esb{
    namespace web{
      DbTableModel::DbTableModel( list<ColumnConfig> & cc,const litesql::Expr & expr,Wt::WContainerWidget* parent): 
    Wt::WStandardItemModel(0, 0, parent), _dbCon("mysql",org::esb::config::Config::getProperty("db.url")){
      //              std::vector<T> data=litesql::select<T>(_dbCon).all();
      //              std::vector<litesql::FieldType> fields;
      //              T tmp_model(_dbCon);
      //              tmp_model.getFieldTypes(fields);

      insertColumns(0, cc.size());

      litesql::SelectQuery sel;
      std::list<std::string> table_list;
      std::list<ColumnConfig>::iterator confit=cc.begin();
      for(int a=0;confit!=cc.end();confit++, a++){
        table_list.push_back((*confit).getFieldType().table());
        sel.result((*confit).getFieldName());
        if (!setHeaderData(a, (*confit).getHeader())) {
          std::cout << "Failed" << std::endl;
        }
         LOGDEBUG("FieldType:"<<(*confit).getFieldType().type());
      }
      table_list.unique();
      std::list<std::string>::iterator it=table_list.begin();
      for(;it!=table_list.end();it++){
        sel.source((*it));
      }
      sel.where(expr);
      LOGDEBUG("SelectQuery:"<<sel.asString());
      litesql::Records recs = _dbCon.query(sel);
      int a=0;
      for (litesql::Records::iterator it = recs.begin(); it != recs.end(); it++, a++){
        LOGDEBUG("insert row")
        if (rowCount() <= a)
          insertRow(rowCount());
        for (int b = 0; b < (*it).size(); b++)
          if (!setData(a, b, (*it)[b])) {
            std::cout << "set data Failed" << std::endl;
          }else {
          }
      }
    }
    DbTableModel::DbTableModel(list<ColumnConfig> & cc,const std::string & sql,Wt::WContainerWidget* parent): 
    Wt::WStandardItemModel(0, 0, parent), _dbCon("mysql",org::esb::config::Config::getProperty("db.url")){
      //              std::vector<T> data=litesql::select<T>(_dbCon).all();
      //              std::vector<litesql::FieldType> fields;
      //              T tmp_model(_dbCon);
      //              tmp_model.getFieldTypes(fields);

      insertColumns(0, cc.size());

      //	      litesql::SelectQuery sel;
      //	      std::list<std::string> table_list;
      std::list<ColumnConfig>::iterator confit=cc.begin();
      for(int a=0;confit!=cc.end();confit++, a++){
        //		table_list.push_back((*confit).getFieldType().table());
        //                sel.result((*confit).getFieldName());
        if (!setHeaderData(a, (*confit).getHeader())) {
          std::cout << "Failed" << std::endl;
        }
      }
      //	      table_list.unique();
      //	      std::list<std::string>::iterator it=table_list.begin();
      //	      for(;it!=table_list.end();it++){
      //	        sel.source((*it));
      //	      }
      //	      sel.where(expr);
      LOGDEBUG("SelectQuery:"<<sql);
      litesql::Records recs = _dbCon.query(sql);
      int a=0;
      for (litesql::Records::iterator it = recs.begin(); it != recs.end(); it++, a++){
        if (rowCount() <= a)
          insertRow(rowCount());
        for (int b = 0; b < (*it).size(); b++)
          if (!setData(a, b, (*it)[b])) {
            std::cout << "set data Failed" << std::endl;
          }else {
          }
      }
    }
    }}}

