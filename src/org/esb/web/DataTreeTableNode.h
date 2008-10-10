/* 
 * File:   DataTreeTableNode.h
 * Author: jhoelscher
 *
 * Created on 6. Oktober 2008, 15:13
 */
#include <Wt/WTreeTableNode>
#include <Wt/WIconPair>
#include <Wt/WInteractWidget>
#include <Wt/WWebWidget>
#include "org/esb/sql/ResultSet.h"

#ifndef _DATATREETABLENODE_H
#define	_DATATREETABLENODE_H
namespace org {
    namespace esb {
        namespace web {

            class DataTreeTableNode : public Wt::WTreeTableNode/*, public Wt::WInteractWidget*/ {
            public:
                /*! \brief Construct a new node for the given file.
                 */
                DataTreeTableNode( org::esb::sql::ResultSet & rs);
                DataTreeTableNode(const std::string & name, const int type=0, const int parentid=-1);

            private:
                //! The path.
                const int _id;
                
//                boost::filesystem::path path_;

                //! Reimplements WTreeNode::populate to read files within a directory.
                virtual void populate();

                //! Reimplements WTreeNode::expandable
                virtual bool expandable();

                virtual bool isSelectable() const;
                //! Create the iconpair for representing the path.
                static Wt::WIconPair *createIcon(const int id);
                
//                virtual Wt::DomElementType domElementType() const;
                
                void selectAction();
                
                int _parentid;
            };

        }
    }
}

#endif	/* _DATATREETABLENODE_H */

