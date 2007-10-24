#include "Job.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameInputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/File.h"
using namespace org::esb::hive::job;
using namespace org::esb::av;
using namespace org::esb::io;
Job::Job(){

}

Job::~Job(){
}

File & Job::getSourceFile(){return *_source;}
File & Job::getTargetFile(){return *_target;}
void Job::setSourceFile(File & source){_source=&source;}
void Job::setTargetFile(File & target){_target=&target;}

int Job::getStartTime(){return _startTime;}
int Job::getCompleteTime(){return _completeTime;}
void Job::setStartTime(int start){_startTime=start;}
void Job::setCompleteTime(int complete){_completeTime=complete;}
void Job::setId(int id){_id=id;}
int Job::getId(){return _id;}



