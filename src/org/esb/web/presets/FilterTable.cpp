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
#include "Wt/Ext/Button"
#include "Wt/Ext/ToolBar"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace web {

      class PresetFilterTableModel : public Wt::WStandardItemModel {
      public:

        PresetFilterTableModel(org::esb::hive::PresetReader::FilterList & filter,std::map<std::string, std::string > available_filter) : _filter(filter), _available_filter(available_filter), Wt::WStandardItemModel() {
          LOGDEBUG("FilterTableModel():Wt::WStandardItemModel()");
          insertColumns(0, 2);
          setHeaderData(0, std::string("Id"));
          setHeaderData(1, std::string("Filter"));
        }

        void refresh() {
          org::esb::hive::PresetReader::FilterList::iterator it = _filter.begin();
          for (int a=0; it != _filter.end(); it++, a++) {
            if (rowCount() < _filter.size()){
            LOGDEBUG("insert filter:"<<(*it).first);
              insertRow(rowCount());
              }
            if (rowCount() > _filter.size()){
            LOGDEBUG("remove filter:"<<(*it).first);
              removeRow(rowCount() - 1);
              }
            setData(a, 0, (*it).first);
            setData(a, 1, _available_filter[(*it).first]);
          }
        }

        ~PresetFilterTableModel() {
          LOGDEBUG("~PresetFilterTableModel()");
        }
      private:
        org::esb::hive::PresetReader::FilterList & _filter;
        std::map<std::string, std::string > _available_filter;
      };

      PresetFilterTable::PresetFilterTable(org::esb::hive::PresetReader::FilterList & filter) : _filter(filter), Wt::Ext::TableView() {
        LOGDEBUG("FilterTable::FilterTable() : Wt::Ext::TableView()");

        _available_filter["resize"] = "Resize Filter";
        _available_filter["deinterlace"] = "Deinterlace Filter";

        setModel(_model = new PresetFilterTableModel(filter, _available_filter));
        cellClicked().connect(SLOT(this, PresetFilterTable::itemSelectionChangedMethod));
        resize(500, 200);
        setColumnWidth(0, 50);
        setAutoExpandColumn(1);
        setSelectionBehavior(Wt::SelectRows);
        setSelectionMode(Wt::SingleSelection);
        /*
         * setting up the toolbar
         */
        setTopToolBar(new Wt::Ext::ToolBar());
        Wt::Ext::Button * addOptionButton = topToolBar()->addButton("Add Filter");
        addOptionButton->setIcon("icons/add-icon.png");
        addOptionButton->clicked().connect(SLOT(this, PresetFilterTable::addFilter));
        topToolBar()->addSeparator();
        removeOptionButton = topToolBar()->addButton("Remove Filter");
        removeOptionButton->setIcon("icons/remove-icon.png");
        removeOptionButton->setEnabled(false);
        removeOptionButton->clicked().connect(SLOT(this, PresetFilterTable::removeFilter));

        static_cast<PresetFilterTableModel*> (model())->refresh();

      }

      PresetFilterTable::~PresetFilterTable() {
        //static_cast<PresetFilterTableModel*> (model())->refresh();
        LOGDEBUG("PresetFilterTable::~PresetFilterTable() ");
      }

      void PresetFilterTable::addFilter() {
        _chooser = new FilterChooser(_available_filter);
        _chooser->selected.connect(SLOT(this, PresetFilterTable::filterSelected));
        _chooser->show();
        //if(_chooser->exec()==Wt::Ext::Dialog::Accepted){}
      }

      void PresetFilterTable::filterSelected() {
        std::list<std::string> result = _chooser->getSelectedFilter();
        std::string filtername=result.front();
        _filter[filtername];
        //LOGDEBUG("Filter selected:"<<result.front()->filtername);
        //result.front()->update();
        //_profile->filter().link(*result.front().get());
        static_cast<PresetFilterTableModel*> (model())->refresh();
        _chooser->accept();
        _chooser.reset();
      }

      void PresetFilterTable::itemSelectionChangedMethod() {
        removeOptionButton->setEnabled(true);
      }

      void PresetFilterTable::refresh() {

      }

      void PresetFilterTable::removeFilter() {
        LOGDEBUG("remove filter");
        /*
        std::string filtername=boost::any_cast<std::string > (model()->data(selectedRows()[0], 1));
        std::vector<db::Filter> filter=_profile->filter().get().all();
        std::vector<db::Filter>::iterator it=filter.begin();
        for(;it!=filter.end();it++){
          if((*it).filtername==filtername){
            _profile->filter().unlink((*it));
            LOGDEBUG("filter removed:"<<(*it).filtername);
          }
        }
         */
        static_cast<PresetFilterTableModel*> (model())->refresh();
      }
    }
  }
}
