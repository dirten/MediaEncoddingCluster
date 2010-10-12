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
#include "Wt/Ext/Button"
#include "org/esb/lang/Ptr.h"
#include "../project/FilterChooser.h"
namespace org {
  namespace esb {
    namespace web {
      class PresetFilterTableModel;
      class PresetFilterTable: public Wt::Ext::TableView {
      public:
        PresetFilterTable(Ptr<db::Profile> p);
        virtual ~PresetFilterTable();
        void refresh();
      private:
        Wt::Ext::Button * removeOptionButton;
        void addFilter();
        void removeFilter();
        Ptr<FilterChooser> _chooser;
        void filterSelected();
         std::vector<Ptr<db::Filter> > _available_filter;
         Ptr<db::Profile> _profile;
         void itemSelectionChangedMethod();
         Ptr<PresetFilterTableModel> _model;
      };
    }
  }
}

#endif	/* FILTERTABLE_H */

