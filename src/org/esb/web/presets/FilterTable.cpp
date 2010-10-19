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

        PresetFilterTableModel(Ptr<db::Profile> p) : _profile(p), Wt::WStandardItemModel() {
          LOGDEBUG("FilterTableModel():Wt::WStandardItemModel()");
          insertColumns(0, 2);
          setHeaderData(0, std::string("Enabled"));
          setHeaderData(1, std::string("Filter"));
        }

        void refresh() {
          std::vector<db::Filter> filter = _profile->filter().get().all();
          std::vector<db::Filter>::iterator it = filter.begin();
          for (; it != filter.end(); it++) {
            if(rowCount()<filter.size())
              insertRow(rowCount());
            if(rowCount()>filter.size())
              removeRow(rowCount()-1);
            setData(rowCount() - 1, 1, (*it).filtername.value());
          }
        }
        ~PresetFilterTableModel(){
          LOGDEBUG("~PresetFilterTableModel()");
        }
      private:
        Ptr<db::Profile> _profile;
      };

      PresetFilterTable::PresetFilterTable(Ptr<db::Profile> p) : _profile(p),Wt::Ext::TableView() {
        LOGDEBUG("FilterTable::FilterTable() : Wt::Ext::TableView()");

        Ptr<db::Filter> f = new db::Filter(p->getDatabase());
        f->filterid = "resize";
        f->filtername = "Resize Filter";
        _available_filter.push_back(f);

        Ptr<db::Filter> f2 = new db::Filter(p->getDatabase());
        f2->filterid = "deinterlace";
        f2->filtername = "Deinterlace Filter";
        _available_filter.push_back(f2);

        setModel(_model=new PresetFilterTableModel(p));
        cellClicked().connect(SLOT(this, PresetFilterTable::itemSelectionChangedMethod));
        resize(300, 300);
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
        std::list<Ptr<db::Filter> > result = _chooser->getSelectedFilter();
        LOGDEBUG("Filter selected:"<<result.front()->filtername);
        result.front()->update();
        _profile->filter().link(*result.front().get());
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
        std::string filtername=boost::any_cast<std::string > (model()->data(selectedRows()[0], 1));
        std::vector<db::Filter> filter=_profile->filter().get().all();
        std::vector<db::Filter>::iterator it=filter.begin();
        for(;it!=filter.end();it++){
          if((*it).filtername==filtername){
            _profile->filter().unlink((*it));
            LOGDEBUG("filter removed:"<<(*it).filtername);
          }
        }

        static_cast<PresetFilterTableModel*> (model())->refresh();
      }
    }
  }
}
