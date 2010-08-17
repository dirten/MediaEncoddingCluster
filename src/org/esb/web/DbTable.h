#ifndef ORG_ESB_WEB_DBTABLE_H
#define ORG_ESB_WEB_DBTABLE_H
#include "org/esb/db/hivedb.hpp"
#include <Wt/WContainerWidget>
#include <Wt/Ext/TableView>
#include <Wt/Ext/ToolBar>
#include <Wt/WTimer>
#include "DbTableModel.h"

#include <string>
namespace org{
  namespace esb{
    namespace web{
//      template <typename T>
      class DbTable:public Wt::Ext::TableView {
      public:
        DbTable(list<ColumnConfig> cc=list<ColumnConfig>(),const litesql::Expr & expr=litesql::Expr(),Wt::WContainerWidget * parent=0);
        DbTable(list<ColumnConfig> cc=list<ColumnConfig>(),const std::string & sql=std::string(),Wt::WContainerWidget * parent=0);
	DbTableModel * getModel();
        void reload();
        Wt::Signal<void>doubleClicked;
        Wt::Signal<void>clicked;
        Wt::Signal<void>selectionChanged;
        void refresh();
      private:
        std::string sql;
        DbTableModel* model;
        void itemSelected();
        void emitClickCount();
        Wt::WTimer * doubleClickTimer;
        int _clickCount;
        list<ColumnConfig> _column_config;
        litesql::Expr _expr;
        std::string _sql;
      };
    }}}
#endif
