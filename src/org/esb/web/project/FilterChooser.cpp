#include "FilterChooser.h"

#include "Wt/WFitLayout"
#include "Wt/WStandardItemModel"
#include "Wt/Ext/TableView"
#include "Wt/Ext/Button"
#include "org/esb/util/StringUtil.h"
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

        void setFilterData(std::vector<db::Filter> filters){
          std::vector<db::Filter>::iterator it=filters.begin();
          for(int a = 0;it!=filters.end();it++, a++){
            if (rowCount() <= a)
              insertRow(rowCount());

            setData(a, 0, org::esb::util::StringUtil::toString((*it).id));
            setData(a, 1, (std::string)(*it).filtername);

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

        void setProfiles(std::vector<db::Filter> filters){
          static_cast<FilterTableModel*>(model())->setFilterData(filters);
        }
      };



      FilterChooser::FilterChooser():Wt::Ext::Dialog("Filter Chooser"){
        setLayout(new Wt::WFitLayout());
        resize(400,300);
        _filter_table=Ptr<FilterTable>(new FilterTable());
        layout()->addWidget(_filter_table.get());

        addButton(new Wt::Ext::Button("Select Filter"));
        buttons().back()->clicked.connect(SLOT(this, FilterChooser::choose));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked.connect(SLOT(this, Wt::Ext::Dialog::reject));

      }
        
      FilterChooser::~FilterChooser(){
      }
      void       FilterChooser::choose(){

      }
      void       FilterChooser::cancel(){
      }
        
    }
  }
}



