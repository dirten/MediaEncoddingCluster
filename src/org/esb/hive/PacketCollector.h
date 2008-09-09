#ifndef ORG_ESB_HIVE_PACKETCOLECTOR_H
#define ORG_ESB_HIVE_PACKETCOLECTOR_H

#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"

using namespace org::esb;
namespace org{
namespace esb{
namespace hive{

class PacketCollector: public lang::Thread{
	public:
		PacketCollector();
		~PacketCollector();
		void run();
	private:
		logger("hive.collector");

};

}}}

#endif

