#ifndef STREAMSOURCE_H
#define STREAMSOURCE_H
#include "org/esb/core/Task.h"
#include "org/esb/av/FormatInputStream.h"

using org::esb::av::FormatInputStream;
namespace plugin {

  class StreamSource: public org::esb::core::Task
  {
  public:
    StreamSource();
    void prepare();
    void execute();
    int getPadTypes();
    org::esb::core::OptionsDescription getOptionsDescription();
    void setSource(FormatInputStream * fis);
  private:
    FormatInputStream * _fis;
    std::map<int, Ptr<org::esb::av::Decoder> >_decs;
  };
}
#endif // STREAMSOURCE_H
