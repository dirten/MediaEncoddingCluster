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
#include "org/esb/av/AV.h"
#include "../wtk/KeyValueModel.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace web {

      AudioPanel::AudioPanel(Ptr<db::Profile> p) : _profile(p), Wt::Ext::Panel() {
        setLayout(new Wt::WFitLayout());
        Wt::WContainerWidget * main = new Wt::WContainerWidget();
        Wt::WGridLayout * grid = new Wt::WGridLayout();
        main->setLayout(grid);
        layout()->addWidget(main);

        if (_profile->params().get().count() > 0) {
          std::vector<db::ProfileParameter> params = _profile->params().get().all();
          std::vector<db::ProfileParameter>::iterator it = params.begin();
          for (; it != params.end(); it++)
            if((*it).mediatype.value()==AVMEDIA_TYPE_AUDIO)
              _parameter[(*it).name.value()] = (*it).val.value();
        }
        /*
         * Combobox for the Codec Selector
         */
        KeyValueModel * codec_model = new KeyValueModel();
        AVCodec *codec = NULL;
        int a = 0;
        while ((codec = av_codec_next(codec))) {
          if (codec->encode && codec->type == CODEC_TYPE_AUDIO) {
            codec_model->addModelData(org::esb::util::StringUtil::toString(codec->id), codec->long_name);
          }
        }

        ComboBox * v_codec = _elcb.addElement("codec", "Codec", "", grid);
        v_codec->setModel(codec_model);
        v_codec->setModelColumn(1);
        v_codec->setSelectedEntry(_parameter["codec"]);
        v_codec->setTextSize(50);
        //        v_codec->activated().connect(SLOT(this, VideoPanel::setPredefinedCodecFlags));


        KeyValueModel * channel_model = new KeyValueModel();
        channel_model->addModelData("1","Mono");
        channel_model->addModelData("2","Stereo");
        channel_model->addModelData("3","Surround");
        channel_model->addModelData("4","Quad");
        channel_model->addModelData("5","5.0");
        channel_model->addModelData("6","5.1");
        channel_model->addModelData("8","7.1");

        ComboBox * a_channels = _elcb.addElement("channels", "Audio Channels", "", grid);
        a_channels->setTextSize(50);
        a_channels->setModel(channel_model);
        a_channels->setModelColumn(1);
        a_channels->setSelectedEntry(_parameter["channels"]);


        //"32000,96000,128000,160000,192000,224000,256000,320000"
        KeyValueModel * bitrate_model = new KeyValueModel();
        bitrate_model->addModelData("32000","32 kb/s");
        bitrate_model->addModelData("96000","96 kb/s");
        bitrate_model->addModelData("128000","128 kb/s");
        bitrate_model->addModelData("160000","160 kb/s");
        bitrate_model->addModelData("192000","192 kb/s");
        bitrate_model->addModelData("224000","224 kb/s");
        bitrate_model->addModelData("256000","256 kb/s");
        bitrate_model->addModelData("320000","320 kb/s");

        ComboBox * a_bitrate = _elcb.addElement("ab", "Audio Bitrate", "", grid);
        a_bitrate->setModel(bitrate_model);
        a_bitrate->setModelColumn(1);
        a_bitrate->setSelectedEntry(_parameter["ab"]);

        //"8000,11025,22050,32000,44100,48000"
        KeyValueModel * samplerate_model = new KeyValueModel();
        samplerate_model->addModelData("8000","8 kHz");
        samplerate_model->addModelData("11025","11,025 kHz ");
        samplerate_model->addModelData("22050","22,05 kHz");
        samplerate_model->addModelData("32000","32 kHz");
        samplerate_model->addModelData("44100","44,1 kHz");
        samplerate_model->addModelData("48000","48 kHz");

        ComboBox * a_samplerate = _elcb.addElement("samplerate", "Audio Samplerate", "", grid);
        a_samplerate->setModel(samplerate_model);
        a_samplerate->setModelColumn(1);
        a_samplerate->setSelectedEntry(_parameter["samplerate"]);


        std::map<std::string, ComboBox*> boxes = _elcb.getElements();
        std::map<std::string, ComboBox*>::iterator bit = boxes.begin();
        for (; bit != boxes.end(); bit++) {
          (*bit).second->activated().connect(SLOT(this, AudioPanel::dataChanged));
        }

        /*these are alwyays the last widget(except Advanced Button) to stretch to the buttom of the Page*/
        grid->addWidget(new Wt::WText(), grid->rowCount(), 0,0,3);
        grid->setRowStretch(grid->rowCount() - 1, 1);

        _advance_table=new VideoAdvanceTableView(_parameter,AV_OPT_FLAG_AUDIO_PARAM);
        grid->addWidget(_advance_table,0,2,grid->rowCount(),0,Wt::AlignRight);
        _advance_table->setCollapsible(true);
        _advance_table->setCollapsed(true);
        _advance_table->changed.connect(SLOT(this,AudioPanel::refresh));


        _advanced = new Wt::Ext::Button("Advanced>>>");
        _advanced->resize(100, Wt::WLength());
        grid->addWidget(_advanced, grid->rowCount(), 0, 0, 1,Wt::AlignRight);
        _advanced->clicked().connect(SLOT(this, AudioPanel::switchAdvanced));

      }

      void AudioPanel::switchAdvanced() {
        if (_advanced->text() == "Advanced>>>") {
          _advanced->setText("Simple<<<");
        _advance_table->setCollapsed(false);
        } else {
          _advanced->setText("Advanced>>>");
        _advance_table->setCollapsed(true);
        }
      }
      void AudioPanel::refresh() {
        std::map<std::string, ComboBox*> boxes = _elcb.getElements();
        std::map<std::string, ComboBox*>::iterator bit = boxes.begin();
        for (; bit != boxes.end(); bit++) {
          LOGDEBUG((*bit).first);
          (*bit).second->setSelectedEntry(_parameter[(*bit).first]);
          LOGDEBUG("=" << _parameter[(*bit).first]);
        }
      }
      void AudioPanel::dataChanged() {
        std::map<std::string, ComboBox*> boxes = _elcb.getElements();
        std::map<std::string, ComboBox*>::iterator bit = boxes.begin();
        for (; bit != boxes.end(); bit++) {
          LOGDEBUG((*bit).first);
          _parameter[(*bit).first] = (*bit).second->currentSelected();
          LOGDEBUG("=" << _parameter[(*bit).first]);
        }
        _advance_table->refresh();
      }
      void AudioPanel::save() {
        //org::esb::util::ScopedTimeCounter t("database profile update");
        _profile->getDatabase().begin();
        std::map<std::string, ComboBox*> boxes = _elcb.getElements();
        std::map<std::string, ComboBox*>::iterator bit = boxes.begin();
        for (; bit != boxes.end(); bit++) {
          LOGDEBUG((*bit).first);
          _parameter[(*bit).first] = (*bit).second->currentSelected();
          LOGDEBUG("=" << _parameter[(*bit).first]);
        }

        vector<db::ProfileParameter> params = _profile->params().get(db::ProfileParameter::Mediatype==(int)AVMEDIA_TYPE_AUDIO).all();
        vector<db::ProfileParameter>::iterator par = params.begin();
        for (; par != params.end(); par++) {
          (*par).del();
        }
        std::map<std::string, std::string>::iterator it = _parameter.begin();
        for (; it != _parameter.end(); it++) {
          db::ProfileParameter p(_profile->getDatabase());
          p.name = (*it).first;
          p.val = (*it).second;
          p.mediatype = (int)AVMEDIA_TYPE_AUDIO;
          p.update();
          _profile->params().link(p);
        }
        _profile->getDatabase().commit();
      }


      AudioPanel::~AudioPanel() {
      }
    }
  }
}
