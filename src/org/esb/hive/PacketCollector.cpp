
#include "PacketCollector.h"
#include "org/esb/io/File.h"

using namespace org::esb;
namespace org{
namespace esb{
namespace hive{


PacketCollector::PacketCollector(){


}

PacketCollector::~PacketCollector(){


}

void PacketCollector::run(){
	while(true){
		lang::Thread::sleep(5000);
		logdebug("collector turn arround");
		io::File dir("/tmp/hive");
		list<boost::shared_ptr<io::File> > files=dir.listFiles();
		list<boost::shared_ptr<io::File> >::iterator it;
		for(it=files.begin();it!=files.end();it++){
			boost::shared_ptr<io::File> file=*it;
			
//			logdebug(file->getPath());
		}
	}
}

}}}


