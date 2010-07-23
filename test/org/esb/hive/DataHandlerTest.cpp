
#include "org/esb/hive/protocol/DataHandler.cpp"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/StringOutputStream.h"
#include "org/esb/io/StringInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"

#include "org/esb/util/Log.h"
using namespace org::esb::io;

int main(){
	Log::open("");
	
		std::string indata;
		std::string outdata;

	StringOutputStream fos(outdata);
	ObjectOutputStream oos(&fos);
	StringInputStream fis(indata);
	ObjectInputStream ois(&fis);
	boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string("127.0.0.1"),6000);
	
	DataHandler handler(&ois, &oos, e);
	
	Log::close();
	return 0;
}