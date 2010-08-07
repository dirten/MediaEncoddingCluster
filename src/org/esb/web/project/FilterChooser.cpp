#include "FilterChooser.h"

#include "Wt/WFitLayout"
#include "Wt/WStandardItemModel"
#include "Wt/Ext/TableView"
#include "Wt/Ext/Button"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
namespace org{
  namespace esb{
    namespace web{



      class FilterTableModel:public Wt::WStandardItemModel{
      public:
        FilterTableModel(){
          insertColumns(0, 3);
          setHeaderData(0, std::string("Id"));
          setHeaderData(1, std::string("Name"));

        }

        void setFilterData(std::vector<Ptr<db::Filter> > filters){
          std::vector<Ptr<db::Filter> >::iterator it=filters.begin();
          for(int a = 0;it!=filters.end();it++, a++){
            if (rowCount() <= a)
              insertRow(rowCount());

            setData(a, 0, org::esb::util::StringUtil::toString((*it)->id));
            setData(a, 1, (std::string)(*it)->filtername);

          }
        }
      };
      class FilterTable:public Wt::Ext::TableView{
      public:
        FilterTable(){
          setModel(new FilterTableModel());
          setColumnWidth(0,50);
          setColumnWidth(1,300);
        }

        void setFilter(std::vector<Ptr<db::Filter> > filters){
          static_cast<FilterTableModel*>(model())->setFilterData(filters);
        }
      };



      FilterChooser::FilterChooser(std::vector<Ptr<db::Filter> > available_filter):
      Wt::Ext::Dialog("Filter Chooser"),
        _available_filters(available_filter){
          setLayout(new Wt::WFitLayout());
          resize(400,300);
          selected.setBlocked(false);
          _filter_table=Ptr<FilterTable>(new FilterTable());
          _filter_table->setFilter(available_filter);
          _filter_table->setAlternatingRowColors(true);
          _filter_table->setHighlightMouseOver(true);
          _filter_table->setSelectionBehavior(Wt::SelectRows);
          _filter_table->setSelectionMode(Wt::SingleSelection);
          _filter_table->itemSelectionChanged.connect(SLOT(this, FilterChooser::enableButtons));

          layout()->addWidget(_filter_table.get());


          addButton(new Wt::Ext::Button("Select Filter"));
          buttons().back()->clicked.connect(SLOT(this, FilterChooser::choose));
          buttons().back()->setEnabled(false);

          addButton(new Wt::Ext::Button("Cancel"));
          buttons().back()->clicked.connect(SLOT(this, Wt::Ext::Dialog::reject));

      }

      FilterChooser::~FilterChooser(){
        LOGDEBUG("FilterChooser::~FilterChooser()")
      }

      void FilterChooser::choose(){
        std::string filtername=boost::any_cast<string > (_filter_table->model()->data(_filter_table->selectedRows()[0], 1));
        std::vector<Ptr<db::Filter> >::iterator it=_available_filters.begin();
        for(;it!=_available_filters.end();it++){
          if((*it)->filtername==filtername){
            selected.emit((*it));
          }
        }

        LOGDEBUG("filtername:"<<filtername);
        this->accept();
      }

      void FilterChooser::cancel(){
      }

      void FilterChooser::enableButtons(){
        if(_filter_table->selectedRows().size()>0){
          buttons().front()->setEnabled(true);
        }else{
          buttons().front()->setEnabled(false);
        }
      }

    }
  }
}



