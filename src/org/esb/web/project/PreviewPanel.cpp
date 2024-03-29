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
#include "org/esb/av/Format.h"
#include "org/esb/av/ResizeFilter.h"
#include "org/esb/av/DeinterlaceFilter.h"
#include "org/esb/lang/Thread.h"
namespace org {
  namespace esb {
    namespace web {
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

      PreviewPanel::PreviewPanel() : Wt::Ext::Panel() {
        LOGDEBUG("PreviewPanel::PreviewPanel()" << this);
        setTitle("Preview");
        setLayout(new Wt::WDefaultLayout());
        setAutoScrollBars(true);

        _video_stream_index = 0;
        _video_preview_frame = 0;
        _scale_factor = 75;

        _buttonSignalMap = new Wt::WSignalMapper<Wt::Ext::Component*>(this);
        _buttonSignalMap->mapped().connect(SLOT(this, PreviewPanel::controllerButtonClicked));

        Wt::Ext::ToolBar *toolbar = new Wt::Ext::ToolBar();
        setTopToolBar(toolbar);



        //toolbar->add(new Wt::Ext::Button("test"));
        _buttons["prev10"] = Ptr<Wt::Ext::Button > (new Wt::Ext::Button(""));
        _buttons["prev"] = Ptr<Wt::Ext::Button > (new Wt::Ext::Button(""));
        _buttons["next"] = Ptr<Wt::Ext::Button > (new Wt::Ext::Button(""));
        _buttons["next10"] = Ptr<Wt::Ext::Button > (new Wt::Ext::Button(""));

        toolbar->add(_buttons["prev10"].get());
        toolbar->add(_buttons["prev"].get());
        toolbar->add(_buttons["next"].get());
        toolbar->add(_buttons["next10"].get());

        _buttons["next"]->setIcon("icons/preview-next.png");
        _buttons["prev"]->setIcon("icons/preview-prev.png");
        _buttons["next10"]->setIcon("icons/preview-next-10.png");
        _buttons["prev10"]->setIcon("icons/preview-prev-10.png");

        _scale_combo = new Wt::Ext::ComboBox();
        _scale_combo->addItem("150%");
        _scale_combo->addItem("125%");
        _scale_combo->addItem("100%");
        _scale_combo->addItem("75%");
        _scale_combo->addItem("50%");
        _scale_combo->addItem("25%");
        _scale_combo->setCurrentIndex(3);
        toolbar->add(_scale_combo);

        _buttonSignalMap->mapConnect(_scale_combo->activated(), _scale_combo);
        _buttonSignalMap->mapConnect(_scale_combo->keyWentUp(), _scale_combo);
        _scale_combo->setObjectName("preview_scale");

        _buttons["prev10"]->setObjectName("prev10");
        _buttons["prev"]->setObjectName("prev");
        _buttons["next10"]->setObjectName("next10");
        _buttons["next"]->setObjectName("next");
        /*
        _buttons["prev10"]->clicked.setBlocked(true);
        _buttons["prev"]->clicked.setBlocked(true);
        _buttons["next"]->clicked.setBlocked(true);
        _buttons["next10"]->clicked.setBlocked(true);
         */
        //        addVideoButton->clicked.connect(SLOT(this, InputFilePanel::addVideoButtonClicked));



        _buttonSignalMap->mapConnect(_buttons["prev10"]->clicked(), _buttons["prev10"].get());
        _buttonSignalMap->mapConnect(_buttons["prev"]->clicked(), _buttons["prev"].get());
        _buttonSignalMap->mapConnect(_buttons["next"]->clicked(), _buttons["next"].get());
        _buttonSignalMap->mapConnect(_buttons["next10"]->clicked(), _buttons["next10"].get());

        image = new Wt::WImage();
        image->setAlternateText("preview image");
        image->imageLoaded().connect(SLOT(this, PreviewPanel::imageLoaded));
        layout()->addWidget(image);
        _frameserver = Ptr<PreviewFrameServer > (new PreviewFrameServer());


      }

      void PreviewPanel::imageLoaded() {

        LOGDEBUG("image loaded")
        std::map<std::string, Ptr<Wt::Ext::Button> >::iterator it = _buttons.begin();
        for (; it != _buttons.end(); it++) {
          (*it).second->setEnabled(true);
        }
      }

      void PreviewPanel::controllerButtonClicked(Wt::Ext::Component * btn) {

        std::map<std::string, Ptr<Wt::Ext::Button> >::iterator it = _buttons.begin();
        for (; it != _buttons.end(); it++) {
          (*it).second->setEnabled(false);
        }
        if (btn->objectName() == "prev10") {
          _frameserver->jumpPrevious(PreviewFrameServer::JUMP_TEN);
        } else if (btn->objectName() == "prev") {
          _frameserver->jumpPrevious(PreviewFrameServer::JUMP_ONE);
        } else if (btn->objectName() == "next") {
          _frameserver->jumpForward(PreviewFrameServer::JUMP_ONE);
        } else if (btn->objectName() == "next10") {
          _frameserver->jumpForward(PreviewFrameServer::JUMP_TEN);
        } else if (btn->objectName() == "preview_scale") {
          std::string value = ((Wt::Ext::ComboBox*)_scale_combo)->currentText().narrow();

          LOGDEBUG("scale factor" << value);

          int factor = atoi(value.c_str());
          LOGDEBUG("scale factor" << factor);
          if (factor > 0)
            _scale_factor = factor;
        }
        preview();
        LOGDEBUG(btn->objectName());

      }

