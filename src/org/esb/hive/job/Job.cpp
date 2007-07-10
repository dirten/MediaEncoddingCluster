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
    File infile(p.getProperty("source.file"));
    _fis=new FormatInputStream(&infile);
    _pis=new PacketInputStream(_fis->getStream(0));
/*    File outfile(p.getProperty("source.file"));
    _fos=new FormatOutputStream(&outfile);
    _pos=new PacketOutputStream(_fos->getStream(0));
    */
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

Codec * Job::getInputCodec(){
    return _pis->getCodec();
}

Codec * Job::getOutputCodec(){
    return _pis->getCodec();
}

void Job::putPacket(Packet & packet){

}

