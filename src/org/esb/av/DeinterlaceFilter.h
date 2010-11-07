#ifndef ORG_ESB_AV_DEINTERLACEFILTER_H
#define ORG_ESB_AV_DEINTERLACEFILTER_H
#include "PlugIn.h"
#include "Frame.h"
#include "Format.h"
#include "AV.h"
namespace org {
  namespace esb {
    namespace av {

      class DeinterlaceFilter : public PlugIn {
      public:
        DeinterlaceFilter(Format in, Format out);
        void close();
        std::string getName();
        void open();
        void reset();
        void setParameter(std::map<std::string, std::string>);
        ProcessStatus process(Frame & in, Frame & out);
      private:
        Format _in_format;
        Format _out_format;
      };
    }
  }
}
#endif