#include "FileNode.h"
#include <Wt/WStringUtil>
#include <Wt/WText>

#include "org/esb/util/Log.h"
/*
#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
*/
namespace org {
    namespace esb {
        namespace web {

            FileNode::FileNode(org::esb::sql::ResultSet & rs)
            : WTreeTableNode(Wt::widen(rs.getString("filename")), createIcon(rs.getInt("type"))),_id(rs.getInt("id")),_parentid(rs.getInt("parent")) {
//              label()->setFormatting(Wt::WText::PlainFormatting);
              Wt::WTreeTableNode::setStyleClass("datatreenode");
//              this->selected.connect(SLOT(this, DataTreeTableNode::selectAction));
            }
            
            FileNode::FileNode(const std::string & name, const int type, const int parentid)
            : WTreeTableNode(Wt::widen(name), createIcon(type)),_id(parentid) {
//              label()->setFormatting(Wt::WText::PlainFormatting);
              Wt::WTreeTableNode::setStyleClass("datatreenode");
//              this->selected.connect(SLOT(this, DataTreeTableNode::selectAction));
            }

            Wt::WIconPair *FileNode::createIcon(const int id) {
              return new Wt::WIconPair("icons/document.png",
			 "icons/document.png", false);

            }
            void FileNode::selectAction(){
                logdebug("Node selected");
            }

            void FileNode::populate() {
/*                
                org::esb::sql::Connection con(org::esb::config::Config::getProperty("db.connection"));
                org::esb::sql::PreparedStatement pstmt=con.prepareStatement("select * from files where parent=:id");
                pstmt.setInt("id",_id>0?_id:0);
                org::esb::sql::ResultSet rs=pstmt.executeQuery();
                while(rs.next()){
                  addChildNode(new FileNode(rs));
                }
 */
            }

            bool FileNode::isSelectable() const {
                return true;
            }

            bool FileNode::expandable() {
                return false;
            }
            int FileNode::getId()const{
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
