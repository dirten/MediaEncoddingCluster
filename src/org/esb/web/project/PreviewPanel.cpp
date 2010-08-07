#include "PreviewPanel.h"
#include "Wt/WFitLayout"
#include "Wt/WDefaultLayout"
#include "Wt/WImage"
#include "Wt/WText"
#include "Wt/Ext/ToolBar"

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
        LOGDEBUG("PreviewPanel::PreviewPanel()"<<this);
        setTitle("Preview");
        setLayout(new Wt::WDefaultLayout());
        _video_stream_index=0;
        _video_preview_frame=0;
        _scale_factor=75;

        _buttonSignalMap = new Wt::WSignalMapper<Wt::Ext::Component*>(this);
        _buttonSignalMap->mapped.connect(SLOT(this, PreviewPanel::controllerButtonClicked));

        Wt::Ext::ToolBar *toolbar=new Wt::Ext::ToolBar();
        setBottomToolBar(toolbar);



        //toolbar->add(new Wt::Ext::Button("test"));
        Wt::Ext::Button * prev10Button = new Wt::Ext::Button("prev 10");
        Wt::Ext::Button * prevButton = new Wt::Ext::Button("prev");
        Wt::Ext::Button * nextButton = new Wt::Ext::Button("next");
        Wt::Ext::Button * next10Button = new Wt::Ext::Button("next 10");
        toolbar->add(prev10Button);
        toolbar->add(prevButton);
        toolbar->add(nextButton);
        toolbar->add(next10Button);



        _scale_combo=new Wt::Ext::ComboBox();
        _scale_combo->addItem("150");
        _scale_combo->addItem("125");
        _scale_combo->addItem("100");
        _scale_combo->addItem("75");
        _scale_combo->addItem("50");
        _scale_combo->addItem("25");
        toolbar->add(_scale_combo);
        
        _buttonSignalMap->mapConnect(_scale_combo->activated,_scale_combo);
        _buttonSignalMap->mapConnect(_scale_combo->keyWentUp,_scale_combo);
        _scale_combo->setObjectName("preview_scale");

        prev10Button->setObjectName("prev10");
        prevButton->setObjectName("prev");
        next10Button->setObjectName("next10");
        nextButton->setObjectName("next");

//        addVideoButton->clicked.connect(SLOT(this, InputFilePanel::addVideoButtonClicked));



        _buttonSignalMap->mapConnect(prev10Button->clicked,prev10Button);
        _buttonSignalMap->mapConnect(prevButton->clicked,prevButton);
        _buttonSignalMap->mapConnect(nextButton->clicked,nextButton);
        _buttonSignalMap->mapConnect(next10Button->clicked,next10Button);

        image = new Wt::WImage();
        image->setAlternateText("preview image");
        layout()->addWidget(image);
        

      }

      void PreviewPanel::controllerButtonClicked(Wt::Ext::Component * btn){
        if(btn->objectName()=="prev10"){
          if(_video_preview_frame>=10)
            _video_preview_frame=10;
          else
            _video_preview_frame=0;
        }else
          if(btn->objectName()=="prev"){
            if(_video_preview_frame>0)
              _video_preview_frame=1;
        }else
          if(btn->objectName()=="next"){
        _video_preview_frame=-1;
        }else
          if(btn->objectName()=="next10"){
        _video_preview_frame=-10;
        }else
          if(btn->objectName()=="preview_scale"){
            int factor=atoi(((Wt::Ext::ComboBox*)_scale_combo)->currentText().narrow().c_str());
            LOGDEBUG("scale factor"<<factor);
            if(factor>0)
              _scale_factor=factor;
        }
        preview();
        LOGDEBUG(btn->objectName());

      }

      PreviewPanel::~PreviewPanel(){
        LOGDEBUG("PreviewPanel::~PreviewPanel()"<<this)
        //Wt::Ext::Panel::~Panel();
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
          _encoder=Ptr<Encoder>(new Encoder(CODEC_ID_PNG));
          _encoder->setPixelFormat(PIX_FMT_BGR24);
          _encoder->setPixelFormat(PIX_FMT_RGB24);
          
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
          _encoder->setWidth((_encoder->getWidth()*100)/_scale_factor);
          _encoder->setHeight((_encoder->getHeight()*100)/_scale_factor);
          LOGDEBUG(_scale_factor)
          LOGDEBUG((_encoder->getHeight()*100)/_scale_factor);
          LOGDEBUG((_encoder->getWidth()*100)/_scale_factor);
          _encoder->open();
          _conv=Ptr<FrameConverter>(new FrameConverter(_decoder.get(), _encoder.get()));
        }
      }

      void PreviewPanel::preview(){
        if(_fis.get()==NULL || !_fis->isValid())return;
        
        Packet * packet;
        Frame * frame=NULL;
        int a=0;
        while ((packet = _pis->readPacket()) != NULL ) {
          if(packet->getStreamIndex()==_video_stream_index){
            frame=_decoder->decode2(*packet);
            if(frame->isFinished()&& _video_preview_frame<=a){
              break;
            }
            delete frame;
            frame=NULL;
            a++;
          }
        }
        if(frame&&!frame->isFinished()){
          delete frame;
          return;
        }

        _imageResource = Ptr<Wt::WMemoryResource>(new Wt::WMemoryResource("image/png"));
       
        Frame * f= new Frame(_encoder->getPixelFormat(), _encoder->getWidth(), _encoder->getHeight());
        _conv->convert(*frame, *f);
        _encoder->encode(*f);
        std::list<boost::shared_ptr<Packet> > packets=_sink->getList();
        boost::shared_ptr<Packet> picture=packets.back();
        delete f;
        _imageResource->setData((char*)picture->getData(), picture->getSize());
        
        image->setResource(_imageResource.get());
        image->resize((_encoder->getWidth()*_scale_factor)/100,(_encoder->getHeight()*_scale_factor)/100);
       // image->refresh();
        
//        layout()->removeItem(layout()->itemAt(0));
      }
    }
  }
}

