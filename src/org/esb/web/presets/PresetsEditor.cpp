/* 
 * File:   PresetsEditor.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 11:52
 */

#include "PresetsEditor.h"
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
namespace org {
  namespace esb {
    namespace web {

      PresetsEditor::PresetsEditor(Ptr<db::Profile> profile) : Wt::Ext::Panel(), _profile(profile) {
        setLayout(new Wt::WBorderLayout());
        /*loading parameters from the database for the Video Codec only*/
        if (_profile->params().get().count() > 0) {
          vector<db::ProfileParameter> params = _profile->params().get().all();
          vector<db::ProfileParameter>::iterator it = params.begin();
          for (; it != params.end(); it++) {
            _parameter[(*it).name.value()] = (*it).val.value();
            LOGDEBUG("name=" << (*it).name.value() << " value=" << (*it).val.value())
          }
        }

        ((Wt::WBorderLayout*)layout())->addWidget(createTop(), Wt::WBorderLayout::North);
        ((Wt::WBorderLayout*)layout())->addWidget(createContent(), Wt::WBorderLayout::Center);
        //        ((Wt::WBorderLayout*)layout())->addWidget(new Wt::WText("Bottom"),Wt::WBorderLayout::South);

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
        name->setText(_profile->name.value());
        name->resize(400, Wt::WLength());
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
        tab->addTab(new org::esb::web::FormatPanel(_parameter), "Format");
        tab->addTab(new org::esb::web::VideoPanel(_parameter), "Video");
        tab->addTab(new org::esb::web::AudioPanel(_parameter), "Audio");
        tab->addTab(new org::esb::web::PresetFilterPanel(_profile), "Filter");

        Wt::Ext::ToolBar * tb = NULL;
        tab->setTopToolBar(tb = new Wt::Ext::ToolBar());

        Wt::Ext::Button * save = tb->addButton("Save Preset");
        save->clicked().connect(SLOT(this, PresetsEditor::save));
        save->setIcon("icons/accept-icon.png");
        tb->addSeparator();
        Wt::Ext::Button * cancel = tb->addButton("Open Preview");

        return tab;
      }

      void PresetsEditor::save() {
        Wt::Ext::TabWidget *tab = static_cast<Wt::Ext::TabWidget *> (((Wt::WBorderLayout*)layout())->widgetAt(Wt::WBorderLayout::Center));
        ((org::esb::web::FormatPanel*)tab->panel(0))->save();
        //((org::esb::web::VideoPanel*)tab->panel(1))->save();
        //((org::esb::web::AudioPanel*)tab->panel(2))->save();
        _profile->getDatabase().begin();

        vector<db::ProfileParameter> params = _profile->params().get().all();
        vector<db::ProfileParameter>::iterator par = params.begin();
        for (; par != params.end(); par++) {
          _profile->params().unlink((*par));
          (*par).del();
        }
        std::map<std::string, std::string>::iterator it = _parameter.begin();
        for (; it != _parameter.end(); it++) {
          db::ProfileParameter p(_profile->getDatabase());
          p.name = (*it).first;
          p.val = (*it).second;
          p.update();
          LOGDEBUG(p);
          _profile->params().link(p);
        }

        _profile->name = name->text().narrow();
        _profile->update();
        _profile->getDatabase().commit();

      }

    }
  }
}
