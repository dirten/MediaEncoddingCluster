#include <Wt/WContainerWidget>
/*#include <Wt/WStandardItemModel>
#include <Wt/WFitLayout>
#include <Wt/WTimer>
#include <Wt/WBorderLayout>
#include <Wt/WString>
#include <Wt/Ext/TableView>
#include <Wt/WContainerWidget>
#include <Wt/Ext/ToolBar>
#include "org/esb/config/config.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/sql/ResultSetMetaData.h"

#include <iostream>
#include "SqlTableModel.h"
*/
#include <Wt/WTimer>

#include <Wt/Ext/Splitter>
#include <Wt/Ext/TabWidget>
#include <Wt/WContainerWidget>
#include <Wt/WTree>
#include <Wt/WTreeTableNode>

#include <Wt/WLength>

#include "SqlTable.h"
#include "FileInfo.cpp"
#include "FileTreeTable.h"
using namespace org::esb;
//using namespace org::esb::config;

namespace org{
namespace esb{
namespace web{

class Files: public Wt::WContainerWidget{
  public:
    Files(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent){
        FileTreeTable *treeTable = new FileTreeTable("/home");
        treeTable->resize(500, 600);
          treeTable->tree()->setSelectionMode(Wt::ExtendedSelection);
          treeTable->treeRoot()->setNodeVisible(false);
          treeTable->treeRoot()->setChildCountPolicy(Wt::WTreeNode::Enabled);

        this->addWidget(treeTable);
      tab = new SqlTable(std::string("select id, filename, author, title, copyright, comment, album, year, track, genre from files where parent=0"),this);
      tab->resize(Wt::WLength()/*100,Wt::WLength::Percentage)*/,300);
      tab->itemSelectionChanged.connect(SLOT(this, Files::fileSelected));

//      sp->addWidget(tab);
      Wt::Ext::TabWidget *exampleTabs = new Wt::Ext::TabWidget(this);
      exampleTabs->addTab(new Wt::WContainerWidget(),"Information");

      str=new SqlTable("select * from files limit 0 ");
      str->itemSelectionChanged.connect(SLOT(this, Files::versionSelected));
      str->resize(Wt::WLength()/*100,Wt::WLength::Percentage)*/,100);
      exampleTabs->addTab(str,"Versions");

//      str->resize(1000,400);
      
      info=new FileInfo(this);
      
      Wt::WTimer * timer=new Wt::WTimer(this);
      timer->setInterval(30000);
      timer->timeout.connect(SLOT(this, Files::refresh));
//      timer->start();
    }

    void refresh(){
        tab->reload(std::string("select id, filename, author, title, copyright, comment, album, year, track, genre from files where parent=0"));
    }
    
    void fileSelected(){
      std::string idstr=boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0));
      std::string sql="select * from files where parent="+idstr+" or id="+idstr;
      str->reload(sql);
    }

    void versionSelected(){
      std::string idstr=boost::any_cast<string>(str->model()->data(str->selectedRows()[0],0));
      info->setData(atoi(idstr.c_str()));
    }

private:
      SqlTable * tab;
      SqlTable * str;
      FileInfo * info;
};
}}}

