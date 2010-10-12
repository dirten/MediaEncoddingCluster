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

        
        grid->addWidget((filter_table = new PresetFilterTable(p)).get(), 0, 0);
        _cont=new Wt::WContainerWidget();
        _cont->setLayout(new Wt::WBorderLayout());
        _cont->resize(500,300);
        grid->addWidget(_cont, 0, 1);
        //filter_table->itemSelectionChanged().connect(SLOT(this,PresetFilterPanel::filterSelected));
        filter_table->cellClicked().connect(SLOT(this,PresetFilterPanel::filterSelected));
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
        }
        _props=FilterPanelFactory::getFilterPanel(filter->filterid);
        ((Wt::WBorderLayout*)_cont->layout())->addWidget(_props, Wt::WBorderLayout::Center);
//        _cont->layout()->addWidget(_props.get());
//        grid->addWidget((_props=FilterPanelFactory::getFilterPanel(filter->filterid)).get(), 0, 1);
        //_props-> =FilterPanelFactory::getFilterPanel(filter->filterid);
        ((BaseFilterPanel *)_props)->setFilter(filter);
      }
    }
  }
}
