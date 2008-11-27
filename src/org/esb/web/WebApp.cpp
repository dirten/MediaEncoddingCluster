#include "WebApp.h"
#include <Wt/WContainerWidget>
#include <Wt/WStackedWidget>
#include <Wt/WWidget>
#include <Wt/WText>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Dashboard.cpp>
#include <Upload.cpp>
#include <Files.cpp>
#include <Profiles.cpp>
//#include <ProfilesEdit.cpp>
#include <Configuration.cpp>
#include <Account.cpp>
#include <Login.cpp>
#include <Encodings.cpp>
#include "WatchFolder.cpp"

#include <Wt/Ext/Menu>
#include <Wt/Ext/Container>
#include <Wt/WBorderLayout>
#include <Wt/WFitLayout>
#include <Wt/WString>
#include <Wt/Ext/MenuItem>
#include <Wt/Ext/ToolBar>
#include <Wt/Ext/TabWidget>


namespace org {
  namespace esb {
    namespace web {

      WebApp::WebApp(const Wt::WEnvironment & env) : WApplication(env) {
        if(string(org::esb::config::Config::getProperty("hive.mode"))=="setup"){
          WApplication::instance()->redirect("/setup");
          WApplication::instance()->quit();
        }
        setTitle("Hive Webadmin");
        _isAuthenticated = false;
        Wt::Ext::Container *viewPort = new Wt::Ext::Container(root());
        Wt::WBorderLayout *layout = new Wt::WBorderLayout(viewPort);


        Wt::Ext::Panel *north = new Wt::Ext::Panel();
        north->setBorder(false);
        Wt::WText *head = new Wt::WText("MediaEncodingCluster V-0.0.1");
        head->setStyleClass("north");
        north->setLayout(new Wt::WFitLayout());
        north->layout()->addWidget(head);
        north->resize(Wt::WLength(), 35);
        layout->addWidget(north, Wt::WBorderLayout::North);

        /* West */
        west = new Wt::Ext::Panel();

        west->setTitle("Menu");
        west->resize(200, Wt::WLength());
        west->setResizable(true);
        west->collapse();
        west->setCollapsible(true);
        west->setAnimate(true);
        west->setAutoScrollBars(true);
        layout->addWidget(west, Wt::WBorderLayout::West);


        /* Center */
        Wt::Ext::Panel *center = new Wt::Ext::Panel();
        center->setTitle("MediaEncodingCluster");
        center->layout()->addWidget(exampleContainer_ = new Wt::WContainerWidget());
        center->setAutoScrollBars(true);
        layout->addWidget(center, Wt::WBorderLayout::Center);

        exampleContainer_->setPadding(5);

        currentExample_ = login = new Login(exampleContainer_);
        login->authenticated.connect(SLOT(this, WebApp::authenticated));
//        useStyleSheet("ext/resources/css/xtheme-gray.css");
        useStyleSheet("filetree.css");
        messageResourceBundle().use("../res/messages",false);

      }

      Wt::WWidget *WebApp::createMenuTree() {
        Wt::WIconPair *mapIcon
            = new Wt::WIconPair("icons/yellow-folder-closed.png",
            "icons/yellow-folder-open.png", false);

        Wt::WTreeNode *rootNode = new Wt::WTreeNode("User MainMenu", mapIcon);
        rootNode->setImagePack("icons/");
        rootNode->expand();
        rootNode->setNodeVisible(false);

        rootNode->setLoadPolicy(Wt::WTreeNode::NextLevelLoading);
        createMenuNode("My Account", rootNode, &WebApp::accountView, "icons/edit_user.png");
        createMenuNode("My Media", rootNode, &WebApp::fileView, "icons/media.png");
        createMenuNode("My Profiles", rootNode, &WebApp::profileView, "icons/profile.png");
        createMenuNode("My Encodings", rootNode, &WebApp::encodingView, "icons/encode.png");
        createMenuNode("My Watch Folder", rootNode, &WebApp::watchFolderView, "icons/folder.png");
        createMenuNode("Admin Config", rootNode, &WebApp::configurationView, "icons/admin.png");
        createMenuNode("Logout", rootNode, &WebApp::logout, "icons/logout.png");
        rootNode->setMargin(5);

        return rootNode;
      }

      Wt::WTreeNode *WebApp::createMenuNode(const Wt::WString& label, Wt::WTreeNode *parentNode, ShowExample f, const char * icon) {
        Wt::WIconPair *labelIcon = new Wt::WIconPair(icon, icon, false);
        Wt::WTreeNode *node = new Wt::WTreeNode(label, labelIcon, parentNode);
        node->label()->setFormatting(Wt::WText::PlainFormatting);
        node->label()->clicked.connect(this, f);
        node->labelIcon()->icon1Clicked.connect(this, f);
        node->labelIcon()->icon2Clicked.connect(this, f);
        node->labelIcon()->setStyleClass("licon");
        node->setSelectable(true);
        return node;
      }

      void WebApp::fileView() {
        setContent(new Files());
      }

      void WebApp::profileView() {
        setContent(new Profiles());
      }

      void WebApp::accountView() {
        setContent(new Account(_user_id));
      }

      void WebApp::encodingView() {
        setContent(new Encoding(_user_id));
      }

      void WebApp::watchFolderView() {
        setContent(new WatchFolder(_user_id));
      }

      void WebApp::configurationView() {
        setContent(new Configuration());
      }

      void WebApp::logout() {
        _isAuthenticated = false;
        _user_id = 0;
        west->layout()->removeWidget(menu);
        delete menu;
        west->collapse();
        Wt::WContainerWidget *container = new Wt::WContainerWidget(exampleContainer_);
        container->addWidget(new Wt::WText(Wt::WString::tr("logged_out")));
        setContent(container);
      }

      void WebApp::authenticated() {
        _isAuthenticated = true;
        _user_id = login->getUserId();
        west->layout()->addWidget(menu = createMenuTree());
        west->expand();
        Wt::WContainerWidget *container = new Wt::WContainerWidget(exampleContainer_);
        container->addWidget(new Wt::WText(Wt::WString::tr("about")));
        login->setHidden(true);
        currentExample_ = container;
      }

      void WebApp::setContent(Wt::WWidget *example) {

        if (!_isAuthenticated) {
          delete example;
          example = login = new Login();
          login->authenticated.connect(SLOT(this, WebApp::authenticated));
        }

        delete currentExample_;
        currentExample_ = example;
        exampleContainer_->addWidget(currentExample_);
      }
    }
  }
}

