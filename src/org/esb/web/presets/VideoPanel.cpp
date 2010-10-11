/* 
 * File:   VideoPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 17:12
 */

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

namespace org {
  namespace esb {
    namespace web {

      VideoPanel::VideoPanel(Ptr<db::Profile> p) : _profile(p), Wt::Ext::Panel() {
        setLayout(new Wt::WFitLayout());

        Wt::WContainerWidget * main = new Wt::WContainerWidget();
        Wt::WGridLayout * grid = new Wt::WGridLayout();
        main->setLayout(grid);
        layout()->addWidget(main);


        if (_profile->params().get().count() > 0) {
          vector<db::ProfileParameter> params = _profile->params().get().all();
          vector<db::ProfileParameter>::iterator it = params.begin();
          for (; it != params.end(); it++)
            if ((*it).mediatype.value() == AVMEDIA_TYPE_VIDEO)
              _parameter[(*it).name.value()] = (*it).val.value();
        }

        /*
         * Combobox for the Codec Selector
         */
        KeyValueModel * codec_model = new KeyValueModel();
        AVCodec *codec = NULL;
        int a = 0;
        while ((codec = av_codec_next(codec))) {
          if (codec->encode && codec->type == CODEC_TYPE_VIDEO) {
            codec_model->addModelData(org::esb::util::StringUtil::toString(codec->id), codec->long_name);
          }
        }

        ComboBox * v_codec = _elcb.addElement("codec", "Codec", "", grid);
        v_codec->setModel(codec_model);
        v_codec->setModelColumn(1);
        v_codec->setSelectedEntry(_parameter["codec"]);
        v_codec->setTextSize(50);
//        v_codec->activated().connect(SLOT(this, VideoPanel::setPredefinedCodecFlags));


         /*
         * Combobox for the Preset Selector
         */
        /*
        KeyValueModel * preset_model = new KeyValueModel();
        vector<db::CodecPreset> cPresets=litesql::select<db::CodecPreset>(p->getDatabase(),db::CodecPreset::Codecid==_parameter["codec"]).all();
        vector<db::CodecPreset>::iterator presetit=cPresets.begin();
        for(;presetit!=cPresets.end();presetit++){
          preset_model->addModelData((*presetit).preset,(*presetit).name);
        }


        ComboBox * presets = _elcb.addElement("preset", "Codec Preset", "", grid);

        presets->setModel(preset_model);
        presets->setModelColumn(1);
        presets->setSelectedEntry(_parameter["preset"]);
         */
//        v_keyframes->setEditable(true);

        /*
         * Combobox for the Encoding Methode Selector
         */
        /*
        KeyValueModel * model = new KeyValueModel();
        model->addModelData("pass1vbr", "1 Pass Variable Bitrate");
        model->addModelData("pass1cbr", "1 Pass Constant Bitrate");
        model->addModelData("pass1cq", "1 Pass Constant Quality");

        ComboBox * v_methode = _elcb.addElement("methode", "Methode", "", grid);
        v_methode->setModel(model);
        v_methode->setModelColumn(1);
        v_methode->setSelectedEntry(_parameter["methode"]);
        v_methode->setTextSize(50);
        v_methode->setEditable(false);
        */
        /*
         * Combobox for the Bitrate Selector
         */
        KeyValueModel * bitrate_model = new KeyValueModel();
        bitrate_model->addModelData("128000", "128 kbit/s");
        bitrate_model->addModelData("256000", "256 kbit/s");
        bitrate_model->addModelData("512000", "512 kbit/s");
        bitrate_model->addModelData("1024000", "1 Mbit/s");
        bitrate_model->addModelData("2048000", "2 Mbit/s");
        bitrate_model->addModelData("4096000", "4 Mbit/s");
        bitrate_model->addModelData("8192000", "8 Mbit/s");
        bitrate_model->addModelData("16384000", "16 Mbit/s");
        bitrate_model->addModelData("25600000", "25 Mbit/s");
        bitrate_model->addModelData("51200000", "50 Mbit/s");

        ComboBox * v_bitrate = _elcb.addElement("b", "Video Bitrate", "please select a Bitrate", grid);
        v_bitrate->setModel(bitrate_model);
        v_bitrate->setModelColumn(1);
        v_bitrate->setSelectedEntry(_parameter["b"]);
        v_bitrate->setEditable(true);


        /*
         * Combobox for the Framerate Selector
         */
        KeyValueModel * framerate_model = new KeyValueModel();
        framerate_model->addModelData("1:1", "1:1 same as source");
        framerate_model->addModelData("2:1", "2:1 half as source");
        framerate_model->addModelData("1/10", "10 Frames/s");
        framerate_model->addModelData("1/15", "15 Frames/s");
        framerate_model->addModelData("1000/23976", "23,97 Frames/s");
        framerate_model->addModelData("1/24", "24 Frames/s");
        framerate_model->addModelData("1/25", "25 Frames/s");
        framerate_model->addModelData("100/2997", "29,97 Frames/s");
        framerate_model->addModelData("1/30", "30 Frames/s");

        ComboBox * v_framerate = _elcb.addElement("framerate", "Video Framerate", _profile->vframerate.value(), grid);
        v_framerate->setModel(framerate_model);
        v_framerate->setModelColumn(1);
        v_framerate->setSelectedEntry(_parameter["framerate"]);
        v_framerate->setTextSize(50);
        v_framerate->setEditable(true);



        /*
         * Combobox for the Bitrate Selector
         */
        KeyValueModel * keyframe_model = new KeyValueModel();
        keyframe_model->addModelData("20", "20 Frames");
        keyframe_model->addModelData("50", "50 Frames");
        keyframe_model->addModelData("100", "100 Frames");
        keyframe_model->addModelData("200", "200 Frames");
        keyframe_model->addModelData("300", "300 Frames");

        ComboBox * v_keyframes = _elcb.addElement("g", "Key Frame every", "100 Frames", grid);

        v_keyframes->setModel(keyframe_model);
        v_keyframes->setModelColumn(1);
        v_keyframes->setSelectedEntry(_parameter["g"]);
        v_keyframes->setEditable(true);

        //        v_keyframes->activated().connect(SLOT(this, VideoPanel::dataChanged));
        std::map<std::string, ComboBox*> boxes = _elcb.getElements();
        std::map<std::string, ComboBox*>::iterator bit = boxes.begin();
        for (; bit != boxes.end(); bit++) {
          (*bit).second->activated().connect(SLOT(this, VideoPanel::dataChanged));
        }

        //        _advance_table->load();
        //        _advance_table->setEnabled(false);

        /*these are alwyays the last widget(except Advanced Button) to stretch to the buttom of the Page*/
        Wt::WText* text=new Wt::WText("");
        text->resize(Wt::WLength(), 500);
        grid->addWidget(text, grid->rowCount(), 0, 0, 3);

        grid->setRowStretch(grid->rowCount() - 1, 1);

        _advance_table = new VideoAdvanceTableView(_parameter, AV_OPT_FLAG_VIDEO_PARAM);
        grid->addWidget(_advance_table, 0, 2, 7, 0, Wt::AlignRight);
        _advance_table->setCollapsible(true);
        _advance_table->setCollapsed(true);
        _advance_table->changed.connect(SLOT(this, VideoPanel::refresh));


        _advanced = new Wt::Ext::Button("Advanced>>>");
        _advanced->resize(100, Wt::WLength());
        grid->addWidget(_advanced, grid->rowCount(), 0, 0, 1, Wt::AlignRight);
        _advanced->clicked().connect(SLOT(this, VideoPanel::switchAdvanced));
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
          if(!(*bit).second->setSelectedEntry(_parameter[(*bit).first])){
            //(*bit).second->
          }
          //          _parameter[(*bit).first]=(*bit).second->currentSelected();
          LOGDEBUG("=" << _parameter[(*bit).first]);
        }
      }

