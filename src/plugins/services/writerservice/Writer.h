#ifndef WRITER_H
#define WRITER_H

#include <string>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include <map>

namespace plugin {
  using org::esb::av::FormatOutputStream;
  using org::esb::av::PacketOutputStream;
  using org::esb::av::Packet;
  class Writer
  {
    public:
      Writer(db::OutputFile outputfile);
      Writer(std::string uuid, std::string filename, std::string data);
      ~Writer();
      void run();
    private:

      Ptr<FormatOutputStream> _fos;
      Ptr<PacketOutputStream> _pos;
      std::map<int, boost::shared_ptr<org::esb::av::Encoder> > encoder_map;
      db::OutputFile _outputfile;
      static bool ptsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b);
      static bool dtsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b);

      map<int,int64_t> _stream_timestamps;
  };
}
#endif // WRITER_H
