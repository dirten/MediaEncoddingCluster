/* 
 * File:   InputFilePanel.cpp
 * Author: HoelscJ
 * 
 * Created on 13. Juli 2010, 17:08
 */

#include "InputFilePanel.h"
#include "Wt/WFitLayout"
#include "Wt/WText"

#include "Wt/Ext/ToolBar"
#include "Wt/Ext/Button"
#include "org/esb/web/FileChooser.h"
#include "org/esb/hive/FileImporter.h"
namespace org {
  namespace esb {
    namespace web {

      InputFilePanel::InputFilePanel() : Wt::Ext::Panel() {
        setTitle("Input Videos");
        setLayout(new Wt::WFitLayout());
        layout()->addWidget(new Wt::WText("InputFilePanel"));
        layout()->setContentsMargins(0, 0, 0, 0);



        setTopToolBar(new Wt::Ext::ToolBar());

        Wt::Ext::Button * addVideoButton = topToolBar()->addButton("Add Video");
        Wt::Ext::Button * removeVideoButton = topToolBar()->addButton("Remove Video");

        addVideoButton->clicked.connect(SLOT(this, InputFilePanel::addVideoButtonClicked));


      }

      void InputFilePanel::setInputFile(org::esb::io::File file) {
        LOGDEBUG("try import file"<<file.getPath());
        int result=import(file);
        LOGDEBUG("File imported:"<<result);
      }

      InputFilePanel::InputFilePanel(const InputFilePanel& orig) {
      }

      InputFilePanel::~InputFilePanel() {
        
      }

      void InputFilePanel::addVideoButtonClicked() {
        FileChooser * chooser=new FileChooser("Add Video", "/");
        chooser->selected.connect(SLOT(this, InputFilePanel::setInputFile));
        chooser->show();
      }
    }
  }
}

