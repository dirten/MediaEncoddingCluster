#ifndef ORG_ESB_WEB_SQLTABLEMODEL_H
#define ORG_ESB_WEB_SQLTABLEMODEL_H
#include <Wt/WStandardItemModel>
#include <Wt/WContainerWidget>
#include "org/esb/sql/ResultSet.h"
namespace org{
namespace esb{
namespace web{
class SqlTableModel:public Wt::WStandardItemModel{
  public:
    SqlTableModel(org::esb::sql::ResultSet result,Wt::WContainerWidget* parent);
    void clear();
    void load(sql::ResultSet result);

};
}}}
#endif


