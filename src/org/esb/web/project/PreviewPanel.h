#ifndef ORG_ESB_WEB_PREVIEWPANEL
#define ORG_ESB_WEB_PREVIEWPANEL

#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/Log.h"


#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/FrameConverter.h"
#include "Wt/Ext/Panel"
#include "Wt/Ext/Button"
#include "Wt/Ext/ComboBox"

#include "Wt/WMemoryResource"
#include "Wt/WSignalMapper"
#include "Wt/WImage"
namespace org{
  namespace esb{
    namespace web{
      class PacketSink;
      class PreviewPanel: public Wt::Ext::Panel{
        classlogger("org.esb.web.PreviewPanel");
      public:
        PreviewPanel();
        virtual ~PreviewPanel();
        void setProject(Ptr<db::Project>p);
        void setMediaFile(Ptr<db::MediaFile> mediafile);
        void setFilterList(std::list<Ptr<db::Filter> >filters);

        void preview();
      private:
        Ptr<org::esb::av::FormatInputStream> _fis;
        Ptr<org::esb::av::PacketInputStream> _pis;
        Ptr<org::esb::av::Decoder> _decoder;
        Ptr<org::esb::av::Encoder> _encoder;
        Ptr<org::esb::av::FrameConverter> _conv;
        Ptr<db::Project> _project;
        Ptr<PacketSink> _sink;
        Ptr<db::MediaFile> mediafile;
        Ptr<Wt::WMemoryResource> _imageResource;
        Wt::WSignalMapper<Wt::Ext::Component*> *_buttonSignalMap;
        Wt::Ext::ComboBox *_scale_combo;
        void controllerButtonClicked(Wt::Ext::Component * btn);
        int _video_stream_index;
        int _video_preview_frame;
        int _scale_factor;
        Wt::WImage *image;
      };
    }
  }
}

#endif

