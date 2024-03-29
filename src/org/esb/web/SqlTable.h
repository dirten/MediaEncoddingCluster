#ifndef ORG_ESB_WEB_SQLTABLE_H
#define ORG_ESB_WEB_SQLTABLE_H
#include <Wt/WContainerWidget>
#include <Wt/Ext/TableView>
#include <Wt/Ext/ToolBar>
#include <Wt/WTimer>
#include "SqlTableModel.h"

#include <string>
namespace org{
namespace esb{
namespace web{
class SqlTable:public Wt::Ext::TableView {
  public:
    SqlTable(std::string sql,Wt::WContainerWidget * parent=0);
    //virtual ~SqlTable();
    void reload(std::string sql="");
    Wt::Signal<void>doubleClicked;
    Wt::Signal<void>clicked;
    Wt::Signal<void>selectionChanged;
  private:
    std::string sql;
    SqlTableModel * mod;
    void itemSelected();
    void emitClickCount();
    Wt::WTimer * doubleClickTimer;
    int _clickCount;
};
}}}
#endif
