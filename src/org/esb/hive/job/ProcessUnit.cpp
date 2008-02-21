#include "ProcessUnit.h"
#include "org/esb/av/Frame.h"
using namespace org::esb::hive::job;
using namespace org::esb::av;


ProcessUnit::ProcessUnit(){
    _decoder=NULL;
    _encoder=NULL;
    _target_stream=0;
}
void ProcessUnit::process(){
	int size=0;
	list< boost::shared_ptr<Packet> >::iterator it; 
//	_decoder->open();
//	_encoder->open();
	for(it=_input_packets.begin();it!=_input_packets.end();it++){
	    boost::shared_ptr<Packet> p=*it;
	    size+=p->size;
	    Frame f=_decoder->decode(*p.get());
	    Packet ret=_encoder->encode(f);
	    boost::shared_ptr<Packet> pEnc(new Packet(ret));
	    _output_packets.push_back(pEnc);
	}
}
