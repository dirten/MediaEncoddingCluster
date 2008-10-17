#include "ProcessUnit.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/av/FrameConverter.h"

#include "org/esb/util/Log.h"
using namespace org::esb::hive::job;
using namespace org::esb::av;


ProcessUnit::ProcessUnit(){
    _decoder=NULL;
    _encoder=NULL;
    codec=NULL;
    _target_stream=0;
    _source_stream=0;
    _frame_group=0;
    id=0;
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
	logdebug("Codex openned");
	FrameFormat in_format;
	in_format.pixel_format=(PixelFormat)_decoder->getPixelFormat();//PIX_FMT_YUV420P;
	in_format.height=_decoder->getHeight();
	in_format.width=_decoder->getWidth();
	in_format.channels=_decoder->getChannels();
	in_format.samplerate=_decoder->getSampleRate();
	logdebug("Input Formater created");

	FrameFormat out_format;
	out_format.pixel_format=(PixelFormat)_encoder->getPixelFormat();//PIX_FMT_YUV420P;
	out_format.height=_encoder->getHeight();
	out_format.width=_encoder->getWidth();
	out_format.channels=_encoder->getChannels();
	out_format.samplerate=_encoder->getSampleRate();
	logdebug("Output Formater created");
	
//	cout << "Create Formater:\twidth:"<<format.width<<"\theight"<<format.height<<endl;

	FrameConverter conv(in_format,out_format);
	logdebug("Converter created");

	//	cout << "start decoding encoding"<<endl;
	list< boost::shared_ptr<Packet> >::iterator it; 
	multiset<boost::shared_ptr<Frame>, PtsComparator > pts_list;
	multiset<boost::shared_ptr<Packet>, PtsPacketComparator > pts_packets;
	int a=0;
	int counter=0;
	for(it=_input_packets.begin();it!=_input_packets.end();it++){		
//		logdebug("Loop");
	    boost::shared_ptr<Packet> p=*it;
      
	    insize+=p->packet->size;

	    if(p->isKeyFrame()){
	      cout <<"KeyFrame\t";
	    }
//	    if(tmp._buffer==0)continue;
//        boost::shared_ptr<Frame> fr(new Frame(tmp));
//        pts_list.insert(fr);

	    cout <<"PacketPts:"<<p->packet->pts<<"\tPacketDts:"<<p->packet->dts<<"\t";
//	    cout <<"\tFramePts:"<<tmp.getPts()<<"\tFrameDts:"<<tmp.getDts();
//	    cout <<"\tFrame*Pts:"<<fr->getPts()<<"\tFrame*Dts:"<<fr->getDts();
//        cout << endl;

	    Frame tmp=_decoder->decode(*p);
//		logdebug("Frame Decoded");
		if(_frame_count>=counter&&tmp.pict_type==FF_I_TYPE){
			break;
		}
	    if(tmp._buffer==0){
	      continue;
	    }
//		logdebug("Frame Buffer > 0");
        
        
      
//	    fr->setPts(++a);
//	    fr->setDts(AV_NOPTS_VALUE);

		Frame f=conv.convert(tmp);
//		logdebug("Frame Converted");

	    f.setPts(f.getDts());
//	    f.setPts(++a);
//	    tmp.setDts(AV_NOPTS_VALUE);
	    Packet ret=_encoder->encode(f);
//		logdebug("Frame Encoded");

		boost::shared_ptr<Packet> pEnc(new Packet(ret));
//		logdebug("Packet Created");

		//	    pEnc->packet->dts=AV_NOPTS_VALUE;
	    outsize+=pEnc->packet->size;
	    _output_packets.push_back(pEnc);
//		logdebug("Packet Added");

		//	    cout <<"FramePts:"<<f.pts<<"\tFrameDts:"<<f.dts;
//	    cout <<"\tPacketPts:"<<pEnc->packet->pts<<"\tPacketDts:"<<pEnc->packet->dts<<endl;


//        pts_packets.insert(p);
	}
	
//	cout <<"ListSize:"<<_input_packets.size()<<"\tSetSize:"<<pts_list.size()<<endl;

//	multiset<boost::shared_ptr<Frame>, PtsComparator>::iterator pts_it;
/*
	multiset<boost::shared_ptr<Packet>, PtsPacketComparator>::iterator packet_it;
	a=0;

	for(packet_it=pts_packets.begin();packet_it!=pts_packets.end();packet_it++){
	    boost::shared_ptr<Packet> p=*packet_it;
	    insize+=p->packet->size;
	    Frame tmp=_decoder->decode(*p);
	    if(tmp._buffer==0)continue;

        boost::shared_ptr<Frame> fr(new Frame(tmp));
//	    fr->setDts(++a);

        pts_list.insert(fr);

      
	    tmp.setPts(++a);
	    tmp.setDts(AV_NOPTS_VALUE);
		Frame f=conv.convert(tmp);
	    Packet ret=_encoder->encode(f);
	    boost::shared_ptr<Packet> pEnc(new Packet(ret));
	    outsize+=pEnc->packet->size;
	    _output_packets.push_back(pEnc);
      
    }
*/
/*
	a=0;
	for(pts_it=pts_list.begin();pts_it!=pts_list.end();pts_it++){
	    boost::shared_ptr<Frame> tmp=*pts_it;
	    tmp->setPts(++a);
//	    tmp->setDts(AV_NOPTS_VALUE);
	    
		Frame f=conv.convert(*tmp);
//		f.dts=f.pts;
	    Packet ret=_encoder->encode(*tmp);
	    boost::shared_ptr<Packet> pEnc(new Packet(ret));
	    outsize+=pEnc->packet->size;
	    pEnc->packet->dts=AV_NOPTS_VALUE;
	    _output_packets.push_back(pEnc);
//	    cout <<"FramePts:"<<f.pts<<"\tFrameDts:"<<f.dts;
	    cout <<"\tPacketPts:"<<pEnc->packet->pts<<"\tPacketDts:"<<pEnc->packet->dts<<endl;
//	    cout << "Frame*Pts:"<<tmp->pts<<"\tPacketPts:"<<ret.packet->pts<<"Packet*Pts"<<pEnc->packet->pts<<endl;
	}
*/
	 logdebug("InputSize:"<<insize<<"OutputSize:"<<outsize);
    
	if(_decoder!=NULL)
		delete _decoder;
	if(_encoder!=NULL)
		delete _encoder;
	_decoder=0;
	_encoder=0;
	

}
