/* 
 * File:   FormatPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 17:10
 */

#include "FormatPanel.h"
#include "Wt/WFitLayout"
#include "Wt/WBorderLayout"
#include "Wt/WContainerWidget"
#include "Wt/WGridLayout"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/util/StringTokenizer.h"
#include "../wtk/KeyValueModel.h"
#include "../GuiBuilder.h"
#include "org/esb/config/config.h"
namespace org {
  namespace esb {
    namespace web {

      FormatPanel::FormatPanel(std::map<std::string, std::string> & params) : _parameter(params), Wt::Ext::Panel() {
        
        Wt::WBorderLayout*l = new Wt::WBorderLayout();
        setLayout(l);
        std::string path = org::esb::config::Config::get("hive.base_path");
        std::string file = path;
        file += "/res/comp/format.";
//        file += formatid;
//        file += ".gui";
        
        KeyValueModel * format_model = new KeyValueModel();
        AVOutputFormat *ofmt = NULL;
        int a = 0;
        while ((ofmt = av_oformat_next(ofmt))) {
          if (org::esb::io::File(file+ofmt->name+".gui").exists())
            format_model->addModelData(ofmt->name,ofmt->long_name);
//          v_format->addItem(ofmt->long_name);
          a++;
        }

        _format = new ComboBox();
        _format->setTextSize(50);
        _format->resize(150, Wt::WLength::Auto);
        _format->setModel(format_model);
        _format->setModelColumn(1);
//        _format->setSelectedEntry(_parameter["file_format"]);
        _format->setSelectedEntry(_parameter["id"]);
        _format->activated().connect(SLOT(this, FormatPanel::setSelectedFormat));
        
        main_panel = new Wt::Ext::Panel();
        Wt::WFitLayout * fit = new Wt::WFitLayout();
        main_panel->setLayout(fit);
        main_panel->setBorder(false);

        l->addWidget(main_panel, Wt::WBorderLayout::Center);

        Wt::Ext::Panel * top_panel = new Wt::Ext::Panel();
        top_panel->resize(Wt::WLength(), 40);
        top_panel->setLayout(new Wt::WFitLayout());
        top_panel->setBorder(false);


        Wt::WContainerWidget *top = new Wt::WContainerWidget();
        top_panel->layout()->addWidget(top);
        Wt::WGridLayout * grid=new Wt::WGridLayout();
        top->setLayout(grid);
        top->resize(Wt::WLength(), 40);

        l->addWidget(top_panel, Wt::WBorderLayout::North);
        Wt::WLabel * label = new Wt::WLabel("File Format:");
        label->setBuddy(_format);

        grid->addWidget(label,0,0);
        grid->addWidget(_format,0,1);
        grid->addWidget(new Wt::WText(""),0,2);
        grid->setColumnStretch(1,1);
//        setSelectedFormat();
        setFormatGui(_parameter["id"]);

      }
      void FormatPanel::setSelectedFormat(){
        setFormatGui(_format->currentSelected());
      }

      void FormatPanel::setFormatGui(std::string formatid){
        _parameter["id"]=formatid;
        std::string path = org::esb::config::Config::get("hive.base_path");
        std::string file = path;
        file += "/res/comp/format.";
        file += formatid;
        file += ".gui";
        if (!org::esb::io::File(file).exists()) {
          LOGDEBUG("Gui File does not exist:" << file);
          file = path + "/res/comp/test.gui";
        }
        LOGDEBUG("Gui File:" << file);
        GuiBuilder * builder = new GuiBuilder(file, _parameter);
        if (main_panel->layout()->count() > 0) {
          Wt::WLayoutItem * item = main_panel->layout()->itemAt(0);
          main_panel->layout()->removeItem(item);
          delete item->widget();
        }
        main_panel->layout()->addWidget(builder);
        main_panel->refresh();

      }
      
      void FormatPanel::save(){
        _parameter["file_format"]=_format->currentSelected();
//        _profile->format=_format->currentSelected();
//        _profile->formatext=_parameter["fileExtension"];
      }
      
      FormatPanel::~FormatPanel() {
      }
    }
  }
}
