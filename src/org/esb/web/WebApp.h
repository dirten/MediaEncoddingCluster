#ifndef ORG_ESB_WEB_WEBAPP_H
#define ORG_ESB_WEB_WEBAPP_H
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WWidget>
#include <Wt/WStackedWidget>
namespace org{
namespace esb{
namespace web{
class WebApp: public Wt::WApplication{
  public:
    WebApp(const Wt::WEnvironment & env);
  private:
    void openFileUpload();
    void openFileList();
    void openProfileNew();
    void openProfileList();
    void openConfig();
    void openFileList2();
    void openProfileList2();
    void openConfig2();
    Wt::WWidget * introduction();
    Wt::WWidget * test();
    Wt::WWidget * submenu1();
    Wt::WStackedWidget *contents;
};
}}}
#endif
