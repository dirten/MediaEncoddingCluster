/* 
 * File:   NodeListPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 29. September 2010, 12:27
 */

#include "NodeListPanel.h"
#include "Wt/WStandardItemModel"
#include "org/esb/hive/NodeResolver.h"
#include "org/esb/util/StringUtil.h"
#include "Wt/Ext/TableView"
#include "Wt/WFitLayout"
namespace org {
  namespace esb {
    namespace web {
      class NodeListTableModel: public Wt::WStandardItemModel{
      public:
        NodeListTableModel(org::esb::hive::NodeResolver::NodeList data): Wt::WStandardItemModel(0, 0, NULL) {
          insertColumns(0, 6);
          setHeaderData(0, std::string("#"));
          setHeaderData(1, std::string("Name"));
          setHeaderData(2, std::string("IP"));
          setHeaderData(3, std::string("Status"));
          setHeaderData(4, std::string("Type"));
          setHeaderData(5, std::string("Version"));

          setModelData(data);
        }
        void refresh(org::esb::hive::NodeResolver::NodeList data){
          setModelData(data);
        }
        void setModelData(org::esb::hive::NodeResolver::NodeList data){
          if (rowCount() < data.size())
            insertRows(rowCount(), data.size() - rowCount());
          if (rowCount() > data.size()) {
            removeRows(0, rowCount() - data.size());
          }
          org::esb::hive::NodeResolver::NodeList::iterator node_it=data.begin();
          for (int a = 0; node_it!=data.end();node_it++, a++) {
            Ptr<org::esb::hive::Node> node = (*node_it);
            setData(a, 0, org::esb::util::StringUtil::toString(a));
//            setData(a, 1, node->getName().to_string());
            setData(a, 2, node->getIpAddress().to_string());
            setData(a, 3, node->getStatus()==org::esb::hive::Node::NODE_UP?"up":"down");
            setData(a, 4, node->getData("type"));
            setData(a, 5, node->getData("version"));
          }
        }
      };
      class NodeListTable: public Wt::Ext::TableView{
      public:
        NodeListTable(): Wt::Ext::TableView(){
          org::esb::hive::NodeResolver::NodeList nodes=org::esb::hive::NodeResolver::getNodes();
          _model=new NodeListTableModel(nodes);
          setModel(_model.get());
        }
        void refresh(){
          org::esb::hive::NodeResolver::NodeList nodes=org::esb::hive::NodeResolver::getNodes();
          _model->refresh(nodes);
        }
      private:
        Ptr<NodeListTableModel> _model;
      };
      NodeListPanel::NodeListPanel():Wt::Ext::Container(NULL) {
        Wt::WFitLayout * l=new Wt::WFitLayout();
        setLayout(l);
        NodeListTable * table=new NodeListTable();
        layout()->addWidget(table);
      }

      NodeListPanel::NodeListPanel(const NodeListPanel& orig) {
      }

      NodeListPanel::~NodeListPanel() {
      }
    }
  }
}
