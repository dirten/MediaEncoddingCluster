/* 
 * File:   VideoPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 17:12
 */
#include "Wt/Ext/MessageBox"

#include "VideoPanel.h"
#include "Wt/WFitLayout"
#include "Wt/WContainerWidget"
#include "Wt/WGridLayout"
#include "org/esb/av/Decoder.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/ScopedTimeCounter.h"
#include "org/esb/hive/DatabaseService.h"
#include "Wt/Ext/Button"
#include "Wt/WStandardItemModel"
#include "../wtk/KeyValueModel.h"
#include "litesql/persistent.hpp"
#include "../GuiBuilder.h"
#include "Wt/WBorderLayout"
#include "org/esb/io/File.h"
/*on windows there is a macro defined with name MessageBox*/
#ifdef MessageBox
#undef MessageBox
#endif
namespace org {
  namespace esb {
    namespace web {

      VideoPanel::VideoPanel(std::map<std::string, std::string>& p) : _parameter(p), Wt::Ext::Panel() {
        //        setLayout(new Wt::WFitLayout());
        Wt::WBorderLayout*l = new Wt::WBorderLayout();
        setLayout(l);
        std::set<std::string> avail_codecs;
        if (_parameter.count("available_codecs") > 0) {
          LOGDEBUG("Available codes" << _parameter["available_codecs"]);
          org::esb::util::StringTokenizer st(_parameter["available_codecs"], ",");
          while (st.hasMoreTokens()) {
            std::string codec_id = st.nextToken();
            avail_codecs.insert(codec_id);
            LOGDEBUG("avalable codec list" << codec_id);
          }
        }
        /*
         * Combobox for the Codec Selector
         */
        KeyValueModel * codec_model = new KeyValueModel();
        codec_model->addModelData("novideo", "No Video");
        codec_model->addModelData("copy", "Stream Copy");

        AVCodec *codec = NULL;
        int a = 0;
        while ((codec = av_codec_next(codec))) {
          if (codec->encode && codec->type == AVMEDIA_TYPE_VIDEO) {
            if (avail_codecs.count(codec->name) > 0) {
              codec_model->addModelData(codec->name, codec->long_name);
            } else {
              if (avail_codecs.size() == 0)
                codec_model->addModelData(codec->name, codec->long_name);
            }
          }
        }
        codec_model->sort(1);

        _codec = new ComboBox();

        _codec->setModel(codec_model);
        _codec->setModelColumn(1);
        _codec->setSelectedEntry(_parameter["codec_id"]);
        _codec->setTextSize(50);
        _codec->resize(300, Wt::WLength());


        _codec->activated().connect(SLOT(this, VideoPanel::codecSelected));


        main_panel = new Wt::Ext::Panel();
        Wt::WFitLayout * fit = new Wt::WFitLayout();
        main_panel->setLayout(fit);
        //main_panel->setBorder(false);

        l->addWidget(main_panel, Wt::WBorderLayout::Center);

        Wt::Ext::Panel * top_panel = new Wt::Ext::Panel();
        top_panel->resize(Wt::WLength(), 40);
        top_panel->setLayout(new Wt::WFitLayout());
        top_panel->setBorder(false);
        //        top_panel->layout()->addWidget(_codec);

        Wt::WContainerWidget *top = new Wt::WContainerWidget();
        top_panel->layout()->addWidget(top);
        Wt::WGridLayout * grid = new Wt::WGridLayout();
        top->setLayout(grid);
        top->resize(Wt::WLength(), 40);

        l->addWidget(top_panel, Wt::WBorderLayout::North);
        Wt::WLabel * label = new Wt::WLabel("Codec:");
        label->setBuddy(_codec);

        grid->addWidget(label, 0, 0);
        grid->addWidget(_codec, 0, 1);
        grid->addWidget(new Wt::WText(""), 0, 2);
        grid->setColumnStretch(1, 1);




        setCodecGui(_parameter["codec_id"]);
        //l->addWidget(builder,Wt::WBorderLayout::Center);
        //        return;
      }

