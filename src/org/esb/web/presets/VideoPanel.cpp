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
namespace org {
  namespace esb {
    namespace web {


      VideoPanel::VideoPanel(Ptr<db::Profile> p) : _profile(p), Wt::Ext::Panel() {
        setLayout(new Wt::WFitLayout());
        Wt::WContainerWidget * main = new Wt::WContainerWidget();
        Wt::WGridLayout * grid = new Wt::WGridLayout();
        main->setLayout(grid);
        layout()->addWidget(main);
        if(_profile->params().get().count()>0){
          vector<db::ProfileParameter> params=_profile->params().get().all();
          vector<db::ProfileParameter>::iterator it=params.begin();
          for(;it!=params.end();it++)
          _parameter[(*it).name.value()]=(*it).val.value();
        }
        /*
         * Combobox for the Codec Selector
         */
        Wt::Ext::ComboBox * v_codec = _elcb.addElement("v_codec", "Codec", "", grid);
        v_codec->setTextSize(50);
        AVCodec *codec = NULL;
        int a = 0;
        while ((codec = av_codec_next(codec))) {
          if (codec->encode && codec->type == CODEC_TYPE_VIDEO) {
            v_codec->addItem(codec->long_name);
            if (codec->id == _profile->vcodec.value()) {
              v_codec->setCurrentIndex(a);
            }
            a++;
          }
        }
        v_codec->activated().connect(SLOT(this, VideoPanel::setPredefinedCodecFlags));

         /*
         * Combobox for the Encoding Methode Selector
         */
        _methods.push_back("1 Pass Variable Bitrate");
        _methods.push_back("1 Pass Constant Bitrate");
        _methods.push_back("1 Pass Constant Quality");

        Wt::Ext::ComboBox * v_methode = _elcb.addElement("v_methode", "Methode", "1 Pass Variable Bitrate", grid);
        v_methode->setTextSize(50);
        v_methode->setEditable(false);
        std::list<std::string>::iterator metit = _methods.begin();
        for (int a = 0; metit != _methods.end(); metit++, a++) {
          v_methode->addItem((*metit) );
          if(_parameter["v_methode"]==(*metit))
            v_methode->setCurrentIndex(a);
        }
        _parameter["v_methode"]=v_methode->currentText().narrow();

        /*
         * Combobox for the Bitrate Selector
         */
        _bitrates.push_back(128);
        _bitrates.push_back(256);
        _bitrates.push_back(512);
        _bitrates.push_back(1024);
        _bitrates.push_back(2048);
        _bitrates.push_back(4096);

        Wt::Ext::ComboBox * v_bitrate = _elcb.addElement("b", "Video Bitrate", "please select a Bitrate", grid);
        v_bitrate->setEditable(true);
        std::list<int>::iterator bitit = _bitrates.begin();
        for (int a = 0; bitit != _bitrates.end(); bitit++, a++) {
          v_bitrate->addItem(org::esb::util::StringUtil::toString((*bitit)) + " kb/s");
          if ((*bitit) == atoi(_parameter["b"].c_str())) {
            v_bitrate->setCurrentIndex(a);
          }
        }


        /*
         * Combobox for the Framerate Selector
         */
        Wt::Ext::ComboBox * v_framerate = _elcb.addElement("v_framerate", "Video Framerate", _profile->vframerate.value(), grid);
        v_framerate->setTextSize(50);
        org::esb::util::StringTokenizer st(org::esb::config::Config::getProperty("framerates"), ",");

        int c = st.countTokens();
        v_framerate->addItem("same as source");
        for (int a = 1; a <= c; a++) {
          std::string t = st.nextToken();
          org::esb::util::StringTokenizer st2(t, "#");
          if (st2.countTokens() == 2) {
            st2.nextToken();
            std::string t2 = st2.nextToken();
            v_framerate->addItem(t2 + " Frames/Sec.");
            if (t2 == _profile->vframerate.value())
              v_framerate->setCurrentIndex(a);
          }
        }

        /*
         * Combobox for the Codec Preset Selector
         */

        Wt::Ext::ComboBox * vpre = _elcb.addElement("_vpre", "Predefined Flags", "", grid);
        vpre->setTextSize(50);

        /*
         * Input field for the Codec Preset
         * @TODO: this should be removed
         */
        _el.addElement("v_extra", "Extra Flags", _profile->vextra.value(), grid);
        setPredefinedCodecFlags();


        /*
         * Combobox for the Bitrate Selector
         */
        _keyframes.push_back(20);
        _keyframes.push_back(50);
        _keyframes.push_back(100);
        _keyframes.push_back(200);
        _keyframes.push_back(300);

        Wt::Ext::ComboBox * v_keyframes = _elcb.addElement("g", "Key Frame every", "100 Frames", grid);
        v_keyframes->setEditable(true);
        std::list<int>::iterator keyit = _keyframes.begin();
        for (int a = 0; keyit != _keyframes.end(); keyit++, a++) {
          v_keyframes->addItem(org::esb::util::StringUtil::toString((*keyit)) + " Frames");
          if(_parameter["g"]==org::esb::util::StringUtil::toString(*keyit))
            v_keyframes->setCurrentIndex(a);
        }
        _parameter["g"]=v_keyframes->currentText().narrow();
         v_keyframes->activated().connect(SLOT(this, VideoPanel::dataChanged));

//        _advance_table->load();
//        _advance_table->setEnabled(false);

        /*these are alwyays the last widget(except Advanced Button) to stretch to the buttom of the Page*/
        grid->addWidget(new Wt::WText(), grid->rowCount(), 0,0,3);
        grid->setRowStretch(grid->rowCount() - 1, 1);

        _advance_table=new VideoAdvanceTableView(_parameter);
        grid->addWidget(_advance_table,0,2,8,0,Wt::AlignRight);
        _advance_table->setCollapsible(true);
        _advance_table->setCollapsed(true);
        _advance_table->changed.connect(SLOT(this,VideoPanel::refresh));

        
        _advanced = new Wt::Ext::Button("Advanced>>>");
        _advanced->resize(100, Wt::WLength());
        grid->addWidget(_advanced, grid->rowCount(), 0, 0, 1,Wt::AlignRight);
        _advanced->clicked().connect(SLOT(this, VideoPanel::switchAdvanced));
      }

