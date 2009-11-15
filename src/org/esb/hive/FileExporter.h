#ifndef ORG_ESB_HIE_FILEEXPORTER
#define ORG_ESB_HIE_FILEEXPORTER
#include "org/esb/av/AV.h"
#include <map>
class FileExporter
{
public:
	static void exportFile(int fileid);
private:
	FileExporter(void);
	~FileExporter(void);

	struct StreamData{
		int64_t in_start_time;
		int64_t out_start_time;
		int in_stream_index;
		int out_stream_index;
		AVRational in_timebase;
		AVRational out_timebase;
	};
	static std::map<int, StreamData> _stream_map;
	static std::map<int, StreamData> _source_stream_map;

};


#endif