      PreviewPanel::~PreviewPanel() {
        LOGDEBUG("PreviewPanel::~PreviewPanel()" << this)
                //Wt::Ext::Panel::~Panel();
      }

      void PreviewPanel::setProject(Ptr<db::Project>p) {
        _project = p;
        if (_project->mediafiles().get().count() > 0) {
          db::MediaFile file = _project->mediafiles().get().one();
          std::string filename = file.path + "/" + file.filename;
          _frameserver->setFileName(filename);
        }
        filterChanged();
      }

      void PreviewPanel::filterChanged() {
        if (!_frameserver->isOpen())return;

        Ptr<org::esb::av::Decoder>dec = _frameserver->getDecoder();


        _encoder = Ptr<Encoder > (new Encoder(CODEC_ID_PNG));

        std::vector<db::Filter> filters = _project->filter().get().all();
        std::vector<db::Filter>::iterator it = filters.begin();
        int width = dec->getWidth();
        int height = dec->getHeight();
        bool deinterlace = false;
        for (; it != filters.end(); it++) {
//          LOGDEBUG((*it))
          if ((*it).filterid == "resize") {
            LOGDEBUG("found resize filter!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
            std::vector<db::FilterParameter> params = (*it).parameter().get().all();
            std::vector<db::FilterParameter>::iterator it2 = params.begin();
            for (; it2 != params.end(); it2++) {
               LOGDEBUG((*it2))
              if((*it2).fkey=="width"){
                int tmp=atoi(((std::string)(*it2).fval).c_str());
                if(tmp>0)
                  width=tmp;
              }
              if((*it2).fkey=="height"){
                int tmp=atoi(((std::string)(*it2).fval).c_str());;
                if(tmp>0)
                  height=tmp;
              }
            }
          } else if ((*it).filterid == "deinterlace") {
            deinterlace = true;
          }
        }

        _encoder->setPixelFormat(PIX_FMT_RGB24);

        _encoder->setBitRate(512000);
        _encoder->setGopSize(10);
        _sink = Ptr<PacketSink > (new PacketSink());
        _encoder->setSink(_sink.get());
        _encoder->setFrameRate(dec->getFrameRate());
        _encoder->setTimeBase(dec->getTimeBase());
        _encoder->setWidth(width);
        _encoder->setHeight(height);
        _encoder->open();
        LOGDEBUG(_encoder->toString());

        Format in=dec->getOutputFormat();
        Format out=_encoder->getInputFormat();
        _plugin=new org::esb::av::ResizeFilter(in, out);
        //_plugin->open();
        _plugin_chain.clear();
        if(deinterlace){
          _plugin_chain.push_back(new org::esb::av::DeinterlaceFilter(in, out));
        }
        _plugin_chain.push_back(new org::esb::av::ResizeFilter(in, out));

        std::list<Ptr<org::esb::av::PlugIn> >::iterator plugin_it=_plugin_chain.begin();
        for(;plugin_it!=_plugin_chain.end();plugin_it++){
          (*plugin_it)->open();
        }
        
        preview();
      }

      void PreviewPanel::preview() {
        LOGDEBUG("PreviewPanel::preview");
        if (!_frameserver->isOpen())return;

        Ptr<org::esb::av::Frame> frame = _frameserver->getFrame();
        if (!frame->isFinished())return;


        std::list<Ptr<org::esb::av::PlugIn> >::iterator plugin_it=_plugin_chain.begin();
        for(;plugin_it!=_plugin_chain.end();plugin_it++){
          Ptr<Frame> f = new Frame(PIX_FMT_YUV444P, _encoder->getWidth(), _encoder->getHeight());
          (*plugin_it)->process(*frame, *f);
          frame=f;
        }

        
        _encoder->encode(*frame);
        std::list<boost::shared_ptr<Packet> > packets = _sink->getList();
        boost::shared_ptr<Packet> picture = packets.back();
        //delete f;
        if (picture->getSize() > 0) {
          _imageResource = Ptr<Wt::WMemoryResource > (new Wt::WMemoryResource("image/png"));
          _imageResource->setData( picture->getData(), picture->getSize());
          image->setResource(_imageResource.get());
          image->resize((_encoder->getWidth() * _scale_factor) / 100, (_encoder->getHeight() * _scale_factor) / 100);
        }
        // image->refresh();

        //        layout()->removeItem(layout()->itemAt(0));
      }
    }
  }
}

