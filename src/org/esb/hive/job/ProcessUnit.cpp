#include "ProcessUnit.h"
#include "org/esb/av/Frame.h"
using namespace org::esb::hive::job;
using namespace org::esb::av;


ProcessUnit::ProcessUnit(){
    _decoder=NULL;
    _encoder=NULL;
    _target_stream=0;
    _source_stream=0;
}

ProcessUnit::~ProcessUnit(){
/*
	if(_decoder!=NULL)
		delete _decoder;
	if(_encoder!=NULL)
		delete _encoder;
*/
//    _decoder=NULL;
//    _encoder=NULL;
    
//    _target_stream=0;
//    _input_packets.clear();
//    _output_packets.clear();
    
}

void ProcessUnit::process(){
	int insize=0, outsize=0;
	list< boost::shared_ptr<Packet> >::iterator it; 

	if(_decoder!=NULL)
		_decoder->open();
	if(_encoder!=NULL)
		_encoder->open();
	for(it=_input_packets.begin();it!=_input_packets.end();it++){
	    boost::shared_ptr<Packet> p=*it;
//		cout << "Input:"<<p->packet->size<<endl;
	    insize+=p->packet->size;
	    Frame f=_decoder->decode(*p);
	    
	    Packet ret=_encoder->encode(f);
	    boost::shared_ptr<Packet> pEnc(new Packet(ret));
    	    cout << "Output:"<<ret.packet->size << "\tKeyFrame:" << ret.isKeyFrame() << endl;
	    outsize+=ret.packet->size;
	    _output_packets.push_back(pEnc);
	    
	}
	cout <<"InputSize:"<<insize<<"OutputSize:"<<outsize<<endl;


	if(_decoder!=NULL)
		delete _decoder;
	if(_encoder!=NULL)
		delete _encoder;
	

}
