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

#include <Wt/Ext/Menu>
#include <Wt/Ext/Container>
#include <Wt/WBorderLayout>
#include <Wt/WFitLayout>
#include <Wt/WString>
#include <Wt/Ext/MenuItem>
#include <Wt/Ext/ToolBar>
#include <Wt/Ext/TabWidget>


namespace org{
namespace esb{
namespace web{

/* Shortcut for a <div id=""> */
class Div : public Wt::WContainerWidget
{
public:
  Div(WContainerWidget *parent, const std::string& id)
    : WContainerWidget(parent)
  {
    setId(id);
  }
};


template <typename Function>
class DeferredWidget : public Wt::WContainerWidget
{
public:
  DeferredWidget(Function f)
    : f_(f) { }

private:
  void load() {
    addWidget(f_());
  }

  Function f_;
};

template <typename Function>
DeferredWidget<Function> *deferCreate(Function f)
{
  return new DeferredWidget<Function>(f);
}

/*
class Dashboard{
  public:
    Wt::WWidget * home(){
      return new Wt::WText("Dashboard Home");
    }
};
*/
WebApp::WebApp(const Wt::WEnvironment & env):WApplication(env){
  setTitle("Hive Webadmin");
  _isAuthenticated=false;
  Wt::Ext::Container *viewPort = new Wt::Ext::Container(root());
  Wt::WBorderLayout *layout = new Wt::WBorderLayout(viewPort);

  
  Wt::Ext::Panel *north = new Wt::Ext::Panel();
  north->setBorder(false);
  Wt::WText *head = new Wt::WText("Header");
  head->setStyleClass("north");
  north->setLayout(new Wt::WFitLayout());
  north->layout()->addWidget(head);
  north->resize(Wt::WLength(), 35);
  layout->addWidget(north, Wt::WBorderLayout::North);

    /* West */
  west = new Wt::Ext::Panel();
//  west->layout()->addWidget(createMenuTree());

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
/*
  Wt::WContainerWidget *container = new Wt::WContainerWidget(exampleContainer_);
  container->addWidget(new Wt::WText(Wt::WString::tr("about")));
  currentExample_ = container;
*/

  currentExample_ = login = new Login(exampleContainer_);
  login->authenticated.connect(SLOT(this,WebApp::authenticated));
/*  
  Wt::WContainerWidget * main=new Wt::WContainerWidget(root());
  main->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength(100, Wt::WLength::Percentage));

  Wt::Ext::TabWidget * tab=new Wt::Ext::TabWidget(main);
  //tab->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength(100, Wt::WLength::Percentage));

//  tab->resize(1000,1000);
	sql::Connection con(Config::getProperty("db.connection"));
	sql::Statement stmt=con.createStatement("select id, filename, container_type, size from files ");
	SqlTableModel * model=new SqlTableModel(stmt.executeQuery());
	Wt::Ext::TableView * table;
//	Files * files;
	tab->addTab(table=new Wt::Ext::TableView(),"Files");
//	tab->addTab(files=new Files(),"Files");
//	files->resize(1000,500);
	table->setModel(model);
  	table->resize(Wt::WLength(100, Wt::WLength::Percentage),300);
  table->setAlternatingRowColors(true);
  table->resizeColumnsToContents(true);
  table->setHighlightMouseOver(true);
  table->setSelectionBehavior(Wt::SelectRows);
  table->setSelectionMode(Wt::SingleSelection);


//  tab->addTab(new Profiles(),"Profiles");
//  tab->addTab(new Configuration(),"Configuration");
*/
  useStyleSheet("ext/resources/css/xtheme-gray.css");
  useStyleSheet("filetree.css");
  
}
Wt::WWidget *WebApp::createMenuTree()
{
  Wt::WIconPair *mapIcon
    = new Wt::WIconPair("icons/yellow-folder-closed.png",
		    "icons/yellow-folder-open.png", false);

  Wt::WTreeNode *rootNode = new Wt::WTreeNode("User MainMenu", mapIcon);
  rootNode->setImagePack("icons/");
  rootNode->expand();
//  rootNode->setNodeVisible(false);
  rootNode->setLoadPolicy(Wt::WTreeNode::NextLevelLoading);
  createMenuNode("My Account", rootNode,
		    &WebApp::accountView);
  createMenuNode("My Media", rootNode,
		    &WebApp::fileView);
  createMenuNode("My Profiles", rootNode,
		    &WebApp::profileView);
  createMenuNode("My Encodings", rootNode,
		    &WebApp::profileView);
  createMenuNode("My Watch Folder", rootNode,
		    &WebApp::profileView);
  createMenuNode("Logout", rootNode,
		    &WebApp::logout);
/*
  createExampleNode("Menu & ToolBar", rootNode,
		    &ExtKitchenApplication::menuAndToolBarExample);
  createExampleNode("Form widgets", rootNode,
		    &ExtKitchenApplication::formWidgetsExample);
  createExampleNode("TableView", rootNode,
		    &ExtKitchenApplication::tableViewExample);
  createExampleNode("Dialogs", rootNode,
		    &ExtKitchenApplication::dialogExample);
  createExampleNode("TabWidget", rootNode,
		    &ExtKitchenApplication::tabWidgetExample);
*/
  rootNode->setMargin(5);
 
  return rootNode;
}
Wt::WTreeNode *WebApp::createMenuNode(const Wt::WString& label,
						    Wt::WTreeNode *parentNode,
						    ShowExample f)
{
  Wt::WIconPair *labelIcon
    = new Wt::WIconPair("icons/document.png", "icons/document.png", false);

  Wt::WTreeNode *node = new Wt::WTreeNode(label, labelIcon, parentNode);
  node->label()->setFormatting(Wt::WText::PlainFormatting);
  node->label()->clicked.connect(this, f);

  return node;
}
void WebApp::fileView(){
  Wt::WContainerWidget *ex = new Wt::WContainerWidget();
//  ex->resize(600,300);
/*
	sql::Connection con(Config::getProperty("db.connection"));
	sql::Statement stmt=con.createStatement("select id, filename, container_type, size from files ");
	SqlTableModel * model=new SqlTableModel(stmt.executeQuery());
	Wt::Ext::TableView * table;
	table=new Wt::Ext::TableView(ex);
	table->setModel(model);
        table->resize(800,300);
        table->setAlternatingRowColors(true);
        table->resizeColumnsToContents(true);
        table->setHighlightMouseOver(true);
        table->setSelectionBehavior(Wt::SelectRows);
        table->setSelectionMode(Wt::SingleSelection);
*/
        setContent(new Files());
}
void WebApp::profileView(){
/*
  Wt::WContainerWidget *ex = new Wt::WContainerWidget();
  ex->resize(500,300);
	sql::Connection con(Config::getProperty("db.connection"));
	sql::Statement stmt=con.createStatement("select * from profiles");
	SqlTableModel * model=new SqlTableModel(stmt.executeQuery());
	Wt::Ext::TableView * table;
	table=new Wt::Ext::TableView(ex);
	table->setModel(model);
        table->resize(700,300);
        table->setAlternatingRowColors(true);
        table->resizeColumnsToContents(true);
        table->setHighlightMouseOver(true);
        table->setSelectionBehavior(Wt::SelectRows);
        table->setSelectionMode(Wt::SingleSelection);
*/
        setContent(new Profiles());
}

void WebApp::accountView(){
        setContent(new Account(_user_id));
}

void WebApp::logout(){
	_isAuthenticated=false;
	_user_id=0;
//	Wt::WWidget * w=west->layout()->widget();
	west->layout()->removeWidget(menu);
	delete menu;
	west->collapse();
  Wt::WContainerWidget *container = new Wt::WContainerWidget(exampleContainer_);
  container->addWidget(new Wt::WText(Wt::WString::tr("logged_out")));
	setContent(container);
}

void WebApp::authenticated(){
	_isAuthenticated=true;
	_user_id=login->getUserId();
    west->layout()->addWidget(menu=createMenuTree());
	west->expand();
}

void WebApp::setContent(Wt::WWidget *example)
{

	if(!_isAuthenticated){
		delete example;
		example=login=new Login();
  		login->authenticated.connect(SLOT(this,WebApp::authenticated));
	}

  delete currentExample_;
  currentExample_ = example;
  exampleContainer_->addWidget(currentExample_);
}
}}}

