/* 
 * File:   TreeMainMenu.cpp
 * Author: HoelscJ
 * 
 * Created on 11. August 2010, 17:52
 */

#include "TreeMainMenu.h"
#include <Wt/WFitLayout>
#include <Wt/WIconPair>
#include <Wt/WImage>
#include <Wt/WText>
namespace org {
  namespace esb {
    namespace web {

      TreeMainMenu::TreeMainMenu(org::esb::web::WebApp2 * parent) : Wt::Ext::Panel(NULL),_parent(parent) {
        //        setLayout(new Wt::WFitLayout());
        Wt::WIconPair *mapIcon
                = new Wt::WIconPair("icons/yellow-folder-closed.png",
                "icons/yellow-folder-open.png", false);

        Wt::WTreeNode *rootNode = new Wt::WTreeNode("User MainMenu", mapIcon);
        rootNode->setImagePack("icons/");
        rootNode->expand();
        rootNode->setNodeVisible(false);
        createMenuNode("Projects", rootNode,&org::esb::web::WebApp2::listProjects, "icons/project-icon.png");
        createMenuNode("Media Data", rootNode, &org::esb::web::WebApp2::listAllFiles, "icons/media-icon.png");
        createMenuNode("Profiles", rootNode, &org::esb::web::WebApp2::listAllProfiles, "icons/profile-icon.png");
        createMenuNode("Encodings", rootNode, &org::esb::web::WebApp2::listAllEncodings, "icons/encoding-list-icon.png");
        createMenuNode("Watch Folder", rootNode, &org::esb::web::WebApp2::listAllWatchfolder, "icons/watch-folder-icon.png");
        createMenuNode("Settings", rootNode, &org::esb::web::WebApp2::editSystemConfiguration, "icons/admin-icon.png");
//        createMenuNode("Logout", rootNode, &TreeMainMenu::dummy, "icons/logout-icon.png");



        layout()->addWidget(rootNode);
      }

      Wt::WTreeNode * TreeMainMenu::createMenuNode(const Wt::WString& label, Wt::WTreeNode *parentNode, MenuFunc f, const char * icon) {
        Wt::WIconPair *labelIcon = new Wt::WIconPair(icon, icon, false);
//        labelIcon->icon1()->resize(30, 30);
//        labelIcon->icon2()->resize(30, 30);
        Wt::WTreeNode *node = new Wt::WTreeNode(label, labelIcon, parentNode);
        node->label()->setTextFormat(Wt::PlainText);
        node->label()->clicked().connect(_parent, f);
        node->labelIcon()->icon1Clicked().connect(_parent, f);
        node->labelIcon()->icon2Clicked().connect(_parent, f);
        node->labelIcon()->setStyleClass("licon");
        node->setSelectable(true);
        return node;
      }

      TreeMainMenu::TreeMainMenu(const TreeMainMenu& orig) {
      }
      void TreeMainMenu::dummy() {
      }

      TreeMainMenu::~TreeMainMenu() {
      }
    }
  }
}

