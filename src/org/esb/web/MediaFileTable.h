/* 
 * File:   MediaFileTable.h
 * Author: HoelscJ
 *
 * Created on 26. August 2010, 12:58
 */

#ifndef MEDIAFILETABLE_H
#define	MEDIAFILETABLE_H
#include "Wt/Ext/TableView"
#include "org/esb/lang/Ptr.h"

namespace org {
  namespace esb {
    namespace web {
      class MediaFileTableModel;

      class MediaFileTable:public Wt::Ext::TableView  {
      public:
        MediaFileTable();
        MediaFileTable(const MediaFileTable& orig);
        virtual ~MediaFileTable();
        void refresh() ;
      private:
        Ptr<MediaFileTableModel> _model;

      };
    }
  }
}

#endif	/* MEDIAFILETABLE_H */

