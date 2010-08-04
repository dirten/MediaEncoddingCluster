#ifndef ORG_ESB_WEB_PREVIEWPANEL
#define ORG_ESB_WEB_PREVIEWPANEL

#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"


#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "Wt/Ext/Panel"

namespace org{
  namespace esb{
    namespace web{
      class PacketSink;
      class PreviewPanel: public Wt::Ext::Panel{
      public:
        PreviewPanel();
        ~PreviewPanel();
        void setProject(Ptr<db::Project>p);
        void preview();
      private:
        Ptr<org::esb::av::FormatInputStream> _fis;
        Ptr<org::esb::av::PacketInputStream> _pis;
        Ptr<org::esb::av::Decoder> _decoder;
        Ptr<org::esb::av::Encoder> _encoder;
        Ptr<db::Project> _project;
        Ptr<PacketSink> _sink;
        int _video_stream_index;
      };
    }
  }
}

#endif

