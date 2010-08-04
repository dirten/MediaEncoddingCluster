#include "PreviewPanel.h"
#include "Wt/WFitLayout"
#include "Wt/WDefaultLayout"
#include "Wt/WMemoryResource"
#include "Wt/WImage"
#include "Wt/WText"

#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/util/StringUtil.h"

#include "org/esb/av/StreamInfo.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/PGMUtil.h"
namespace org{
  namespace esb{
    namespace web{
      using namespace org::esb::av;
      using namespace org::esb::io;
      using namespace org::esb::util;



      class PacketSink : public Sink {
      public:

        PacketSink() {
        }

        void write(void * p) {
          Packet* pt = (Packet*) p;
          boost::shared_ptr<Packet> pEnc(new Packet(*pt));
          pkts.push_back(pEnc);
        }

        std::list<boost::shared_ptr<Packet> > getList() {
          return pkts;
        }
      private:
        std::list<boost::shared_ptr<Packet> > pkts;

      };

      PreviewPanel::PreviewPanel():Wt::Ext::Panel(){
        setTitle("Preview");
//        setLayout(new Wt::WFitLayout);
        setLayout(new Wt::WDefaultLayout);
        _video_stream_index=0;
        //        layout()->addWidget(new Wt::WText("bla"));

      }

      PreviewPanel::~PreviewPanel(){

      }

      void PreviewPanel::setProject(Ptr<db::Project>p){
        _project=p;
        if(_project->mediafiles().get().count()>0){
          db::MediaFile file=_project->mediafiles().get().one();
          std::string filename=file.path+"/"+file.filename;
          File infile(filename);
          _fis=Ptr<FormatInputStream>(new FormatInputStream(&infile));
          _pis=Ptr<PacketInputStream>(new PacketInputStream(_fis.get()));

          int c=_fis->getStreamCount();
          _encoder=Ptr<Encoder>(new Encoder(CODEC_ID_BMP));
          _encoder->setPixelFormat(PIX_FMT_BGR24);
          _encoder->setBitRate(1024000);
          _encoder->setGopSize(10);
          _sink=Ptr<PacketSink>(new PacketSink());
          _encoder->setSink(_sink.get());
          _encoder->setTimeBase(1,25);
          AVRational ar;
          ar.num=1;
          ar.den=25;
          _encoder->setFrameRate(ar);
          for(int a = 0;a<c;a++){
            StreamInfo * info=_fis->getStreamInfo(a);
            if(info->getCodecType()==CODEC_TYPE_VIDEO){
              _video_stream_index=info->getIndex();
              _decoder=Ptr<Decoder>(new Decoder(_fis->getAVStream(_video_stream_index)));
              _encoder->setWidth(info->getWidth());
              _encoder->setHeight(info->getheight());
              _decoder->open();
              LOGDEBUG("Video Stream found!");
              break;
            }
          }
          _encoder->open();
          _conv=Ptr<FrameConverter>(new FrameConverter(_decoder.get(), _encoder.get()));
        }
      }
      void PreviewPanel::preview(){

        Packet * packet;
        Frame * frame=NULL;
        while ((packet = _pis->readPacket()) != NULL /*&& !_isStopSignal*/) {
          if(packet->getStreamIndex()==_video_stream_index){
            frame=_decoder->decode2(*packet);
            if(frame->isFinished()){
              break;
            }
          }
        }
        Wt::WMemoryResource *imageResource = new Wt::WMemoryResource("image/png", this);

        static const unsigned char gifData[]
        = { 0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x01, 0x00, 0x01, 0x00,
          0x80, 0xff, 0xff, 0xdb, 0xdf, 0xef, 0xff, 0xff, 0xff, 0x21,
          0xf9, 0x04, 0x01, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00,
          0x00, 0x00, 0x01, 0x00, 0x01, 0xff, 0xff, 0x02, 0x02, 0x44,
          0x01, 0x00, 0x3b,0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x01, 0x00, 0x01, 0x00,
          0x80, 0xff, 0xff, 0xdb, 0xdf, 0xef, 0xff, 0xff, 0xff, 0x21,
          0xf9, 0x04, 0x01, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00,
          0x00, 0x00, 0x01, 0x00, 0x01, 0xff, 0xff, 0x02, 0x02, 0x44,
          0x01, 0x00, 0x3b,0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x01, 0x00, 0x01, 0x00,
          0x80, 0xff, 0xff, 0xdb, 0xdf, 0xef, 0xff, 0xff, 0xff, 0x21,
          0xf9, 0x04, 0x01, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00,
          0x00, 0x00, 0x01, 0x00, 0x01, 0xff, 0xff, 0x02, 0x02, 0x44,
          0x01, 0x00, 0x3b };
        std::string data;
        data+="P5\n";
        data+=StringUtil::toString(frame->getWidth());
        data+=" ";
        data+=StringUtil::toString(frame->getHeight());
        data+="\n";
        data+="255\n";
        for(int i=0;i<frame->getHeight();i++)
          data.append((char*)frame->getData() + i * frame->getAVFrame()->linesize[0], frame->getWidth());
        //        data.append((char*)frame->getData(), frame->getSize());
        Frame * f= new Frame(_encoder->getPixelFormat(), _encoder->getWidth(), _encoder->getHeight());
        _conv->convert(*frame, *f);
        _encoder->encode(*f);
        std::list<boost::shared_ptr<Packet> > packets=_sink->getList();
        boost::shared_ptr<Packet> picture=packets.front();
        FileOutputStream fos("test.bmp");
        fos.write((char*)picture->getData(), picture->getSize());
//        fos.close();

        PGMUtil::save("test2.png", frame);
        imageResource->setData((char*)picture->getData(), picture->getSize());
        Wt::WImage *image = new Wt::WImage(imageResource, "preview image");
        //        image->resize(100,100);
        layout()->addWidget(image);
      }
    }
  }
}

