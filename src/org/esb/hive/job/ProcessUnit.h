#ifndef ORG_ESB_HIVE_JOB_PROCESS_UNIT_H
#define ORG_ESB_HIVE_JOB_PROCESS_UNIT_H

#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include <vector>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>

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
//	        friend class boost::serialization::access;

		list<Packet*> _input_packets;
		list<Packet*> _output_packets;
		Codec * _input_codec;
		Codec * _output_codec;
		template<class Archive>
	    	    void serialize(Archive & ar, const unsigned int version){
	    		ar & _input_packets;
	    		ar & _output_packets;
	    		ar & _input_codec;
	    		ar & _output_codec;
	    	    }
	private:
};
}}}}

#endif
