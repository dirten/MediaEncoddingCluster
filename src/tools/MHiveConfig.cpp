/* 
 * File:   MHiveConfig.cpp
 * Author: HoelscJ
 *
 * Created on 4. April 2011, 12:00
 */

#include <cstdlib>
#include "config.h"
#include "org/esb/db/hivedb.hpp"
#include "boost/program_options.hpp"
#include <boost/program_options/parsers.hpp>

#include "org/esb/api/MHiveServerConnection.h"
#include "org/esb/api/MediaFile.h"
using namespace std;
namespace po = boost::program_options;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  po::options_description co("Configuration options");
  co.add_options()
          ("help", "produce this message")
          ("version", "Prints the Version")
          ;
  po::options_description user("User Configuration options");
  user.add_options()
          ("list", "List all Users in the Database")
          ("user", po::value<std::string > (), "Edit the User provided by [arg] in the Database")
          ("add", "Add the user to the Database")
          ("mod", "Modify the user in the Database")
          ("del", "Delete the User from the Database")
          ("lock", "Lock the User Account in the Database")
          ("unlock", "Unlock the User Account in the Database")
          ;

  po::options_description useradd("User Add Configuration options");
  useradd.add_options()
          ("useradd", po::value<std::string > (), "Edit the User provided by [arg] in the Database")
          ("comment", "Any text string. It is generally a short description of the login, and is currently used as the field for the user´s full name.")
          ("expiredate", "The date on which the user account will be disabled. The date is specified in the format YYYY-MM-DD.")
          ("gid", "The group name or number of the user´s initial login group. The group name must exist. A group number must refer to an already existing group.")
          ;

  po::options_description userdel("User Delete Configuration options");
  userdel.add_options()
          ("userdel", po::value<std::string > (), "Edit the User provided by [arg] in the Database")
          ("del", "The del command modifies the system account files, deleting all entries that refer to the user name LOGIN. The named user must exist.")
          ;

  co.add(user).add(useradd).add(userdel);


  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, co), vm);
  po::notify(vm);


  if (vm.count("help") || argc == 1) {
    cout << co << "\n";
    return 1;
  }
  if (vm.count("version")) {
    cout << "MediaEncodingCluster " << MHIVE_VERSION << endl;
  }

  org::esb::api::MHiveServerConnection server("localhost","jh","jh");
  server.connect();
  std::list<org::esb::rpc::Mediafile>files=server.getMediaFiles();
  std::list<org::esb::rpc::Mediafile>::iterator it=files.begin();

  for(;it!=files.end();it++){
    it->PrintDebugString();
  }

  LOGDEBUG("shutdown");
  Log::close();
  

  return 0;
}

