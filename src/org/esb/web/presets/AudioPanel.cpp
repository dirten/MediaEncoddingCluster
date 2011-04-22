/* 
 * File:   AudioPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 17:13
 */

#include "AudioPanel.h"
#include "Wt/WFitLayout"
#include "Wt/WContainerWidget"
#include "Wt/WGridLayout"
#include "Wt/WBorderLayout"
#include "org/esb/av/AV.h"
#include "../wtk/KeyValueModel.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "../GuiBuilder.h"

namespace org {
  namespace esb {
    namespace web {

      AudioPanel::AudioPanel(std::map<std::string, std::string>&p) : _parameter(p), Wt::Ext::Panel() {
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
        codec_model->addModelData("noaudio", "No Audio");
        codec_model->addModelData("copy", "Stream Copy");
        AVCodec *codec = NULL;
        int a = 0;
        while ((codec = av_codec_next(codec))) {
          if (codec->encode && codec->type == AVMEDIA_TYPE_AUDIO) {
            if (avail_codecs.count(codec->name) > 0) {
              codec_model->addModelData(codec->name, codec->long_name);
            } else {
              if (avail_codecs.size() == 0)
                codec_model->addModelData(codec->name, codec->long_name);
            }
          }
        }

        _codec = new ComboBox();
        _codec->setModel(codec_model);
        _codec->setModelColumn(1);
        _codec->setSelectedEntry(_parameter["codec_id"]);
        _codec->setTextSize(50);
        _codec->resize(300, Wt::WLength());
        _codec->activated().connect(SLOT(this, AudioPanel::codecSelected));


        main_panel = new Wt::Ext::Panel();
        Wt::WFitLayout * fit = new Wt::WFitLayout();
        main_panel->setLayout(fit);
        //main_panel->setBorder(false);

        l->addWidget(main_panel, Wt::WBorderLayout::Center);


        Wt::Ext::Panel * top_panel = new Wt::Ext::Panel();
        top_panel->resize(Wt::WLength(), 40);
        top_panel->setLayout(new Wt::WFitLayout());
        top_panel->setBorder(false);

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
      }

      void AudioPanel::codecSelected() {
        std::string codecid = _codec->data(_codec->currentIndex());
        setCodecGui(codecid);
      }

      void AudioPanel::setCodecGui(std::string codecid) {
        LOGDEBUG("CodecId=" << codecid);

        _parameter["codec_id"] = codecid;
        std::string path = org::esb::config::Config::get("hive.base_path");
        std::string file = path;
        file += "/res/comp/encoder.audio.";
        file += codecid;
        file += ".gui";
        if (!org::esb::io::File(file).exists()) {
          file = path + "/res/comp/encoder.audio.default.gui";
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

      void AudioPanel::dataChanged() {

      }

      AudioPanel::~AudioPanel() {
      }
    }
  }
}
