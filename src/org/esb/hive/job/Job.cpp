#include "Job.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameInputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/File.h"
using namespace org::esb::hive::job;
using namespace org::esb::av;
using namespace org::esb::io;
Job::Job(Properties & p){
    _props=p;
    File file(p.getProperty("source.file"));
    _fis=new FormatInputStream(&file);
    _pis=new PacketInputStream(_fis);
}

Job::~Job(){
    delete _fis;
    delete _pis;
}

Properties & Job::getProperties(){
    return _props;
}

Frame & Job::getFrame(){

}

void Job::putFrame(Frame & frame){

}

Packet & Job::getPacket(){
    return _pis->readPacket();
}

void Job::putPacket(Packet & packet){

}