      void VideoPanel::setCodecGui(std::string codecid) {
        LOGDEBUG("CodecId=" << codecid);

        if (_parameter["codec_id"].length() > 0 && _parameter["codec_id"] != codecid) {
          Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Data will be lost", "when the codec changed, all previous data set will be lost", Wt::Warning, Wt::Ok | Wt::Cancel);
          box->show();
          if (box->exec() == Wt::Ext::Dialog::Accepted) {
            _parameter["codec_id"] = codecid;
            LOGDEBUG("found codec_id="<<codecid);
            _parameter.clear();
          } else {
            _codec->setSelectedEntry(_parameter["codec_id"]);
            return;
          }
        }
        _parameter["codec_id"] = codecid;
        std::string path = org::esb::config::Config::get("hive.base_path");
        std::string file = path;
        file += "/res/comp/encoder.video.";
        file += codecid;
        file += ".gui";
        if (!org::esb::io::File(file).exists()) {
          file = path + "/res/comp/encoder.video.default.gui";
        }
        LOGDEBUG("Gui File:" << file);
        GuiBuilder * builder = new GuiBuilder(file, _parameter);
        //builder->resize(700, Wt::WLength());
        if (main_panel->layout()->count() > 0) {
          Wt::WLayoutItem * item = main_panel->layout()->itemAt(0);
          main_panel->layout()->removeItem(item);
          delete item->widget();
        }
        main_panel->layout()->addWidget(builder);
        main_panel->refresh();

      }

      void VideoPanel::codecSelected() {
        std::string codecid = _codec->data(_codec->currentIndex());
        setCodecGui(codecid);
      }

      void VideoPanel::switchAdvanced() {
        if (_advanced->text() == "Advanced>>>") {
          _advanced->setText("Simple<<<");
          //_advance_table->setEnabled(true);
          _advance_table->setCollapsed(false);
        } else {
          _advanced->setText("Advanced>>>");
          //_advance_table->setEnabled(false);
          _advance_table->setCollapsed(true);
        }
      }

      void VideoPanel::dataChanged() {
        std::map<std::string, ComboBox*> boxes = _elcb.getElements();
        std::map<std::string, ComboBox*>::iterator bit = boxes.begin();
        for (; bit != boxes.end(); bit++) {
          LOGDEBUG((*bit).first);
          _parameter[(*bit).first] = (*bit).second->currentSelected();
          LOGDEBUG("=" << _parameter[(*bit).first]);
        }
        _advance_table->refresh();
      }

      void VideoPanel::refresh() {
        std::map<std::string, ComboBox*> boxes = _elcb.getElements();
        std::map<std::string, ComboBox*>::iterator bit = boxes.begin();
        for (; bit != boxes.end(); bit++) {
          LOGDEBUG((*bit).first);
          if (!(*bit).second->setSelectedEntry(_parameter[(*bit).first])) {
            //(*bit).second->
          }
          //          _parameter[(*bit).first]=(*bit).second->currentSelected();
          LOGDEBUG("=" << _parameter[(*bit).first]);
        }
      }

      void VideoPanel::save() {
        return;
        org::esb::util::ScopedTimeCounter t("database profile update");
        _profile->getDatabase().begin();
        std::map<std::string, ComboBox*> boxes = _elcb.getElements();
        std::map<std::string, ComboBox*>::iterator bit = boxes.begin();
        for (; bit != boxes.end(); bit++) {
          LOGDEBUG((*bit).first);
          _parameter[(*bit).first] = (*bit).second->currentSelected();
          LOGDEBUG("=" << _parameter[(*bit).first]);
        }

        vector<db::ProfileParameter> params = _profile->params().get(db::ProfileParameter::Mediatype == (int) AVMEDIA_TYPE_VIDEO).all();
        vector<db::ProfileParameter>::iterator par = params.begin();
        for (; par != params.end(); par++) {
          (*par).del();
        }
        std::map<std::string, std::string>::iterator it = _parameter.begin();
        for (; it != _parameter.end(); it++) {
          db::ProfileParameter p(_profile->getDatabase());
          p.name = (*it).first;
          p.val = (*it).second;
          p.mediatype = (int) AVMEDIA_TYPE_VIDEO;
          p.update();
          LOGDEBUG(p);
          _profile->params().link(p);
        }
        _profile->getDatabase().commit();
      }

      VideoPanel::~VideoPanel() {
      }
    }
  }
}
