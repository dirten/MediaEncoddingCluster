#include "MainMenu.h"
#include "WebApp2.h"
#include <Wt/Ext/Menu>
#include <Wt/Ext/MenuItem>
#include <Wt/Ext/ToolBar>
#include <Wt/Ext/Button>

//#include "MainMenuActionHandler.h"
MainMenu::MainMenu(org::esb::web::WebApp2 * parent) : Wt::WContainerWidget(NULL) {

  
  Wt::Ext::ToolBar * toolbar = new Wt::Ext::ToolBar(this);


  Wt::Ext::Menu * menu = new Wt::Ext::Menu();
  Wt::Ext::MenuItem *item;
  item = menu->addItem("All Media Files...",parent,&org::esb::web::WebApp2::listAllFiles);
  item = menu->addItem("Imported Media Files...",parent,&org::esb::web::WebApp2::listImportedFiles);
  item = menu->addItem("Encoded Media Files...",parent,&org::esb::web::WebApp2::listEncodedFiles);

  Wt::Ext::Button * button = toolbar->addButton("Media Files", menu);

  menu = new Wt::Ext::Menu();
  item = menu->addItem("All Encodings...",parent,&org::esb::web::WebApp2::listAllEncodings);
  item = menu->addItem("Pending Encodings...",parent,&org::esb::web::WebApp2::listPendingEncodings);
  item = menu->addItem("Active Encodings...",parent,&org::esb::web::WebApp2::listActiveEncodings);
  item = menu->addItem("Successfull Encodings...",parent,&org::esb::web::WebApp2::listSuccessfullEncodings);
//  item = menu->addItem("Failure Encodings...",parent,&org::esb::web::WebApp2::listFailureEncodings);

  button = toolbar->addButton("Encodings", menu);

  menu = new Wt::Ext::Menu();
  item = menu->addItem("View Profiles...",parent,&org::esb::web::WebApp2::listAllProfiles);
  item = menu->addItem("Create Profile...",parent,&org::esb::web::WebApp2::createProfiles);

  button = toolbar->addButton("Profiles", menu);

  menu = new Wt::Ext::Menu();
  item = menu->addItem("View Watchfolder...",parent,&org::esb::web::WebApp2::listAllWatchfolder);
  item = menu->addItem("Create Watchfolder...",parent,&org::esb::web::WebApp2::createWatchfolder);

  button = toolbar->addButton("Watchfolder", menu);

  menu = new Wt::Ext::Menu();
  item = menu->addItem("Configuration...");

  button = toolbar->addButton("System", menu);
}
