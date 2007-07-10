#ifndef ORG_ESB_HIVE_JOB_JOB_H
#define ORG_ESB_HIVE_JOB_JOB_H
#include "org/esb/util/Properties.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
using namespace org::esb::util;
using namespace org::esb::av;
namespace org{
namespace esb{
namespace hive{
namespace job{
class Job{
    private:
	Properties _props;
	FormatInputStream * _fis;
	PacketInputStream * _pis;
//	FormatOutputStream * _fos;
//	PacketOutputStream * _pos;
    public:
	Job(Properties &props);
	~Job();
	Properties & getProperties();
	Frame & getFrame();
	void putFrame(Frame & frame);
	Packet & getPacket();
	void putPacket(Packet & packet);
	Codec * getInputCodec();
	Codec * getOutputCodec();
//	void putPacket(Packet & packet);
};
}}}}
#endif

