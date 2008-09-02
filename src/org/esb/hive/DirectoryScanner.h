#ifndef ORG_ESB_HIVE_DIRECTORYSCANNER_H
#define ORG_ESB_HIVE_DIRECTORYSCANNER_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/io/File.h"
#include <boost/thread.hpp>

#include <string>

namespace org{
namespace esb{
namespace hive{
//class org::esb::signal::Message;
class DirectoryScanner: public org::esb::signal::MessageListener{
  public:
    DirectoryScanner(std::string dir, int interval);
    ~DirectoryScanner();
    void onMessage(org::esb::signal::Message & msg);
//  private:
    void scan();
    void scan(std::string dir);
    void computeFile(org::esb::io::File file);
//    org::esb::sql::Connection _con;
//    org::esb::sql::PreparedStatement _stmt;
    bool _halt;
    boost::thread * th;
    std::string _dir;
    int _interval;
    int _level;
};
}}}
#endif

