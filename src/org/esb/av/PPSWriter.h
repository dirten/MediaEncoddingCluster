#ifndef ORG_ESB_AV_PPSWRITER_H
#define ORG_ESB_AV_PPSWRITER_H
#include "org/esb/av/Frame.h"
namespace org{
namespace esb{
namespace av{
class PPSWriter{
    private:
	const char * _filePattern;
	int _fileCounter;
    public:
	PPSWriter(const char * fileNamePattern);
	void writeFrame(Frame * frame);
};
}}}
#endif
