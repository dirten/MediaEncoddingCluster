/* 
 * File:   FilterTable.h
 * Author: HoelscJ
 *
 * Created on 11. Oktober 2010, 17:16
 */

#ifndef FILTERTABLE_H
#define	FILTERTABLE_H
#include "org/esb/db/hivedb.hpp"

#include "Wt/Ext/TableView"
#include "org/esb/lang/Ptr.h"
namespace org {
  namespace esb {
    namespace web {

      class PresetFilterTable: public Wt::Ext::TableView {
      public:
        PresetFilterTable(Ptr<db::Profile> p);
        virtual ~PresetFilterTable();
      private:

      };
    }
  }
}

#endif	/* FILTERTABLE_H */

