#ifndef ORG_ESB_HIVE_JOB_PROCESS_UNIT_H
#define ORG_ESB_HIVE_JOB_PROCESS_UNIT_H

#include "org/esb/av/Packet.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include <vector>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
using namespace std;
using namespace org::esb::av;
//using namespace boost;
namespace org{
namespace esb{
namespace hive{
namespace job{
class ProcessUnit{
	public:
		ProcessUnit();
		void process();

//	private:
//	        friend class boost::serialization::access;

		list<boost::shared_ptr<Packet> > _input_packets;
		list<boost::shared_ptr<Packet> > _output_packets;
		Decoder * _decoder;
		Encoder * _encoder;
		int _source_stream;
		int _target_stream;
		template<class Archive>
	    	    void serialize(Archive & ar, const unsigned int version){
	    		ar & _input_packets;
	    		ar & _output_packets;
	    		ar & _source_stream;
	    		ar & _target_stream;
//	    		if(_decoder!=NULL)
	    		    ar & _decoder;
//	    		if(_encoder!=NULL)
	    		    ar & _encoder;
	    	    }
	private:
};
}}}}

#endif
