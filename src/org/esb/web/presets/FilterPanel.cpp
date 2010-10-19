/* 
 * File:   FilterPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 11. Oktober 2010, 14:29
 */

#include "FilterPanel.h"
#include "Wt/WFitLayout"
#include "Wt/WTable"
#include "Wt/WTableCell"
#include "Wt/WText"
#include "Wt/WGroupBox"
#include "org/esb/web/wtk/ComboBox.h"
#include "Wt/WHBoxLayout"
#include "Wt/WVBoxLayout"
#include "org/esb/web/wtk/KeyValueModel.h"
#include "org/esb/av/AV.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
#include "org/esb/web/project/FilterPanelFactory.h"
#include "Wt/WBorderLayout"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "../GuiBuilder.h"
namespace org {
  namespace esb {
    namespace web {

      PresetFilterPanel::PresetFilterPanel(Ptr<db::Profile> p) :_profile(p),  Wt::Ext::Panel() {
        LOGDEBUG("PresetFilterPanel::PresetFilterPanel(Ptr<db::Profile> p):Wt::Ext::Panel()");
        setLayout(new Wt::WFitLayout());

        Wt::WContainerWidget * main = new Wt::WContainerWidget();
        grid = new Wt::WGridLayout();
        main->setLayout(grid);
        layout()->addWidget(main);

        
        grid->addWidget(filter_table = new PresetFilterTable(p), 0, 0);
        _cont=new Wt::WContainerWidget();
        _cont->setLayout(new Wt::WBorderLayout());
        _cont->resize(500,300);
        grid->addWidget(_cont, 0, 1);
        filter_table->itemSelectionChanged().connect(SLOT(this,PresetFilterPanel::filterSelected));
        //filter_table->cellClicked().connect(SLOT(this,PresetFilterPanel::filterSelected));
      }

      PresetFilterPanel::~PresetFilterPanel() {
        LOGDEBUG("PresetFilterPanel::~PresetFilterPanel()");
      }

      void PresetFilterPanel::filterSelected() {
        std::string filtername=boost::any_cast<std::string > (filter_table->model()->data(filter_table->selectedRows()[0], 1));
        Ptr<db::Filter> filter=new db::Filter(_profile->filter().get(db::Filter::Filtername==filtername).one());
        if(_cont->layout()->count()>0){
          Wt::WLayoutItem * item =((Wt::WBorderLayout*)_cont->layout())->itemAt(Wt::WBorderLayout::Center);
          _cont->layout()->removeItem(item);
          //delete item;
        }
        std::string path = org::esb::config::Config::get("hive.base_path");
        std::string file = path;
        file += "/res/comp/filter.video.";
        file += filter->filterid;
        file += ".gui";
        if (!org::esb::io::File(file).exists()) {
          LOGDEBUG("Gui File does not exist:" << file);
          file = path + "/res/comp/test.gui";
        }
        LOGDEBUG("Gui File:" << file);
        _parameter.clear();
        std::vector<db::FilterParameter> params=filter->parameter().get().all();
        std::vector<db::FilterParameter>::iterator it=params.begin();
        for(;it!=params.end();it++){
          _parameter[(*it).fkey]=(*it).fval;
        }
//        return;
        GuiBuilder * builder = new GuiBuilder(file, _parameter);
        builder->dataChanged.connect(SLOT(this,PresetFilterPanel::save));
//        if (main_panel->layout()->count() > 0) {
//          Wt::WLayoutItem * item = main_panel->layout()->itemAt(0);
//          main_panel->layout()->removeItem(item);
//          delete item->widget();
//        }
//        main_panel->layout()->addWidget(builder);
//        main_panel->refresh();

        _props=builder;
        ((Wt::WBorderLayout*)_cont->layout())->addWidget(_props, Wt::WBorderLayout::Center);
//        _cont->layout()->addWidget(_props.get());
//        grid->addWidget((_props=FilterPanelFactory::getFilterPanel(filter->filterid)).get(), 0, 1);
        //_props-> =FilterPanelFactory::getFilterPanel(filter->filterid);
        //((BaseFilterPanel *)_props)->setFilter(filter);
      }
      void PresetFilterPanel::save(){
        LOGDEBUG("Save Filter Parameter");
        _profile->getDatabase().begin();
        std::string filtername=boost::any_cast<std::string > (filter_table->model()->data(filter_table->selectedRows()[0], 1));
        Ptr<db::Filter> filter=new db::Filter(_profile->filter().get(db::Filter::Filtername==filtername).one());
        std::vector<db::FilterParameter> params=filter->parameter().get().all();
        std::vector<db::FilterParameter>::iterator it=params.begin();
        for(;it!=params.end();it++){
          filter->parameter().unlink((*it));
          (*it).del();
        }
        std::map<std::string, std::string>::iterator itout=_parameter.begin();
        for(;itout!=_parameter.end();itout++){
          db::FilterParameter p(_profile->getDatabase());
          p.fkey=(*itout).first;
          p.fval=(*itout).second;
          p.update();
          filter->parameter().link(p);
        }
        _profile->getDatabase().commit();
      }
    }
  }
}
