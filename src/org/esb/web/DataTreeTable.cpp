#include "DataTreeTable.h"
#include "DataTreeTableNode.h"

#include <Wt/WText>
#include <Wt/WTree>
//#include <Wt/WTreeNodeSet>

#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/util/Log.h"

namespace org {
    namespace esb {
        namespace web {

            DataTreeTable::DataTreeTable(const int pId, Wt::WContainerWidget *parent) {
/*
                addColumn("Size", 80);
                addColumn("Type", 110);
                addColumn("Modified", 110);
                header(1)->setStyleClass("fsize");
                header(2)->setStyleClass("date");
                header(3)->setStyleClass("date");
 */
/*
                org::esb::sql::Connection con(org::esb::config::Config::getProperty("db.connection"));
                org::esb::sql::Statement pstmt=con.createStatement("select * from files where parent=0");
                org::esb::sql::ResultSet rs=pstmt.executeQuery();
*/              
//                if(rs.next())
                DataTreeTableNode *root=new DataTreeTableNode("",0,0,false);
                DataTreeTableNode *files=new DataTreeTableNode("Files");
                DataTreeTableNode *versions=new DataTreeTableNode("Versions");
                DataTreeTableNode *profiles=new DataTreeTableNode("Profiles");
                root->addChildNode(files);
                root->addChildNode(versions);
                root->addChildNode(profiles);
                setTreeRoot(root,"");
                treeRoot()->setImagePack("icons/");
                treeRoot()->expand();
//                tree()->itemSelectionChanged.connect(SLOT(this,DataTreeTable::dataSelected));
                
            }
            
            void DataTreeTable::dataSelected(){
                std::set<Wt::WTreeNode*> set=tree()->selectedNodes();
                std::set<Wt::WTreeNode*>::iterator it=set.begin();
            }
        }
    }
}


