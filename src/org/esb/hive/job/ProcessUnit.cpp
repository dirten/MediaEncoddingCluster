#include "ProcessUnit.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/av/FrameConverter.h"
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

	FrameFormat format;
	format.pixel_format=(PixelFormat)_encoder->getPixelFormat();//PIX_FMT_YUV420P;
	format.height=_encoder->getHeight();
	format.width=_encoder->getWidth();
	cout << "Create Formater"<<endl;

	FrameConverter conv(format);
	cout << "start decoding encoding"<<endl;
	for(it=_input_packets.begin();it!=_input_packets.end();it++){
		
	    boost::shared_ptr<Packet> p=*it;
	    insize+=p->packet->size;
	    Frame tmp=_decoder->decode(*p);
	    Frame f;
        if (_decoder->ctx->codec_type == CODEC_TYPE_VIDEO)
  	      f=conv.convert(tmp);
  	    else
  	      f=tmp;
	    
	    Packet ret=_encoder->encode(f);
	    boost::shared_ptr<Packet> pEnc(new Packet(ret));
	    outsize+=ret.packet->size;
	    _output_packets.push_back(pEnc);
	    
	}
	cout << "decoding encoding ready"<<endl;

	 logdebug("InputSize:"<<insize<<"OutputSize:"<<outsize);
	if(_decoder!=NULL)
		delete _decoder;
	if(_encoder!=NULL)
		delete _encoder;
	

}
