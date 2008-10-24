#ifndef ORG_ESB_WEB_WEBAPP_H
#define ORG_ESB_WEB_WEBAPP_H
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WWidget>
#include <Wt/WTreeNode>
#include <Wt/WStackedWidget>
#include <Wt/Ext/Panel>
namespace org{
namespace esb{
namespace web{
class Login;
class WebApp: public Wt::WApplication{
  public:
    WebApp(const Wt::WEnvironment & env);
  private:
      typedef void (WebApp::*ShowExample)();
       Wt::WWidget                       *currentExample_;
  Wt::WContainerWidget              *exampleContainer_;
    void openFileUpload();
    void openFileList();
    void openProfileNew();
    void openProfileList();
    void openConfig();
    void openFileList2();
    void openProfileList2();
    void openConfig2();
    void fileView();
    void profileView();
    void accountView();
    void encodingView();
    void logout();
    void setContent(Wt::WWidget *example);
    void authenticated();
    Wt::WWidget * introduction();
    Wt::WWidget * createMenuTree();
    Wt::WTreeNode * createMenuNode(const Wt::WString& label,
						    Wt::WTreeNode *parentNode,
						    ShowExample f,const char * icon="icons/document.png");
//    Wt::WWidget * test();
    Wt::WWidget * menu;
    Wt::WStackedWidget *contents;
	Login * login;
	int _isAuthenticated;
	int _user_id;
	Wt::Ext::Panel *west;
};
}}}
#endif
