#ifndef WRITER_H
#define WRITER_H

#include <string>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/lang/Ptr.h"
#include <map>

namespace plugin {
  using org::esb::av::FormatOutputStream;
  using org::esb::av::PacketOutputStream;
  using org::esb::av::Packet;
  class Writer : public org::esb::core::ServicePlugin
  {
  public:
    Writer();
    Writer(db::OutputFile outputfile);
    Writer(std::string uuid, std::string filename, std::string data);
    ~Writer();
    void run();
    void setOutputFileById(std::string id);
    void setOutputFile(db::OutputFile outfile);
    void startService();
    void stopService();
    org::esb::core::OptionsDescription getOptionsDescription();
    org::esb::core::ServicePlugin::ServiceType getServiceType();

  private:

    Ptr<FormatOutputStream> _fos;
    Ptr<PacketOutputStream> _pos;
    std::map<int, boost::shared_ptr<org::esb::av::Encoder> > encoder_map;
    Ptr<db::OutputFile> _outputfile;
    //std::string _jobid;
    static bool ptsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b);
    static bool dtsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b);

    map<int,int64_t> _stream_timestamps;
    list< boost::shared_ptr<org::esb::av::Encoder> > _encoderList;
  };

  }
#endif // WRITER_H
