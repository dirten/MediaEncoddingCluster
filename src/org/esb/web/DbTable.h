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
      template <typename T>
      class DbTable:public Wt::Ext::TableView {
      public:
        DbTable(Wt::WContainerWidget * parent=0){
          model=new DbTableModel<T>(parent);
          setModel(model);
        }
        void reload();
        Wt::Signal<void>doubleClicked;
        Wt::Signal<void>clicked;
        Wt::Signal<void>selectionChanged;
      private:
        std::string sql;
        DbTableModel<T> * model;
        void itemSelected();
        void emitClickCount();
        Wt::WTimer * doubleClickTimer;
        int _clickCount;
      };
    }}}
#endif
