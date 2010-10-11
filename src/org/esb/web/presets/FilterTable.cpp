/* 
 * File:   FilterTable.cpp
 * Author: HoelscJ
 * 
 * Created on 11. Oktober 2010, 17:16
 */

#include "FilterTable.h"
#include "Wt/WStandardItemModel"
#include "org/esb/util/Log.h"
#include "litesql/datasource.hpp"
namespace org {
  namespace esb {
    namespace web {

      class PresetFilterTableModel : public Wt::WStandardItemModel {
      public:

        PresetFilterTableModel(Ptr<db::Profile> p) : Wt::WStandardItemModel() {
          LOGDEBUG("FilterTableModel():Wt::WStandardItemModel()");
          insertColumns(0, 2);
          setHeaderData(0, std::string("Enabled"));
          setHeaderData(1, std::string("Filter"));
          std::vector<db::Filter> filters=p->filter().get().all();
          std::vector<db::Filter>::iterator it=filters.begin();
          for(;it!=filters.end();it++){
            insertRow(rowCount());
            setData(rowCount()-1,1,(*it).filtername.value());
          }
        }
      };

      PresetFilterTable::PresetFilterTable(Ptr<db::Profile> p) {
        LOGDEBUG("FilterTable::FilterTable() : Wt::Ext::TableView()");
        setModel(new PresetFilterTableModel( p));
        resize(300, 300);
        setColumnWidth(0, 50);
        setAutoExpandColumn(1);
        
      }

      PresetFilterTable::~PresetFilterTable() {
      }
    }
  }
}
