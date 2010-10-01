/* 
 * File:   PresetTree.cpp
 * Author: HoelscJ
 * 
 * Created on 9. September 2010, 15:58
 */
#include "PresetTree.h"
#include "Wt/WFitLayout"
#include "Wt/WHBoxLayout"
#include "Wt/WTreeTable"
#include "Wt/WTreeTableNode"
#include "Wt/WTreeView"
#include "org/esb/config/config.h"
#include "Wt/WStandardItem"
#include "Wt/WStandardItemModel"
#include "Wt/WContainerWidget"
#include "Wt/Ext/Button"
#include "Wt/WAccordionLayout"
#include "org/esb/web/project/ProfilePanel.h"
#include "org/esb/web/project/FilterPanel.h"
#include "org/esb/web/project/ProjectPropertyPanel.h"
#include "org/esb/hive/DatabaseService.h"

namespace org {
  namespace esb {
    namespace web {
      namespace v2 {

        class ProfileTreeTableNode : public Wt::WTreeTableNode {
        private:
          db::Profile profile;
        public:
          ProfileTreeTableNode(db::Profile g) : profile(g), Wt::WTreeTableNode(g.name.value()) {
            
          }

          void populate() {

          }

          bool expandable() {
            return false;
          }
        };

        class PresetTreeTableNode : public Wt::WTreeTableNode {
        private:
          db::ProfileGroup group;
        public:

          PresetTreeTableNode(db::ProfileGroup g) : group(g), Wt::WTreeTableNode(g.name.value()) {

          }

          void populate() {
            if (group.childrens().get().count() > 0) {
              vector<db::ProfileGroup> groups = group.childrens().get().all();
              vector<db::ProfileGroup>::iterator it = groups.begin();
              for (; it != groups.end(); it++) {
                addChildNode(new PresetTreeTableNode((*it)));
              }
            }
            if (group.profiles().get().count() > 0) {
              vector<db::Profile> profiles = group.profiles().get().all();
              vector<db::Profile>::iterator it = profiles.begin();
              for (; it != profiles.end(); it++) {
                Wt::WTreeTableNode * node=new Wt::WTreeTableNode("");
                addChildNode(node);
                node->setColumnWidget(1,new ProfileTreeTableNode((*it)));
              }
            }
          }

          bool expandable() {
            return group.childrens().get().count() > 0||group.profiles().get().count() > 0;
          }
        };

        class PresetTreeTable : public Wt::WTreeTable {
        public:

          PresetTreeTable(db::ProfileGroup g) : Wt::WTreeTable() {
            setTreeRoot(new PresetTreeTableNode(g), "Type");
            treeRoot()->setImagePack("resources/");
            treeRoot()->setStyleClass("treetablecol");
            setStyleClass("tree-table");
            //addColumn("Type", 100);

            addColumn("Preset", 300);
          }
        };

        class PresetTreeView : public Wt::WTreeView {
        public:

          PresetTreeView(Wt::WContainerWidget *parent = 0) : _db(org::esb::hive::DatabaseService::getDatabase()), Wt::WTreeView(parent) {
            LOGDEBUG("void PresetTreeView::PresetTreeView(Wt::WContainerWidget *parent = 0)");

            Wt::WStandardItemModel * model = new Wt::WStandardItemModel(0, 2, this);

            model->setHeaderData(0, Wt::Horizontal, std::string("Type"));
            model->setHeaderData(1, Wt::Horizontal, std::string("Name"));
            db::ProfileGroup group = litesql::select<db::ProfileGroup > (_db, db::ProfileGroup::Name == "root").one();
            Wt::WStandardItem * root = new Wt::WStandardItem("absolute root");
            setModelData(root, group);
            model->appendRow(root);
            setModel(model);
            setAlternatingRowColors(true);
            setColumnResizeEnabled(true);
            setSelectionMode(Wt::SingleSelection);
            setColumnWidth(0,100);
            setColumnWidth(1,200);
            

//            setRootIndex(model->index(1,0));

          }
          void refresh(){
            LOGDEBUG("void PresetTreeView::refresh()");
          }
          
          ~PresetTreeView(){
            LOGDEBUG("void PresetTreeView::~PresetTreeView()");
          }
          
        private:
          db::HiveDb _db;

          void setModelData(Wt::WStandardItem * parent, db::ProfileGroup g) {
            LOGDEBUG("void setModelData(Wt::WStandardItem * parent, db::ProfileGroup g)");
            std::vector<Wt::WStandardItem *> result_1;

            Wt::WStandardItem * item = new Wt::WStandardItem(g.name.value());
            item->setToolTip("here is a long tooltip, bal fasel bal fasel");
            result_1.push_back(item);
            result_1.push_back(new Wt::WStandardItem());
            parent->appendRow(result_1);
            if (g.childrens().get().count() > 0) {
              vector<db::ProfileGroup> groups = g.childrens().get().all();
              vector<db::ProfileGroup>::iterator it = groups.begin();
              for (; it != groups.end(); it++) {
                setModelData(item, (*it));
              }
            }
            if (g.profiles().get().count() > 0) {
              vector<db::Profile> profiles = g.profiles().get().all();
              vector<db::Profile>::iterator it = profiles.begin();
              for (; it != profiles.end(); it++) {
                std::vector<Wt::WStandardItem *> result;
                result.push_back(new Wt::WStandardItem());
                Wt::WStandardItem * profileitem = new Wt::WStandardItem((*it).name.value());
                result.push_back(profileitem);
                item->appendRow(result);
              }
            }
          }
        };

        PresetTree::PresetTree() :_db(org::esb::hive::DatabaseService::getDatabase()), Wt::Ext::Panel() {
          LOGDEBUG("PresetTree::PresetTree() ");
          //setTitle("Presets");
          setLayout(new Wt::WAccordionLayout());
          
          //layout()->addWidget(new Wt::WTreeView());
          //db::HiveDb db("mysql",org::esb::config::Config::getProperty("db.url"));
          _db.verbose=true;
          db::ProfileGroup group = litesql::select<db::ProfileGroup > (_db, db::ProfileGroup::Name == "root").one();
//          _table = new PresetTreeTable(group);
          
//          this->layout()->addWidget(new PresetTreeTable(group));
          Wt::Ext::Panel * panel1=new org::esb::web::ProfilePanel();
          
          this->layout()->addWidget(panel1);

          Wt::Ext::Panel * panel2=new org::esb::web::FilterPanel();
          
          this->layout()->addWidget(panel2);

          Wt::Ext::Panel * panel3=new org::esb::web::ProjectPropertyPanel();
          
          this->layout()->addWidget(panel3);
        }
        void PresetTree::refresh(){
          LOGDEBUG("void PresetTree::refresh()");
        }
        PresetTree::PresetTree(const PresetTree& orig):_db(orig._db)  {

        }

        PresetTree::~PresetTree() {
          LOGDEBUG("PresetTree::~PresetTree() ");
  //        _table.reset();
        }
      }
    }
  }
}

