
namespace org{
namespace esb{
namespace web{
class Dashboard{
  public:
    Wt::WWidget * home(){
      return new Wt::WText("Dashboard Home");
    }
};
}}}

