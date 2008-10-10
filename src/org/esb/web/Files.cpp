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
#include <Wt/WLength>
#include <Wt/WScrollArea>

#include <Wt/Ext/Splitter>
#include <Wt/Ext/TabWidget>
#include <Wt/WContainerWidget>
#include <Wt/WTree>
#include <Wt/WText>
#include <Wt/WTreeTableNode>

#include <Wt/WLength>

#include <Wt/Ext/Splitter>

#include "org/esb/util/Log.h"
#include "SqlTable.h"
#include "FileInfo.cpp"
#include "FileTreeTable.h"
#include "DataTreeTable.h"
using namespace org::esb;
//using namespace org::esb::config;

namespace org{
namespace esb{
namespace web{

class Files: public Wt::WContainerWidget{
  public:
    Files(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent){


        Wt::Ext::Splitter * spVert=new Wt::Ext::Splitter(Wt::Vertical,this);
        spVert->resize(1260,800);

        Wt::Ext::Splitter * sp=new Wt::Ext::Splitter(spVert);
        sp->resize(1260,500);
/*
        FileTreeTable *treeTable = new FileTreeTable("/home");
        treeTable->resize(500, 600);
          treeTable->tree()->setSelectionMode(Wt::ExtendedSelection);
          treeTable->treeRoot()->setNodeVisible(true);
          treeTable->treeRoot()->setChildCountPolicy(Wt::WTreeNode::Enabled);

        sp->addWidget(treeTable);
//        sp->children().back()->resize(350, 600);
        sp->children().back()->setMinimumSize(130, 600);
//        sp->children().back()->setMaximumSize(600, 600);

*/


//        Wt::WScrollArea * sa=new Wt::WScrollArea();

        
        DataTreeTable *datatree = new DataTreeTable(0);
        datatree->resize(1200, 500);
          datatree->tree()->setSelectionMode(Wt::ExtendedSelection);
          datatree->treeRoot()->setNodeVisible(false);
          datatree->treeRoot()->setChildCountPolicy(Wt::WTreeNode::Enabled);
                  
//        sa->setWidget(datatree);
        sp->addWidget(datatree);
        sp->children().back()->resize(200, 500);
        sp->children().back()->setMinimumSize(130, 500);

/*          
        sp->addWidget(treeTable2);
        sp->children().back()->resize(250, 600);
        sp->children().back()->setMinimumSize(230, 600);
 */
//        sp->children().back()->setMaximumSize(270, Wt::WLength());
  
//        sp->addWidget(treeTable);
//        sp->addWidget(new Wt::WText("left"));
//        sp->addWidget(new Wt::WText("right"));
//        this->addWidget(treeTable);      
//        this->addWidget(treeTable);
      tab = new SqlTable(std::string("select id, filename, container_type, size from files "),this);
      tab->resize(Wt::WLength()/*100,Wt::WLength::Percentage)*/,300);
      tab->itemSelectionChanged.connect(SLOT(this, Files::fileSelected));
      tab->setColumnHidden(0,true);
      sp->addWidget(tab);
        sp->children().back()->resize(1260, 500);
        sp->children().back()->setMinimumSize(230, 500);

        
      Wt::Ext::TabWidget *exampleTabs = new Wt::Ext::TabWidget(spVert);
      spVert->children().back()->resize(1260, 200);

//      str=new SqlTable("select * from files limit 0 ");
//      str->itemSelectionChanged.connect(SLOT(this, Files::versionSelected));
//      str->resize(Wt::WLength()/*100,Wt::WLength::Percentage)*/,100);
//      exampleTabs->addTab(str,"Versions");


//      str->resize(1000,400);
      
      info=new FileInfo(this);
      exampleTabs->addTab(info,"Information");
      
      Wt::WTimer * timer=new Wt::WTimer(this);
      timer->setInterval(30000);
      timer->timeout.connect(SLOT(this, Files::refresh));
//      timer->start();
    }

    void refresh(){
        tab->reload(std::string("select id, filename, container_type, size from files "));
    }
    
    void fileSelected(){
        logdebug("fileSelected");
//      std::string idstr=boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0));
//      std::string sql="select * from files where parent="+idstr+" or id="+idstr;
//      str->reload(sql);
//      std::string idstr=boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0));
//      std::string idstr=boost::any_cast<string>(str->model()->data(str->selectedRows()[0],0));
//      info->setData(atoi(idstr.c_str()));
    }

    void versionSelected(){
      std::string idstr=boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0));
//      std::string idstr=boost::any_cast<string>(str->model()->data(str->selectedRows()[0],0));
      info->setData(atoi(idstr.c_str()));
    }

private:
      SqlTable * tab;
      SqlTable * str;
      FileInfo * info;
};
}}}

