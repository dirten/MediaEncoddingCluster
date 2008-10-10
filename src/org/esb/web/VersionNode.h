/* 
 * File:   VersionNode.h
 * Author: jhoelscher
 *
 * Created on 7. Oktober 2008, 18:08
 */


#ifndef _VERSIONNODE_H
#define	_VERSIONNODE_H

#include <Wt/WTreeTableNode>
#include <Wt/WIconPair>

#include "org/esb/sql/ResultSet.h"

namespace org {
    namespace esb {
        namespace web {
            class VersionNode : public Wt::WTreeTableNode {
            public:
                VersionNode( org::esb::sql::ResultSet & rs);

            private:
                //! Reimplements WTreeNode::populate to read files within a directory.
                virtual void populate();

                //! Reimplements WTreeNode::expandable
                virtual bool expandable();

                virtual bool isSelectable() const;
                //! Create the iconpair for representing the path.
                static Wt::WIconPair *createIcon(const int id);
            };
        }
    }
}

#endif	/* _VERSIONNODE_H */

