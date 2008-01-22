#ifndef ORG_ESB_HIVE_JOB_PROCESS_UNIT_H
#define ORG_ESB_HIVE_JOB_PROCESS_UNIT_H

#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include <vector>

using namespace std;
using namespace org::esb::av;

namespace org{
namespace esb{
namespace hive{
namespace job{
class ProcessUnit{
	public:
		ProcessUnit();	

//	private:
		vector<Packet*> _input_packets;
		vector<Packet*> _output_packets;
		Codec * _input_codec;
		Codec * _output_codec;
};
}}}}

#endif
