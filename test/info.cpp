
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include <vector>



using namespace org::esb::av;
using namespace org::esb::util;

int main(int argc, char ** argv) {
  Log::open("");
	avcodec_init();
	avcodec_register_all();
	av_register_all();
  
  if (argc < 2)return 1;
  long long int packet_start = 0;
  int packet_count = 50;
  if (argc > 2)
   sscanf(argv[2],"%llu",&packet_start);
  if (argc > 3)
    packet_count = atoi(argv[3]);
  File file(argv[1]);
  FormatInputStream fis(&file);
  fis.dumpFormat();
  AVFormatContext * f = fis.getFormatContext();

  cout << endl;
  cout << "<File Information><TIME_BASE>" << AV_TIME_BASE << endl;
  //      cout <<"-----------------"<<endl;
  cout << "#streams\tformatname\ttimestamp\tstart\tduration\tfilename\tidxsize" << endl;
  cout << "---------------------------------------------------------------------------------" << endl;
  cout << f->nb_streams << "\t\t";
  cout << f->iformat->name << "\t\t";
  cout << f->timestamp << "\t\t";
  cout << f->start_time << "\t";
  cout << f->duration << "\t";
  cout << f->filename << "\t";
//  cout << sizeof(*f->streams[0]->index_entries);
  cout << endl;
  int streams = fis.getStreamCount();
  std::vector<long long int> start_dts;
  std::vector<long long int> start_pts;
  cout << endl;
  cout << "<Stream Information>" << endl;
  cout << "#\tindex\tcodec\tnum\tden\tquality\tstart\tfirst_dts\tduration\tnb_index_entries" << endl;
  cout << "-------------------------------------------------------------------------" << endl;
  for (int a = 0; a < streams; a++) {
    StreamInfo * s = fis.getStreamInfo(a);
    cout << a << "\t";
    cout << s->getIndex() << "\t";
    cout << s->getCodecId() << "\t";
    cout << s->getTimeBase().num << "\t";
    cout << s->getTimeBase().den << "\t";
    cout << s->getQuality() << "\t";
    cout << s->getFirstPts() << "\t";
    cout << s->getFirstDts() << "\t";
    cout << s->getDuration() << "\t";
    cout << s->getNBIndexEntries() << "\t";
    cout << endl;
    start_dts.push_back(s->getFirstDts());
    start_pts.push_back(s->getFirstPts());
  }


  cout << endl;
  cout << "<Codec Information>" << endl;
  cout << "#\tindex\ttype\tcodec\tnum\tden\tFrameSize\trepeat_pict\tticks\tbframes" << endl;
  cout << "-------------------------------------------------------------------------" << endl;
  for (int a = 0; a < streams; a++) {
    StreamInfo * s = fis.getStreamInfo(a);
    cout << a << "\t";
    cout << s->getIndex() << "\t";
    cout << s->getCodecType() << "\t";
    cout << s->getCodecId() << "\t";
    cout << s->getCodecTimeBase().num << "\t";
    cout << s->getCodecTimeBase().den << "\t";
    cout << s->getFrameBytes() << "\t";
	printf("%10d\t", fis.getAVStream(a)->parser?fis.getAVStream(a)->parser->repeat_pict+2:-1);
//	cout << fis.getAVStream(a)->parser?fis.getAVStream(a)->parser->repeat_pict+2:-1;
	cout << fis.getAVStream(a)->codec->ticks_per_frame<< "\t";
    cout << fis.getAVStream(a)->codec->has_b_frames;
    cout << endl;
  }

  
  PacketInputStream pis(&fis);
  cout << endl;
  cout << "<Packet Information>" << endl;
  //      cout << "#\tpts\tdts\tsize\tindex\tflags\tiskey\tduration\tpos"<<endl;
  printf("%10s|", "#");
  printf("%20s|", "pts");
  printf("%20s|", "dts");
  printf("%20s|", "offset-pts");
  printf("%20s|", "offset-dts");
//  printf("%20s|", "timebase");
  printf("%8s|", "size");
  printf("%2s|", "ix");
//  printf("%10s|", "pidx");
//  printf("%10s|", "idxentry");
  printf("%2s|", "k");
  printf("%5s|", "dur");
  printf("%2s|", "type");
  cout << endl;
  cout << "------------------------------------------------------------------------------------------------------------" << endl;
  cout << "seeking to :"<<packet_start<<endl;
  if(packet_start>0)
    fis.seek(0,packet_start);
  for (int a = 0; a < packet_count; a++) {
    Packet p;
    if (pis.readPacket(p) != 0) {
      logdebug("Last Packet REACHED");
      break;
    }
//	if(p.getStreamIndex()!=1)continue;
//	av_pkt_dump_log(NULL, AV_LOG_DEBUG, p.packet, false);
//    if (a < packet_start)continue;
    //        cout <<a<<"\t";
    printf("%10ld|", a);
    printf("%20lld|", p.packet->pts);
    printf("%20lld|", p.packet->dts);
    printf("%20lld|", p.packet->pts-start_pts[p.packet->stream_index]);
    printf("%20lld|", p.packet->dts-start_dts[p.packet->stream_index]);
//    printf("%20lld|",fis.getFormatContext()->streams[p.packet->stream_index]->time_base);
    printf("%8d|", p.packet->size);
    printf("%2d|", p.packet->stream_index);
	int pidx=av_index_search_timestamp(f->streams[p.packet->stream_index], p.packet->dts, 0);
//	printf("%10d|", pidx);
//	printf("%10d|", f->streams[p.packet->stream_index]->index_entries[pidx].timestamp);
//	printf("%10d|", av_index_search_timestamp(f->streams[p.packet->stream_index], p.packet->dts, 0));
    printf("%s|", p.isKeyFrame()==1?"x ":"  ");
    printf("%5d|", p.packet->duration);
	std::string type;
//	if(f->streams[p.packet->stream_index]->parser){
	switch(p._pict_type){
		case FF_I_TYPE: type="I";break;
		case FF_B_TYPE: type="B";break;
		case FF_BI_TYPE: type="BI";break;
		case FF_P_TYPE: type="P";break;
		default :type="U";break;
	}
  printf("%2s|", type.c_str());
    //        cout <<p.packet->pts<<"\t";
    //        cout <<p.packet->dts<<"\t";
    //        cout <<p.packet->size<<"\t";
    //        cout <<p.packet->stream_index<<"\t";
    //        cout <<p.packet->flags<<"\t";
    //        cout <<p.isKeyFrame()<<"\t";
    //        cout <<p.packet->duration<<"\t\t";
    //        cout <<p.packet->pos<<"\t";
    cout << endl;
  }

  cout << endl;
  cout << endl;

  return 0;
}

