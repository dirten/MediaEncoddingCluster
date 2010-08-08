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
#include "org/esb/lang/Thread.h"
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
        _buttons["prev10"]=Ptr<Wt::Ext::Button>(new Wt::Ext::Button("prev 10"));
        _buttons["prev"]=Ptr<Wt::Ext::Button>(new Wt::Ext::Button("prev"));
        _buttons["next"]=Ptr<Wt::Ext::Button>(new Wt::Ext::Button("next"));
        _buttons["next10"]=Ptr<Wt::Ext::Button>(new Wt::Ext::Button("next 10"));

        toolbar->add(_buttons["prev10"].get());
        toolbar->add(_buttons["prev"].get());
        toolbar->add(_buttons["next"].get());
        toolbar->add(_buttons["next10"].get());



        _scale_combo=new Wt::Ext::ComboBox();
        _scale_combo->addItem("150%");
        _scale_combo->addItem("125%");
        _scale_combo->addItem("100%");
        _scale_combo->addItem("75%");
        _scale_combo->addItem("50%");
        _scale_combo->addItem("25%");
        _scale_combo->setCurrentIndex(3);
        toolbar->add(_scale_combo);

        _buttonSignalMap->mapConnect(_scale_combo->activated,_scale_combo);
        _buttonSignalMap->mapConnect(_scale_combo->keyWentUp,_scale_combo);
        _scale_combo->setObjectName("preview_scale");

        _buttons["prev10"]->setObjectName("prev10");
        _buttons["prev"]->setObjectName("prev");
        _buttons["next10"]->setObjectName("next10");
        _buttons["next"]->setObjectName("next");
        _buttons["prev10"]->clicked.setBlocked(true);
        _buttons["prev"]->clicked.setBlocked(true);
        _buttons["next"]->clicked.setBlocked(true);
        _buttons["next10"]->clicked.setBlocked(true);

        //        addVideoButton->clicked.connect(SLOT(this, InputFilePanel::addVideoButtonClicked));



        _buttonSignalMap->mapConnect(_buttons["prev10"]->clicked,_buttons["prev10"].get());
        _buttonSignalMap->mapConnect(_buttons["prev"]->clicked,_buttons["prev"].get());
        _buttonSignalMap->mapConnect(_buttons["next"]->clicked,_buttons["next"].get());
        _buttonSignalMap->mapConnect(_buttons["next10"]->clicked,_buttons["next10"].get());

        image = new Wt::WImage();
        image->setAlternateText("preview image");
        image->loaded.connect(SLOT(this,PreviewPanel::imageLoaded));      
        layout()->addWidget(image);
        _frameserver=Ptr<PreviewFrameServer>(new PreviewFrameServer());


      }
      void PreviewPanel::imageLoaded(){

        LOGDEBUG("image loaded")
          std::map<std::string,Ptr<Wt::Ext::Button> >::iterator it=_buttons.begin();
        for(;it!=_buttons.end();it++){
          (*it).second->setEnabled(true);
        }
      }
      void PreviewPanel::controllerButtonClicked(Wt::Ext::Component * btn){

        std::map<std::string,Ptr<Wt::Ext::Button> >::iterator it=_buttons.begin();
        for(;it!=_buttons.end();it++){
          (*it).second->setEnabled(false);
        }
        org::esb::lang::Thread::sleep2(200);
        if(btn->objectName()=="prev10"){
          if(_video_preview_frame>=10){
            _frameserver->jumpPrevious(PreviewFrameServer::JUMP_TEN);
          }
        }else if(btn->objectName()=="prev"){
          if(_video_preview_frame>0){
            _frameserver->jumpPrevious(PreviewFrameServer::JUMP_ONE);
          }
        }else if(btn->objectName()=="next"){
          _frameserver->jumpForward(PreviewFrameServer::JUMP_ONE);
        }else if(btn->objectName()=="next10"){
          _frameserver->jumpForward(PreviewFrameServer::JUMP_TEN);
        }else if(btn->objectName()=="preview_scale"){
          std::string value=((Wt::Ext::ComboBox*)_scale_combo)->currentText().narrow();

          LOGDEBUG("scale factor"<<value);

          int factor=atoi(value.c_str());
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
          _frameserver->setFileName(filename);

          _encoder=Ptr<Encoder>(new Encoder(CODEC_ID_PNG));
          _encoder->setPixelFormat(PIX_FMT_RGB24);

          _encoder->setBitRate(512000);
          _encoder->setGopSize(10);
          _sink=Ptr<PacketSink>(new PacketSink());
          _encoder->setSink(_sink.get());
          Ptr<org::esb::av::Decoder>dec=_frameserver->getDecoder();
          _encoder->setFrameRate(dec->getFrameRate());
          _encoder->setTimeBase(dec->getTimeBase());
          _encoder->setWidth(dec->getWidth());
          _encoder->setHeight(dec->getHeight());
          _encoder->open();
          _conv=Ptr<FrameConverter>(new FrameConverter(_frameserver->getDecoder().get(), _encoder.get()));
        }
      }

      void PreviewPanel::preview(){
        LOGDEBUG("PreviewPanel::preview");
        if(!_frameserver->isOpen())return;

        Ptr<org::esb::av::Frame> frame=_frameserver->getFrame();
        if(!frame->isFinished())return;

        Frame * f= new Frame(_encoder->getPixelFormat(), _encoder->getWidth(), _encoder->getHeight());
        _conv->convert(*frame, *f);
        _encoder->encode(*f);
        std::list<boost::shared_ptr<Packet> > packets=_sink->getList();
        boost::shared_ptr<Packet> picture=packets.back();
        delete f;
        if(picture->getSize()>0){
          _imageResource = Ptr<Wt::WMemoryResource>(new Wt::WMemoryResource("image/png"));
          _imageResource->setData((char*)picture->getData(), picture->getSize());
          image->setResource(_imageResource.get());
          image->resize((_encoder->getWidth()*_scale_factor)/100,(_encoder->getHeight()*_scale_factor)/100);
        }
        // image->refresh();

        //        layout()->removeItem(layout()->itemAt(0));
      }
    }
  }
}

