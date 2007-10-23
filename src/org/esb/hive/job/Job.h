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

    public:
		Job();
		~Job();
		File & getSourceFile(void);
		File & getTargetFile(void);
};
}}}}
#endif

