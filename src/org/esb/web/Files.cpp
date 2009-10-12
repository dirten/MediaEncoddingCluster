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
//#include "org/esb/lang/Thread.h"
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

#include <Wt/WLabel>
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/TabWidget>
#include <Wt/WContainerWidget>
#include <Wt/WTree>
#include <Wt/WText>
#include <Wt/WTreeTableNode>

#include <Wt/WLength>

#include <Wt/Ext/Splitter>

#include "org/esb/util/Log.h"
#include "org/esb/util/ScopedTimeCounter.h"
#include "SqlTable.h"
#include "FileInfo.cpp"
#include "FileTreeTable.h"
#include "DataTreeTable.h"
#include "DataTreeTableNode.h"
#include "ProfileSelector.cpp"
using namespace org::esb;
//using namespace org::esb::config;

namespace org
{
  namespace esb
  {
    namespace web
    {

      class Files : public Wt::WContainerWidget
      {

public:

        Files(Wt::WContainerWidget * parent = 0) : Wt::WContainerWidget(parent) {
          //                    this->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength(100, Wt::WLength::Percentage));
          /*
                  Wt::Ext::Splitter * spVert=new Wt::Ext::Splitter(Wt::Vertical,this);
                  spVert->resize(1260,800);
           */
          //                    Wt::Ext::Splitter * sp = new Wt::Ext::Splitter(this);
          //                    sp->resize(1260, 800);
          //        sp->resize(Wt::WLength(100,Wt::WLength::Percentage),Wt::WLength(100,Wt::WLength::Percentage));
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

          /*
                              datatree = new DataTreeTable(0);
                              datatree->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength(100, Wt::WLength::Percentage));
                              datatree->tree()->setSelectionMode(Wt::ExtendedSelection);
                              datatree->treeRoot()->setNodeVisible(false);
                              datatree->treeRoot()->setChildCountPolicy(Wt::WTreeNode::Enabled);
                              //          this->addWidget(datatree);
                              //        sa->setWidget(datatree);
                              datatree->tree()->itemSelectionChanged.connect(SLOT(this, Files::fileSelected));

                              sp->addWidget(datatree);
                              //        sp->children().back()->resize(Wt::WLength(100,Wt::WLength::Percentage), Wt::WLength(100,Wt::WLength::Percentage));
                              sp->children().back()->setMinimumSize(250, Wt::WLength(100, Wt::WLength::Percentage));
           */
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



          tab = new SqlTable(std::string("select id, filename, container_type type, concat(size/1024/1024,'MB'), duration from files "), this);
          //                    tab->resize(800, 300);
//          tab->resize(Wt::WLength(), 300);
          tab->itemSelectionChanged.connect(SLOT(this, Files::fileSelected));
          tab->setColumnWidth(0, 10);
          tab->setColumnWidth(2, 10);
          tab->setColumnWidth(3, 20);

          //                    tab->setColumnHidden(0, true);


/*
          Wt::Ext::TabWidget * tab = new Wt::Ext::TabWidget(this);
          tab->resize(Wt::WLength(), Wt::WLength());
          tab->addTab(info = new FileInfo(), "Information");
          tab->addTab(pSelector = new ProfileSelector(), "Encoding");
*/
        }

        void search() {
          //                    std::string idstr=boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0));
          ScopedTimeCounter stc("search");
          std::string sql = std::string("select id, filename, container_type, size from files where filename like '%" + searchField->text().narrow() + "%'");
          tab->reload(sql);
        }

        void fileSelected() {
          if (tab->selectedRows().size() > 0) {
            std::string idstr = boost::any_cast<string > (tab->model()->data(tab->selectedRows()[0], 0));
            info->setData(atoi(idstr.c_str()));
            pSelector->setFileId(atoi(idstr.c_str()));
            logdebug("fileSelected" << idstr);
          }
          //      std::string idstr=boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0));
          //      std::string sql="select * from files where parent="+idstr+" or id="+idstr;
          //      str->reload(sql);
          //      std::string idstr=boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0));
          //      std::string idstr=boost::any_cast<string>(str->model()->data(str->selectedRows()[0],0));
          //      info->setData(atoi(idstr.c_str()));
        }

        void versionSelected() {
          std::string idstr = boost::any_cast<string > (tab->model()->data(tab->selectedRows()[0], 0));
          //      std::string idstr=boost::any_cast<string>(str->model()->data(str->selectedRows()[0],0));
          info->setData(atoi(idstr.c_str()));
        }



        SqlTable * tab;
        SqlTable * str;
        FileInfo * info;
        ProfileSelector * pSelector;
        DataTreeTable *datatree;
        Wt::Ext::LineEdit * searchField;
        Wt::Ext::Button * searchButton;
      };
    }
  }
}

