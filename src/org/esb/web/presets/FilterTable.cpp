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
#include "Wt/Ext/CheckBox"
namespace org {
  namespace esb {
    namespace web {

      class PresetFilterTableModel : public Wt::WStandardItemModel {
      public:

        PresetFilterTableModel(std::vector<Ptr<db::Filter> > filter) : Wt::WStandardItemModel() {
          LOGDEBUG("FilterTableModel():Wt::WStandardItemModel()");
          insertColumns(0, 2);
          setHeaderData(0, std::string("Enabled"));
          setHeaderData(1, std::string("Filter"));
          std::vector<Ptr<db::Filter> >::iterator it=filter.begin();
          for(;it!=filter.end();it++){
            insertRow(rowCount());
            setData(rowCount()-1,1,(*it)->filtername.value());
          }
        }
      };

      PresetFilterTable::PresetFilterTable(std::vector<Ptr<db::Filter> > filter) {
        LOGDEBUG("FilterTable::FilterTable() : Wt::Ext::TableView()");
        setModel(new PresetFilterTableModel( filter));
        resize(300, 300);
        setColumnWidth(0, 50);
        setAutoExpandColumn(1);
        Wt::Ext::CheckBox * check=new Wt::Ext::CheckBox();
        
        setEditor(0, check);
        
      }

      PresetFilterTable::~PresetFilterTable() {
      }
    }
  }
}
