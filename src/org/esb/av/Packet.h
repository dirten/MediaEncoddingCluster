#ifndef ORG_ESB_PACKET_H
#define ORG_ESB_PACKET_H

#include "AV.h"
#include <iostream>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/serialization/split_member.hpp>
//using namespace boost;
namespace org {
namespace esb {
namespace av {
class Packet {
public:
	Packet();
	Packet(int size);
	~Packet();
	uint8_t * getData();
	int getSize();
	int getPts();
	int getDts();
	int getFlags();
	int getStreamIndex();
	int getDuration();
	bool isKeyFrame();
	void * getPriv();
	int64_t getPosition();
	//        	    boost::shared_ptr<unsigned char*> data;
	Packet(const Packet & packet);
	Packet operator=(Packet & packet);

	AVPacket * packet;
	boost::shared_ptr<AVPacket> packetPtr;
//private:
	AVPacket _packet;
	bool isCopy;
	bool callDestruct;
	uint8_t _data;
	friend class boost::serialization::access;
	//		    void serialize(boost::archive::Archive & ar, const unsigned int version);
/*
	template<class Archive> void serialize(Archive & ar,
			const unsigned int version) {
		ar & packet->size;

		if (packet->data==NULL) {
			packet->data=new uint8_t[packet->size];
			memset(packet->data, 0, packet->size);
			callDestruct=true;
		}
		ar & boost::serialization::make_binary_object(packet->data,	packet->size);
		ar & packet->pts;
		ar & packet->dts;
		ar & packet->flags;
		ar & packet->stream_index;
		ar & packet->duration;
		ar & packet->pos;
	}
*/
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {

		ar & packet->size;
/*
		if (packet->data==NULL) {
			packet->data=new uint8_t[packet->size];
			memset(packet->data, 0, packet->size);
			callDestruct=true;
		}*/
		ar & boost::serialization::make_binary_object(packet->data,	packet->size);
		ar & packet->pts;
		ar & packet->dts;
		ar & packet->flags;
		ar & packet->stream_index;
		ar & packet->duration;
		ar & packet->pos;


    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
		ar & packet->size;
//		std::cout << "PacketSize:"<<packet->size<<std::endl;;
		packet->data=new unsigned char[packet->size + FF_INPUT_BUFFER_PADDING_SIZE];
		memset(packet->data, 0, packet->size + FF_INPUT_BUFFER_PADDING_SIZE);
		callDestruct=true;
		ar & boost::serialization::make_binary_object(packet->data,	packet->size);
		ar & packet->pts;
		ar & packet->dts;
		ar & packet->flags;
		ar & packet->stream_index;
		ar & packet->duration;
		ar & packet->pos;

    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

};
}
}
}
#endif
