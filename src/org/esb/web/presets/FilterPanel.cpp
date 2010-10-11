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
#include "org/esb/web/presets/FilterTable.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace web {

      PresetFilterPanel::PresetFilterPanel(Ptr<db::Profile> p) : Wt::Ext::Panel() {
        LOGDEBUG("PresetFilterPanel::PresetFilterPanel(Ptr<db::Profile> p):Wt::Ext::Panel()");
        setLayout(new Wt::WFitLayout());

        Wt::WContainerWidget * main = new Wt::WContainerWidget();
        Wt::WGridLayout * grid = new Wt::WGridLayout();
        main->setLayout(grid);
        layout()->addWidget(main);

        Ptr<db::Filter> f = new db::Filter(p->getDatabase());
        f->filterid = "resize";
        f->filtername = "Resize Filter";
        _available_filter.push_back(f);
        Ptr<db::Filter> f2 = new db::Filter(p->getDatabase());
        f2->filterid = "deinterlace";
        f2->filtername = "Deinterlace Filter";
        _available_filter.push_back(f2);

        org::esb::web::PresetFilterTable * filter_table = new PresetFilterTable(_available_filter);

        grid->addWidget(filter_table, 0, 0);
      }

      PresetFilterPanel::~PresetFilterPanel() {
      }
    }
  }
}
