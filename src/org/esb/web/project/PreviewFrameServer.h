#ifndef ORG_ESB_WEB_PREVIEWFRAMESERVER
#define ORG_ESB_WEB_PREVIEWFRAMESERVER
#include <string>
#include "org/esb/av/Frame.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/lang/Ptr.h"
namespace org{
  namespace esb{
    namespace web{
      class PreviewFrameServer{
      public:
        enum JUMP_WIDTH{
          JUMP_NONE,
          JUMP_ONE,
          JUMP_TEN=10
        };
        PreviewFrameServer();
        void setFileName(std::string filename);
        Ptr<org::esb::av::Frame> getFrame();
        Ptr<org::esb::av::Decoder> getDecoder();
        void jumpForward(JUMP_WIDTH);
        void jumpPrevious(JUMP_WIDTH);
        bool isOpen();
      private:
        Ptr<org::esb::av::FormatInputStream> _fis;
        Ptr<org::esb::av::PacketInputStream> _pis;
        Ptr<org::esb::av::Decoder> _decoder;
        Ptr<org::esb::av::Frame> _frame;
        Ptr<org::esb::av::Packet> _last_packet;
        Ptr<org::esb::av::StreamInfo> _stream_info;

      };
    }
  }
}



#endif
