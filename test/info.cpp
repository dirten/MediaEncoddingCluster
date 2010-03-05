
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include <vector>
#include <limits>

//extern void dump_stream_format(AVFormatContext *ic, int i, int index, int is_output);
using namespace org::esb::av;
using namespace org::esb::util;
namespace bla {

  classlogger("info")
  int main(int argc, char ** argv) {
    Log::open("");
    avcodec_init();
    avcodec_register_all();
    av_register_all();

    if (argc < 2)return 1;
    long long int packet_start = 0;
    int packet_count = 50;
    if (argc > 2)
      sscanf(argv[2], "%llu", &packet_start);
    if (argc > 3)
      packet_count = atoi(argv[3]);
    File file(argv[1]);
    FormatInputStream fis(&file);
        fis.dumpFormat();
    AVFormatContext * f = fis.getFormatContext();
/*
    for (int i = 0; i < f->nb_streams; i++) {
      char buf[256];
      int flags = (false ? f->oformat->flags : f->iformat->flags);
      AVStream *st =f->streams[i];
      int g = av_gcd(st->time_base.num, st->time_base.den);
      AVMetadataTag *lang = av_metadata_get(st->metadata, "language", NULL, 0);
      avcodec_string(buf, sizeof (buf), st->codec, false);
      av_log(NULL, AV_LOG_INFO, "    Stream #%d.%d", index, i);
      if (flags & AVFMT_SHOW_IDS)
        av_log(NULL, AV_LOG_INFO, "[0x%x]", st->id);
      if (lang)
        av_log(NULL, AV_LOG_INFO, "(%s)", lang->value);
      av_log(NULL, AV_LOG_DEBUG, ", %d/%d", st->time_base.num / g, st->time_base.den / g);
      av_log(NULL, AV_LOG_INFO, ": %s", buf);

//      AVStream *st = f->streams[i];
      cout << "printing " << i << endl;
      if (st->sample_aspect_ratio.num && // default
          av_cmp_q(st->sample_aspect_ratio, st->codec->sample_aspect_ratio)) {
        AVRational display_aspect_ratio;
        av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den,
            st->codec->width * st->codec->sample_aspect_ratio.num,
            st->codec->height * st->codec->sample_aspect_ratio.den,
            1024 * 1024);
        printf("%d/%d|", st->sample_aspect_ratio.num, st->sample_aspect_ratio.den);
        printf("%d/%d|", st->codec->sample_aspect_ratio.num, st->codec->sample_aspect_ratio.den);
        printf("%d/%d", display_aspect_ratio.num, display_aspect_ratio.den);
      }
    }
    cout << "printing end" << endl;
*/
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
    cout << "#\tindex\tcodec\ttimebase\tframerate\tquality\tstart\tfirst_dts\tduration\tnb_index_entries" << endl;
    cout << "-------------------------------------------------------------------------" << endl;
    for (int a = 0; a < streams; a++) {
      StreamInfo * s = fis.getStreamInfo(a);
      cout << a << "\t";
      cout << s->getIndex() << "\t";
      cout << s->getCodecId() << "\t";
      cout << s->getTimeBase().num << "/" << s->getTimeBase().den << "\t";
      cout << s->getFrameRate().num << "/" << s->getFrameRate().den << "\t";
      //    cout << s->getTimeBase().den << "\t";
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
    cout << "#\tindex\ttype\tcodec\ttimebase\tframerate\tFrameSize\trepeat_pict\tticks\tbframes" << endl;
    cout << "-------------------------------------------------------------------------" << endl;
    for (int a = 0; a < streams; a++) {
      Decoder * dec = new Decoder(fis.getAVStream(a));
      dec->open();
      StreamInfo * s = fis.getStreamInfo(a);
      cout << a << "\t";
      cout << s->getIndex() << "\t";
      cout << s->getCodecType() << "\t";
      cout << s->getCodecId() << "\t";
      cout << s->getCodecTimeBase().num << "/" << s->getCodecTimeBase().den << "\t\t";
      cout << dec->getFrameRate().num << "/" << dec->getFrameRate().den << "\t\t";
      cout << s->getFrameBytes() << "\t";
      printf("%10d\t", fis.getAVStream(a)->parser ? fis.getAVStream(a)->parser->repeat_pict + 2 : -1);
      //	cout << fis.getAVStream(a)->parser?fis.getAVStream(a)->parser->repeat_pict+2:-1;
      cout << fis.getAVStream(a)->codec->ticks_per_frame << "\t";
      cout << fis.getAVStream(a)->codec->has_b_frames << "\t";
      //    if(fis.getAVStream(a)->codec->flags)
      cout << ((fis.getAVStream(a)->codec->flags & CODEC_FLAG_CLOSED_GOP) ? "closedgop\t" : "opengop\t");
      cout << endl;
      delete dec;
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
    printf("%10s|", "tb");
    printf("%10s|", "dar");
    cout << endl;
    cout << "------------------------------------------------------------------------------------------------------------" << endl;
//    cout << "seeking to :" << packet_start << endl;
    if (packet_start > 0)
      fis.seek(0, packet_start);
    for (int a = 0; a < packet_count; a++) {
      Packet p;
      if (pis.readPacket(p) != 0) {
        LOGDEBUG("Last Packet REACHED");
        break;
      }
      //	if(p.getStreamIndex()!=1)continue;
      //	av_pkt_dump_log(NULL, AV_LOG_DEBUG, p.packet, false);
      //    if (a < packet_start)continue;
      //        cout <<a<<"\t";
      printf("%10ld|", a);
      printf("%20lld|", p.packet->pts);
      printf("%20lld|", p.packet->dts);
      printf("%20lld|", p.packet->pts - start_pts[p.packet->stream_index]);
      printf("%20lld|", p.packet->dts - start_dts[p.packet->stream_index]);
      //    printf("%20lld|",fis.getFormatContext()->streams[p.packet->stream_index]->time_base);
      printf("%8d|", p.packet->size);
      printf("%2d|", p.packet->stream_index);
      int pidx = av_index_search_timestamp(f->streams[p.packet->stream_index], p.packet->dts, 0);
      //	printf("%10d|", pidx);
      //	printf("%10d|", f->streams[p.packet->stream_index]->index_entries[pidx].timestamp);
      //	printf("%10d|", av_index_search_timestamp(f->streams[p.packet->stream_index], p.packet->dts, 0));
      printf("%s|", p.isKeyFrame() == 1 &&fis.getFormatContext()->streams[p.getStreamIndex()]->codec->codec_type==CODEC_TYPE_VIDEO? "x " : "  ");
      printf("%5d|", p.packet->duration);
      std::string type;
      //	if(f->streams[p.packet->stream_index]->parser){
      switch (p._pict_type) {
        case FF_I_TYPE: type = "I";
          break;
        case FF_B_TYPE: type = "B";
          break;
        case FF_BI_TYPE: type = "BI";
          break;
        case FF_P_TYPE: type = "P";
          break;
        default:type = "U";
          break;
      }
      printf("%2s|", type.c_str());
      printf("%d/%d|", p.getTimeBase().num, p.getTimeBase().den);
      //        cout <<p.packet->pts<<"\t";
      //        cout <<p.packet->dts<<"\t";
      //        cout <<p.packet->size<<"\t";
      //        cout <<p.packet->stream_index<<"\t";
      //        cout <<p.packet->flags<<"\t";
      //        cout <<p.isKeyFrame()<<"\t";
      //        cout <<p.packet->duration<<"\t\t";
      //        cout <<p.packet->pos<<"\t";
      AVStream *st = fis.getFormatContext()->streams[p.getStreamIndex()];

        AVRational display_aspect_ratio;
        av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den,
            st->codec->width * st->codec->sample_aspect_ratio.num,
            st->codec->height * st->codec->sample_aspect_ratio.den,
            1024 * 1024);
        printf("%d/%d|", st->sample_aspect_ratio.num, st->sample_aspect_ratio.den);
        printf("%d/%d|", st->codec->sample_aspect_ratio.num, st->codec->sample_aspect_ratio.den);
        printf("%d/%d", display_aspect_ratio.num, display_aspect_ratio.den);
      cout << endl;
    }

    cout << endl;
    cout << endl;

    return 0;
  }
}

int main(int argc, char**argv) {
  //  std::cout << std::numeric_limits<int>::max() <<std::endl;
  bla::main(argc, argv);
}

