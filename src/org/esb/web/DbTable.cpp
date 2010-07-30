#include "DbTable.h"

namespace org{
namespace esb{
namespace web{
        DbTable::DbTable(list<ColumnConfig> cc,const litesql::Expr & expr,Wt::WContainerWidget * parent): Wt::Ext::TableView(parent){
          model=new DbTableModel(cc,expr,parent);
          setModel(model);
          setAlternatingRowColors(true);
          resizeColumnsToContents(true);
          setHighlightMouseOver(true);
          setSelectionBehavior(Wt::SelectRows);
          setSelectionMode(Wt::SingleSelection);
	  std::list<ColumnConfig>::iterator confit=cc.begin();
	  for(int a=0;confit!=cc.end();confit++, a++){
            enableColumnHiding(a, true);
            setColumnSortable(a, true);
	    setColumnWidth(a,(*confit).getWidth());
	  }
	_clickCount=0;
        cellClicked.connect(SLOT(this,DbTable::itemSelected));
        doubleClickTimer=new Wt::WTimer(this);
        doubleClickTimer->setInterval(200);
        doubleClickTimer->timeout.connect(SLOT(this, DbTable::emitClickCount));

        }
	DbTable::DbTable(list<ColumnConfig> cc,const std::string & sql,Wt::WContainerWidget * parent): Wt::Ext::TableView(parent){
          model=new DbTableModel(cc,sql,parent);
          setModel(model);
          setAlternatingRowColors(true);
          resizeColumnsToContents(true);
          setHighlightMouseOver(true);
          setSelectionBehavior(Wt::SelectRows);
          setSelectionMode(Wt::SingleSelection);
	  std::list<ColumnConfig>::iterator confit=cc.begin();
	  for(int a=0;confit!=cc.end();confit++, a++){
            enableColumnHiding(a, true);
            setColumnSortable(a, true);
	    setColumnWidth(a,(*confit).getWidth());
	  }
	_clickCount=0;
        cellClicked.connect(SLOT(this,DbTable::itemSelected));
        doubleClickTimer=new Wt::WTimer(this);
        doubleClickTimer->setInterval(200);
        doubleClickTimer->timeout.connect(SLOT(this, DbTable::emitClickCount));

	}
	DbTableModel * DbTable::getModel(){
	    return model;
	}
	
	void DbTable::reload(){
	
	}
        void DbTable::itemSelected(){
        if(!doubleClickTimer->isActive())
          doubleClickTimer->start();
        _clickCount++;

      }

      void DbTable::emitClickCount(){
        if(_clickCount>1){
          doubleClicked.emit();
        }else{
          clicked.emit();
        }
        if(doubleClickTimer->isActive())
          doubleClickTimer->stop();
        _clickCount=0;
      }

}}}

