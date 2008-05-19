#include <libcwd/sys.h>
#include <libcwd/debug.h>
#include <libcwd/type_info.h>
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
//#include "org/esb/sql/Connection.h"

#include <iostream>
#include <signal.h>
using namespace std;
using namespace org::esb::sql;
#define builtin_return_address(addr) ((char*)__builtin_return_address(addr) + libcwd::builtin_return_address_offset)

void terminate(int val){
	cout << "Terminate...\n"<<endl;
	

}
int main(){

//  Debug( libcw_do.on() );               // Turn on the default Debug Object.
//	 Debug( core_dump() );
// Turn on all debug channels that are off.
/*
  ForAllDebugChannels(
      if (!debugChannel.is_on())
          debugChannel.on();
      );
              
      // Print a listing of all debug channels to debug object `libcw_do'.
      Debug( list_channels_on(libcw_do) );
*/
//       Debug( list_channels_on(libcw_do) );
//		Debug( dc::notice.on() );             // Turn on the NOTICE Debug Channel.
  
//  Debug(dc::malloc.on());
//  Dout(dc::notice, "Hello World");
//libcwd::location_ct(builtin_return_address(0));
//	signal(SIGSEGV, terminate);

//	char * tmp;
//	delete tmp;

  Connection con("mysql:db=hive;user=root;passwd=root");
  Statement stmt=con.createStatement("select * from version");
// Debug( list_allocations_on(libcw_do) );
	char * t=0;
	delete t;
  ResultSet rs=stmt.executeQuery();
  
  while(rs.next()){
    cout << "Component:"<<rs.getString("component")<<"\tversion:"<<rs.getString("version")<<endl;
  }
  mysql_library_end();
//Debug( list_allocations_on(libcw_do) );
}

