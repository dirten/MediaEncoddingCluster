/* 
 * File:   PresetTree.h
 * Author: HoelscJ
 *
 * Created on 9. September 2010, 15:58
 */

#ifndef PRESETTREE_H
#define	PRESETTREE_H
#include "org/esb/db/hivedb.hpp"

#include "Wt/Ext/Panel"
#include "org/esb/lang/Ptr.h"
namespace org {
  namespace esb {
    namespace web {
      namespace v2 {
        class PresetTreeTable;
        class PresetTreeView;
        class PresetTree :public Wt::Ext::Panel{
        public:
          PresetTree();
          PresetTree(const PresetTree& orig);
          virtual ~PresetTree();
          void refresh();
        private:
          Ptr<PresetTreeTable> _table;
          Ptr<PresetTreeView> _view;
          db::HiveDb _db;
        };
      }
    }
  }
}

#endif	/* PRESETTREE_H */

