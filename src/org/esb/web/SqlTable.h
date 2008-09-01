#ifndef ORG_ESB_WEB_SQLTABLE_H
#define ORG_ESB_WEB_SQLTABLE_H
#include <Wt/Ext/TableView>
#include <Wt/WContainerWidget>
#include "SqlTableModel.h"

#include <string>
namespace org{
namespace esb{
namespace web{
class SqlTable:public Wt::Ext::TableView {
  public:
    SqlTable(std::string sql,Wt::WContainerWidget * parent=0);
    void reload(std::string sql="");
  private:
    std::string sql;
    SqlTableModel * mod;
};
}}}
#endif
