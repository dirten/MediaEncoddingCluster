/* 
 * File:   pu.cpp
 * Author: jhoelscher
 *
 * Created on 29. September 2009, 13:33
 */
#include "org/esb/av/Packet.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include <stdlib.h>
#include <string>
/*
 * 
 */
using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::hive::job;

void help() {
  std::cout << "usage: pu <view|execute> infile [outfile]" << std::endl;
}

void execute(char * infile, char * outfile) {
  FileInputStream fis(infile);
  ObjectInputStream ois(&fis);
  ProcessUnit pu;
  ois.readObject(pu);
  pu.process();
  FileOutputStream fos(outfile);
  ObjectOutputStream oos(&fos);
  oos.writeObject(pu);
  delete pu._decoder;
  delete pu._encoder;
}
void view_packet_data(Packet * p){
    int64_t inpts=0;
    int64_t indts=0;
    AVRational intb;
    int64_t indur=0;
    memset(&intb,0,sizeof(intb));
    bool isKey=false;
    if(p!=NULL){
      inpts=p->getPts();
      indts=p->getDts();
      intb=p->getTimeBase();
      indur=p->getDuration();
      isKey=p->isKeyFrame();
    }
    printf("%20lld|", inpts);
    printf("%20lld|", indts);
    printf("%6d/", intb.num);
    printf("%6d/", intb.den);
    printf("%6d", indur);
    printf("%1s", isKey?"x":" ");

}

void view(char * filename) {
  FileInputStream fis(filename);
  ObjectInputStream ois(&fis);
  ProcessUnit pu;
  ois.readObject(pu);
//  logdebug(pu._decoder->toString());
//  logdebug(pu._encoder->toString());
  printf("----------------------------------------------------------------------------------------------------------");
cout << endl;
  printf("%10s|", "inframes");
  printf("%10s|", "outframes");
  printf("%10s|", "decoder");
  printf("%10s|", "encoder");
  cout << endl;
  printf("%10ld|", pu._input_packets.size());
  printf("%10ld|", pu._output_packets.size());
  printf("%10ld|", pu._decoder->getCodecId());
  printf("%10ld|", pu._encoder->getCodecId());
  cout << endl;
  printf("----------------------------------------------------------------------------------------------------------");
  cout << endl;
  int c=max(pu._input_packets.size(),pu._output_packets.size());
  std::vector<boost::shared_ptr<Packet> > in(pu._input_packets.begin(),pu._input_packets.end());
  std::vector<boost::shared_ptr<Packet> > out(pu._output_packets.begin(),pu._output_packets.end());
    printf("%10s|", "count");
    printf("%20s|", "pts");
    printf("%20s|", "dts");
    printf("%6s/", ".num");
    printf("%6s/", ".den");
    printf("%6s", "dur");
    printf("%ss", "K");
    printf("%20s|", "pts");
    printf("%20s|", "dts");
    printf("%6s/", ".num");
    printf("%6s/", ".den");
    printf("%6s", "dur");
    printf("%ss", "K");
  cout << endl;
  printf("----------------------------------------------------------------------------------------------------------");
  cout << endl;

  for(int a=0;a<c;a++){
    printf("%10d|", a);
    if(in.size()>a)
      view_packet_data(in[a].get());
    else
      view_packet_data(NULL);
    if(out.size()>a)
      view_packet_data(out[a].get());
    else
      view_packet_data(NULL);
    cout << endl;
  }
cout << endl;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    help();
    exit(0);
  }
  avcodec_register_all();
  av_register_all();

  std::string command = argv[1];
  char * infilename = argv[2];
  if (command == "view")view(infilename);
  if (command == "execute")execute(infilename, argv[3]);

  return (EXIT_SUCCESS);
}