      void VideoPanel::save() {
        org::esb::util::ScopedTimeCounter t("database profile update");
        _profile->getDatabase().begin();
        std::map<std::string, ComboBox*> boxes = _elcb.getElements();
        std::map<std::string, ComboBox*>::iterator bit = boxes.begin();
        for (; bit != boxes.end(); bit++) {
          LOGDEBUG((*bit).first);
          _parameter[(*bit).first] = (*bit).second->currentSelected();
          LOGDEBUG("=" << _parameter[(*bit).first]);
        }

        vector<db::ProfileParameter> params = _profile->params().get(db::ProfileParameter::Mediatype==(int)AVMEDIA_TYPE_VIDEO).all();
        vector<db::ProfileParameter>::iterator par = params.begin();
        for (; par != params.end(); par++) {
          (*par).del();
        }
        std::map<std::string, std::string>::iterator it = _parameter.begin();
        for (; it != _parameter.end(); it++) {
          db::ProfileParameter p(_profile->getDatabase());
          p.name = (*it).first;
          p.val = (*it).second;
          p.mediatype = (int)AVMEDIA_TYPE_VIDEO;
          p.update();
          _profile->params().link(p);
        }
        _profile->getDatabase().commit();
      }

      VideoPanel::~VideoPanel() {
      }
    }
  }
}
