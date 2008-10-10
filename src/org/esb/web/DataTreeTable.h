/* 
 * File:   DataTreeTable.h
 * Author: jhoelscher
 *
 * Created on 6. Oktober 2008, 14:46
 */

#ifndef _DATATREETABLE_H
#define	_DATATREETABLE_H
#include <Wt/WTreeTable>
namespace org {
    namespace esb {
        namespace web {
            class DataTreeTable : public Wt::WTreeTable {
            public:
                /*! \brief Construct a new DataTreeTable.
                 *
                 * Create a new DataTreeTable to browse the given data.
                 */
                DataTreeTable(const int pId, Wt::WContainerWidget *parent = 0);
                void dataSelected();
            };

        }
    }
}

#endif	/* _DATATREETABLE_H */

