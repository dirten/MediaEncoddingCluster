/* 
 * File:   PresetsEditor.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 11:52
 */

#include "PresetsEditor.h"
#include "Wt/Ext/MessageBox"
#include "Wt/WContainerWidget"
#include "Wt/WCompositeWidget"
#include "Wt/WBoxLayout"
#include "Wt/WLabel"
#include "Wt/WText"

#include "Wt/Ext/TabWidget"
#include "Wt/WFitLayout"
#include "Wt/WBorderLayout"
#include "org/esb/hive/DatabaseService.h"
#include "FormatPanel.h"
#include "VideoPanel.h"
#include "AudioPanel.h"
#include "FilterPanel.h"
#include "Wt/Ext/ToolBar"
#include "Wt/Ext/Button"
#include "Wt/WDateTime"
#include "org/esb/hive/PresetFileWriter.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "org/esb/util/StringUtil.h"
/*on windows there is a macro defined with name MessageBox*/
#ifdef MessageBox
#undef MessageBox
#endif

namespace org {
  namespace esb {
    namespace web {

      PresetsEditor::PresetsEditor(std::string filename) : _filename(filename), Wt::Ext::Panel() {
        org::esb::hive::PresetReader reader(filename);
        org::esb::hive::PresetReader::Preset preset = reader.getPreset();
        _presetparameter.insert(preset.begin(), preset.end());

        org::esb::hive::PresetReader::CodecList codecparam = reader.getCodecList();
        _videoparameter.insert(codecparam["video"].begin(), codecparam["video"].end());
        _audioparameter.insert(codecparam["audio"].begin(), codecparam["audio"].end());

        _filterparameter = reader.getFilterList();
        buildGui();

      }

      PresetsEditor::PresetsEditor(Ptr<db::Profile> profile) : Wt::Ext::Panel(), _profile(profile) {
        /*loading parameters from the database*/
        if (_profile->params().get().count() > 0) {
          vector<db::ProfileParameter> params = _profile->params().get().all();
          vector<db::ProfileParameter>::iterator it = params.begin();
          for (; it != params.end(); it++) {
            //            _parameter[(*it).name.value()] = (*it).val.value();
            LOGDEBUG("name=" << (*it).name.value() << " value=" << (*it).val.value())
          }
        }
        //        ((Wt::WBorderLayout*)layout())->addWidget(new Wt::WText("Bottom"),Wt::WBorderLayout::South);
        buildGui();
      }

      void PresetsEditor::buildGui() {
        setLayout(new Wt::WBorderLayout());

        ((Wt::WBorderLayout*)layout())->addWidget(createTop(), Wt::WBorderLayout::North);
        ((Wt::WBorderLayout*)layout())->addWidget(createContent(), Wt::WBorderLayout::Center);

      }

      PresetsEditor::~PresetsEditor() {
      }

      Wt::WWidget * PresetsEditor::createTop() {
        Wt::Ext::Panel * root = new Wt::Ext::Panel();
        root->resize(Wt::WLength(), 40);
        root->setLayout(new Wt::WFitLayout());
        Wt::WContainerWidget *top = new Wt::WContainerWidget();
        root->layout()->addWidget(top);
        Wt::WGridLayout * grid = new Wt::WGridLayout();
        top->setLayout(grid);
        top->resize(Wt::WLength(), 40);
        Wt::WLabel * label = new Wt::WLabel("Preset Name:");

        name = new Wt::Ext::LineEdit();
        name->setText(_presetparameter["name"]);
        name->resize(400, Wt::WLength());
        name->setMaxLength(100);
        label->setBuddy(name);
        grid->addWidget(label, 0, 0);
        grid->addWidget(name, 0, 1);
        grid->addWidget(new Wt::WText(""), 0, 2);
        grid->setColumnStretch(1, 1);
        return root;
      }

      Wt::WWidget * PresetsEditor::createContent() {
        Wt::Ext::TabWidget *tab = new Wt::Ext::TabWidget();
        tab->resize(Wt::WLength(), Wt::WLength());
        tab->addTab(new org::esb::web::FormatPanel(_presetparameter), "Format");
        tab->addTab(new org::esb::web::VideoPanel(_videoparameter), "Video");
        tab->addTab(new org::esb::web::AudioPanel(_audioparameter), "Audio");
        tab->addTab(new org::esb::web::PresetFilterPanel(_filterparameter), "Filter");

        /*
        Wt::Ext::ToolBar * tb = NULL;
        tab->setTopToolBar(tb = new Wt::Ext::ToolBar());

        Wt::Ext::Button * save = tb->addButton("Save Preset");
        save->clicked().connect(SLOT(this, PresetsEditor::save));
        save->setIcon("icons/accept-icon.png");
        tb->addSeparator();
        Wt::Ext::Button * cancel = tb->addButton("Open Preview");
         */
        return tab;
      }

      bool PresetsEditor::save() {
        if (name->text().narrow().size()==0) {
          Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Missing Preset Name", "please define a Preset Name in the top of the Window", Wt::Warning, Wt::Ok);
          box->show();
          return false;
        }
        _presetparameter["name"] = name->text().narrow();
        //        std::string path=org::esb::config::Config::get("preset.path");

        /*this is a new Preset, ask for a filename*/
        if (_filename.length() == 0) {
          /*
          Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Define a File Name", "please define a File Name for the new Preset", Wt::Question, Wt::Ok);
          box->enablePrompt(true);
          box->show();
          box->exec();
          std::string filename=box->value().narrow();
          delete box;
          if(filename.length()==0){
            Wt::Ext::MessageBox *boxf = new Wt::Ext::MessageBox("Setting Filename", "Filename could not be empty", Wt::Warning, Wt::Ok);
            boxf->show();
            boxf->exec();
            delete boxf;
            return false;
          }*/
          std::string filename=_presetparameter["name"];
          filename=org::esb::util::StringUtil::replace(filename," ","_");
          filename=org::esb::util::StringUtil::replace(filename,"*","_");
          filename=org::esb::util::StringUtil::replace(filename,"@","_");
          //filename=org::esb::util::StringUtil::replace(filename,".","_");

          /*cutting out filename to 150 chars*/

          filename=filename.substr(0,filename.length()>100?100:filename.length());
          _filename = org::esb::config::Config::get("preset.path");
          _filename += "/";
          _filename += filename;
          _filename+=".preset";
          if(org::esb::io::File(_filename).exists()){
            /*make an unique file name to avoid overwriting exising files*/
            Wt::WDateTime cur=Wt::WDateTime::currentDateTime();
            std::string ts=cur.toString("yyyyMMddhhmmss").narrow();
            
            filename+="-"+ts;
            _filename = org::esb::config::Config::get("preset.path");
            _filename += "/";
            _filename += filename;
            _filename+=".preset";
          }
        }
        org::esb::hive::PresetFileWriter writer(_filename);
        writer.setPreset(_presetparameter);

        org::esb::hive::PresetFileWriter::CodecList codecs;

        std::multimap<std::string, std::string> tmp_video_map;
        tmp_video_map.insert(_videoparameter.begin(), _videoparameter.end());
        codecs.insert(std::pair<std::string, std::multimap<std::string, std::string> >("video", tmp_video_map));

        std::multimap<std::string, std::string> tmp_audio_map;
        tmp_audio_map.insert(_audioparameter.begin(), _audioparameter.end());
        codecs.insert(std::pair<std::string, std::multimap<std::string, std::string> >("audio", tmp_audio_map));
        writer.setCodecList(codecs);
        writer.setFilterList(_filterparameter);
        writer.write();
        return true;
      }
    }
  }
}
