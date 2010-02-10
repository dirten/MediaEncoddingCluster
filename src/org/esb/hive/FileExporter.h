#ifndef ORG_ESB_HIE_FILEEXPORTER
#define ORG_ESB_HIE_FILEEXPORTER
#include "org/esb/av/AV.h"
#include "org/esb/av/Packet.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include "org/esb/util/Log.h"
class FileExporter
{
  classlogger("org.esb.hive.FileExporter")
public:
	static void exportFile(int fileid);
private:
	FileExporter(void);
	~FileExporter(void);

	struct StreamData{
		int64_t in_start_time;
		int64_t out_start_time;
                int64_t last_timestamp;
                int64_t next_timestamp;
		int packet_duration;
		AVRational packet_timebase;

		int in_stream_index;
		int out_stream_index;
		AVRational in_timebase;
		AVRational out_timebase;
                int stream_type;
	};
	static std::map<int, StreamData> _stream_map;
	static std::map<int, StreamData> _source_stream_map;
	static bool ptsComparator(boost::shared_ptr<org::esb::av::Packet>,boost::shared_ptr<org::esb::av::Packet> );
	static bool dtsComparator(boost::shared_ptr<org::esb::av::Packet>,boost::shared_ptr<org::esb::av::Packet> );

};


#endif
