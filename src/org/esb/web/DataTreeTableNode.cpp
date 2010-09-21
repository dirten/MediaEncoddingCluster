#include "DataTreeTableNode.h"
#include <Wt/WStringUtil>
#include <Wt/WIconPair>
#include <Wt/WText>

#include "FileNode.h"
#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/util/Log.h"
namespace org {
    namespace esb {
        namespace web {

            DataTreeTableNode::DataTreeTableNode(org::esb::sql::ResultSet & rs)
            : WTreeTableNode(Wt::widen(rs.getString("filename")), createIcon(rs.getInt("type"))),_id(rs.getInt("id")),_parentid(rs.getInt("parent")) {
              label()->setTextFormat(Wt::PlainText);
              Wt::WTreeTableNode::setStyleClass("datatreenode");
//              this->selected.connect(SLOT(this, DataTreeTableNode::selectAction));
            }
            
            DataTreeTableNode::DataTreeTableNode(const std::string & name, const int type, const int parentid, const bool pop)
            : WTreeTableNode(Wt::widen(name), createIcon(type)),_id(parentid) {
              label()->setTextFormat(Wt::PlainText);
              Wt::WTreeTableNode::setStyleClass("datatreenode");
              _withPopulate=pop;
//              this->selected.connect(SLOT(this, DataTreeTableNode::selectAction));
              
            }

            Wt::WIconPair *DataTreeTableNode::createIcon(const int id) {
              return new Wt::WIconPair("icons/yellow-folder-closed.png",
			 "icons/yellow-folder-open.png", false);

            }
            void DataTreeTableNode::selectAction(){
//                logdebug("Node selected");
            }

            void DataTreeTableNode::populate() {
                if(_withPopulate){
                  org::esb::sql::Connection con(org::esb::config::Config::getProperty("db.connection"));
                  org::esb::sql::PreparedStatement pstmt=con.prepareStatement("select * from files where parent=:id");
                  pstmt.setInt("id",_id>0?_id:0);
                  org::esb::sql::ResultSet rs=pstmt.executeQuery();

                  while(rs.next()){
                    addChildNode(new FileNode(rs));
                  }
                }
            }

            bool DataTreeTableNode::isSelectable() const {
                return true;
            }

            bool DataTreeTableNode::expandable() {
                return true;
            }
            int DataTreeTableNode::getId()const{
                return _id;
            }
            /*
            Wt::DomElementType domElementType(){
                Wt::WTreeTableNode::domElementType();
            }
             **/
        }
    }
}
