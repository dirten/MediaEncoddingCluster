#ifndef ORG_ESB_WEB_WEBAPP_H
#define ORG_ESB_WEB_WEBAPP_H
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WWidget>
namespace org{
namespace esb{
namespace web{
class WebApp: public Wt::WApplication{
  public:
    WebApp(const Wt::WEnvironment & env);
  private:
    Wt::WWidget * introduction();
    Wt::WWidget * test();
    Wt::WWidget * submenu1();
};
}}}
#endif
