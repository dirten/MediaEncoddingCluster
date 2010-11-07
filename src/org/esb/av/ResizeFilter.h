#ifndef ORG_ESB_AV_RESIZEFILTER_H
#define ORG_ESB_AV_RESIZEFILTER_H
#include "PlugIn.h"
#include "Frame.h"
#include "Format.h"
#include "AV.h"
#include "org/esb/util/Log.h"
namespace org{
  namespace esb{
    namespace av{
      class ResizeFilter: public PlugIn{
        classlogger("org.esb.av.ResizeFilter");
      public:
        ResizeFilter(Format in, Format out);
        void close();
        std::string getName();
        void open();
        void reset();
        void setParameter(std::map<std::string, std::string>);
        ProcessStatus process(Frame & in, Frame & out);
      private:
        SwsContext * _swsContext;
        Format _in_format;
        Format _out_format;
      };
    }
  }
}
#endif
