#include "ProcessUnit.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/av/FrameConverter.h"
using namespace org::esb::hive::job;
using namespace org::esb::av;


ProcessUnit::ProcessUnit(){
    _decoder=NULL;
    _encoder=NULL;
    codec=NULL;
    _target_stream=0;
    _source_stream=0;
    _frame_group=0;
}

ProcessUnit::~ProcessUnit(){
/*
	if(_decoder!=NULL)
		delete _decoder;
	if(_encoder!=NULL)
		delete _encoder;
	if(codec!=NULL)
		delete codec;
*/
//    _decoder=NULL;
//    _encoder=NULL;
    
//    _target_stream=0;
//    _input_packets.clear();
//    _output_packets.clear();
    
}

void ProcessUnit::process(){
	int insize=0, outsize=0;

	if(_decoder!=NULL)
		_decoder->open();
	if(_encoder!=NULL)
		_encoder->open();

	FrameFormat format;
	format.pixel_format=(PixelFormat)_encoder->getPixelFormat();//PIX_FMT_YUV420P;
	format.height=_encoder->getHeight();
	format.width=_encoder->getWidth();
//	cout << "Create Formater:\twidth:"<<format.width<<"\theight"<<format.height<<endl;

	FrameConverter conv(format);
//	cout << "start decoding encoding"<<endl;
	list< boost::shared_ptr<Packet> >::iterator it; 
	for(it=_input_packets.begin();it!=_input_packets.end();it++){
		
	    boost::shared_ptr<Packet> p=*it;
	    insize+=p->packet->size;
//	    cout << "start decoding insize:"<<p->packet->size<<endl;

	    Frame tmp=_decoder->decode(*p);

		Frame f=conv.convert(tmp);
//	    cout << "FrameSize:"<<f.getSize()<<endl;
	    Packet ret=_encoder->encode(f);

	    boost::shared_ptr<Packet> pEnc(new Packet(ret));
	    outsize+=pEnc->packet->size;
	    _output_packets.push_back(pEnc);
	    
	}
//	cout << "decoding encoding ready"<<endl;

	 logdebug("InputSize:"<<insize<<"OutputSize:"<<outsize);
    
	if(_decoder!=NULL)
		delete _decoder;
	if(_encoder!=NULL)
		delete _encoder;
	

}