      void VideoPanel::setPredefinedCodecFlags() {
        Wt::Ext::ComboBox * v_codec = _elcb.getElement("v_codec");
        Wt::Ext::ComboBox * vpre = _elcb.getElement("_vpre");
        vpre->activated().connect(SLOT(this, VideoPanel::setSelectedPredifinedCodecFlags));
        std::string longname = v_codec->currentText().narrow();
        AVCodec *p = NULL;
        int a = 0;
        vpre->clear();
        while ((p = av_codec_next(p))) {
          if (p->encode && p->type == CODEC_TYPE_VIDEO && longname == p->long_name) {
            std::string codecpresetname = "";
            if (_profile->vpreset().get().count() > 0) {
              codecpresetname = _profile->vpreset().get().one().name;
            }
            vector<db::CodecPreset> presets = litesql::select<db::CodecPreset > (_profile->getDatabase(), db::CodecPreset::Codecid == p->id).all();
            int a = 0;
            for (vector<db::CodecPreset>::iterator it = presets.begin(); it != presets.end(); it++, a++) {
              vpre->addItem((std::string)(*it).name);
              if ((*it).name.value() == codecpresetname)
                vpre->setCurrentIndex(a);
            }
          }
        }
      }

      void VideoPanel::setSelectedPredifinedCodecFlags() {
        Wt::Ext::ComboBox * vpre = _elcb.getElement("_vpre");
        std::string name = vpre->currentText().narrow();
        db::CodecPreset preset = litesql::select<db::CodecPreset > (_profile->getDatabase(), db::CodecPreset::Name == name).one();
        Wt::Ext::LineEdit * v_extra = _el.getElement("v_extra");
        if (_profile->vpreset().get().count() > 0) {
          _profile->vpreset().unlink(_profile->vpreset().get().one());
        }
        _profile->vpreset().link(preset);
        v_extra->setText((std::string)preset.preset);
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
      }

      void VideoPanel::refresh() {

      }
      
      void VideoPanel::save() {
        org::esb::util::ScopedTimeCounter t("database profile update");
        _profile->getDatabase().begin();
        std::map<std::string, Wt::Ext::ComboBox*> boxes=_elcb.getElements();
        std::map<std::string, Wt::Ext::ComboBox*>::iterator bit=boxes.begin();
        for(;bit!=boxes.end();bit++){
          _parameter[(*bit).first]=(*bit).second->currentText().narrow();
        }

//        _parameter["v_keyframes"]=_elcb.getElement("v_keyframes")->currentText().narrow();
//        _parameter["v_methode"]=_elcb.getElement("v_methode")->currentText().narrow();
        //_advance_table->refresh();
        vector<db::ProfileParameter> params=_profile->params().get().all();
        vector<db::ProfileParameter>::iterator par=params.begin();
        for(;par!=params.end();par++){
          (*par).del();
        }
        std::map<std::string, std::string>::iterator it=_parameter.begin();
        for(;it!=_parameter.end();it++){
          db::ProfileParameter p(_profile->getDatabase());
          p.name=(*it).first;
          p.val=(*it).second;
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
