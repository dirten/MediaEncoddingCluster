/* 
 * File:   PresetsEditorWindow.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 11:56
 */

#include "PresetsEditorWindow.h"
#include "Wt/WFitLayout"
#include "Wt/Ext/Button"
#include "PresetsEditor.h"
#include "org/esb/hive/DatabaseService.h"
namespace org {
  namespace esb {
    namespace web {

      PresetsEditorWindow::PresetsEditorWindow(Ptr<db::Profile> p) :_profile(p), Wt::Ext::Dialog("Preset Editor") {
        resize(830, 500);
        setBorder(false);
        setLayout(new Wt::WFitLayout());
        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked().connect(SLOT(this, PresetsEditorWindow::cancel));
        buttons().back()->setIcon("icons/remove-icon.png");

        addButton(new Wt::Ext::Button("Save"));
        buttons().back()->clicked().connect(SLOT(this, PresetsEditorWindow::save));
        buttons().back()->setIcon("icons/accept-icon.png");
        //_profile=new db::Profile(_db);
        
        editor=new PresetsEditor(_profile);

        layout()->addWidget(editor);


      }

      PresetsEditorWindow::~PresetsEditorWindow() {
      }

      void PresetsEditorWindow::cancel() {
        this->reject();

      }

      void PresetsEditorWindow::save() {
        editor->save();
        _profile->update();
        this->accept();
      }

    }
  }
}
