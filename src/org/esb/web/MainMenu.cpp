#include "MainMenu.h"
#include "org/esb/config/config.h"
#include "WebApp2.h"
#include <Wt/Ext/Menu>
#include <Wt/Ext/MenuItem>
#include <Wt/Ext/ToolBar>
#include <Wt/Ext/Button>

#include "project_v2/ProjectWizard.h"

//#include "MainMenuActionHandler.h"
MainMenu::MainMenu(org::esb::web::WebApp2 * parent) : Wt::WContainerWidget(NULL) {

  
  Wt::Ext::ToolBar * toolbar = new Wt::Ext::ToolBar(this);


  Wt::Ext::Menu * menu;
  Wt::Ext::MenuItem *item;
  Wt::Ext::Button * button;

//  menu = new Wt::Ext::Menu();
//  org::esb::web::v2::ProjectWizard *wiz=new org::esb::web::v2::ProjectWizard();
//  item = menu->addItem("ProjectWizard...",wiz,&org::esb::web::v2::ProjectWizard::open);
//  toolbar->addButton("V2 Gui", menu);

  menu = new Wt::Ext::Menu();
  
  item = menu->addItem("icons/encoding-project-list-icon.png","All Projects...",parent,&org::esb::web::WebApp2::listProjects);
  item = menu->addItem("icons/encoding-project-add-icon.png","Create New Encoding...",parent,&org::esb::web::WebApp2::createProject);
  toolbar->addButton("Projects", menu);
  
  
  menu = new Wt::Ext::Menu();
  item = menu->addItem("All Media Files...",parent,&org::esb::web::WebApp2::listAllFiles);
//  item = menu->addItem("Imported Media Files...",parent,&org::esb::web::WebApp2::listImportedFiles);
//  item = menu->addItem("Encoded Media Files...",parent,&org::esb::web::WebApp2::listEncodedFiles);

  button = toolbar->addButton("Media Files", menu);

  menu = new Wt::Ext::Menu();
  item = menu->addItem("All Encodings...",parent,&org::esb::web::WebApp2::listAllEncodings);
//  item = menu->addItem("Pending Encodings...",parent,&org::esb::web::WebApp2::listPendingEncodings);
//  item = menu->addItem("Active Encodings...",parent,&org::esb::web::WebApp2::listActiveEncodings);
//  item = menu->addItem("Successfull Encodings...",parent,&org::esb::web::WebApp2::listSuccessfullEncodings);
//  item = menu->addItem("Failure Encodings...",parent,&org::esb::web::WebApp2::listFailureEncodings);

  button = toolbar->addButton("Encodings", menu);

  menu = new Wt::Ext::Menu();
  item = menu->addItem("View Presets...",parent,&org::esb::web::WebApp2::listAllProfiles);
  item = menu->addItem("Create Preset...",parent,&org::esb::web::WebApp2::createProfiles);

  button = toolbar->addButton("Presets", menu);
/*
  menu = new Wt::Ext::Menu();
  item = menu->addItem("View Watchfolder...",parent,&org::esb::web::WebApp2::listAllWatchfolder);
  item = menu->addItem("Create Watchfolder...",parent,&org::esb::web::WebApp2::createWatchfolder);

  button = toolbar->addButton("Watchfolder", menu);
*/
  menu = new Wt::Ext::Menu();
  item = menu->addItem("Nodes...",parent,&org::esb::web::WebApp2::viewNodes);
  item = menu->addItem("Configuration...",parent,&org::esb::web::WebApp2::editSystemConfiguration);

  button = toolbar->addButton("System", menu);
}
