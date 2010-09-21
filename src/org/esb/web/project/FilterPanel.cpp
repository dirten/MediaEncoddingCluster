#include "FilterPanel.h"

#include "Wt/WFitLayout"
#include "Wt/WStandardItemModel"
#include "Wt/Ext/TableView"
#include "Wt/Ext/ToolBar"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
namespace org{
  namespace esb{
    namespace web{
      class FilterTableModel:public Wt::WStandardItemModel{
      public:
        FilterTableModel(){
          insertColumns(0, 2);
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

        void setFilter(std::vector<db::Filter> filters){
          FilterTableModel*oldptr=static_cast<FilterTableModel*>(model());
          setModel(new FilterTableModel());
          delete oldptr;

          static_cast<FilterTableModel*>(model())->setFilterData(filters);
        }
      };




      FilterPanel::FilterPanel():Wt::Ext::Panel(){
        setTitle("Filter");
        setLayout(new Wt::WFitLayout());
        layout()->setContentsMargins(0, 0, 0, 0);

        _filter_table=Ptr<FilterTable>(new FilterTable());

        _filter_table->setAlternatingRowColors(true);
        _filter_table->setHighlightMouseOver(true);
        _filter_table->setSelectionBehavior(Wt::SelectRows);
        _filter_table->setSelectionMode(Wt::SingleSelection);
        _filter_table->itemSelectionChanged().connect(SLOT(this, FilterPanel::enableButtons));
        layout()->addWidget(_filter_table.get());
        setTopToolBar(new Wt::Ext::ToolBar());

        Wt::Ext::Button * addFilterButton = topToolBar()->addButton("Add Filter");
        addFilterButton->setIcon("icons/add-icon.png");
        topToolBar()->addSeparator();
        editFilterButton = topToolBar()->addButton("Edit Filter");
        editFilterButton->setIcon("icons/edit-icon.png");
        editFilterButton->setEnabled(false);
        topToolBar()->addSeparator();
        removeFilterButton = topToolBar()->addButton("Remove Filter");
        removeFilterButton->setIcon("icons/remove-icon.png");

        removeFilterButton->setEnabled(false);

        addFilterButton->clicked().connect(SLOT(this, FilterPanel::addFilter));
        removeFilterButton->clicked().connect(SLOT(this, FilterPanel::removeFilter));
        editFilterButton->clicked().connect(SLOT(this, FilterPanel::editFilter));
      }

      FilterPanel::~FilterPanel(){

      }

      void FilterPanel::setProject(Ptr<db::Project> p){
        _project=p;
        Ptr<db::Filter> filter=Ptr<db::Filter>(new db::Filter(p->getDatabase()));
        filter->filtername="Resize Filter";
        filter->filterid="resize";
        _available_filters.push_back(filter);
        filter=Ptr<db::Filter>(new db::Filter(p->getDatabase()));
        filter->filtername="Deinterlace Filter";
        filter->filterid="deinterlace";
        _available_filters.push_back(filter);
        _filter_table->setFilter(_project->filter().get().all());

      }
      void FilterPanel::filterSelected(Ptr<db::Filter> filter){
        filter->update();
        _project->filter().link(*filter);
/*
        std::vector<Ptr<db::Filter> >::iterator it=_available_filters.begin();
        for(;it!=_available_filters.end();it++){
          if((*it)->filtername==filter->filtername){
            break;
          }
        }
        _available_filters.erase(it);
*/
        _filter_table->setFilter(_project->filter().get().all());
        _filter_editor.reset();
        _filter_editor=Ptr<FilterEditor>(new FilterEditor(filter));
        _filter_editor->show();
//        _filter_editor->exec();
        LOGDEBUG(*filter);
      }

      void FilterPanel::addFilter(){
        _filter_chooser=Ptr<FilterChooser>(new FilterChooser(_available_filters));
        _filter_chooser->selected.connect(SLOT(this, FilterPanel::filterSelected));
        _filter_chooser->show();
//        _filter_chooser->exec();
//        LOGDEBUG("_filter_chooser->exec(); returned")
        //_filter_chooser.reset();

      }
      void FilterPanel::removeFilter(){
        int fid=atoi(boost::any_cast<string > (_filter_table->model()->data(_filter_table->selectedRows()[0], 0)).c_str());
        db::Filter filter=litesql::select<db::Filter>(_project->getDatabase(), db::Filter::Id==fid).one();
        _project->filter().unlink(filter);
        _filter_table->setFilter(_project->filter().get().all());
//        removeFilterButton->setEnabled(false);
      }
      void FilterPanel::editFilter(){
        int fid=atoi(boost::any_cast<string > (_filter_table->model()->data(_filter_table->selectedRows()[0], 0)).c_str());
        Ptr<db::Filter> filter=Ptr<db::Filter>(new db::Filter(litesql::select<db::Filter>(_project->getDatabase(), db::Filter::Id==fid).one()));
        _filter_editor=Ptr<FilterEditor>(new FilterEditor(filter));
        _filter_editor->show();
//        removeFilterButton->setEnabled(false);
      }

      void FilterPanel::enableButtons(){
        if(_filter_table->selectedRows().size()>0){
          removeFilterButton->setEnabled(true);
          editFilterButton->setEnabled(true);
        }else{
          removeFilterButton->setEnabled(false);
          editFilterButton->setEnabled(false);
        }
      }
    }
  }
}

