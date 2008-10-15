/* 
 * File:   FileNode.h
 * Author: jhoelscher
 *
 * Created on 13. Oktober 2008, 13:30
 */

#include "Wt/WTreeTableNode"
#include "org/esb/sql/ResultSet.h"
#ifndef _FILENODE_H
#define	_FILENODE_H

namespace org {
    namespace esb {
        namespace web {

            class FileNode : public Wt::WTreeTableNode/*, public Wt::WInteractWidget*/ {
            public:
                /*! \brief Construct a new node for the given file.
                 */
                FileNode( org::esb::sql::ResultSet & rs);
                FileNode(const std::string & name, const int type=0, const int parentid=-1);
                int getId() const;

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


#endif	/* _FILENODE_H */

