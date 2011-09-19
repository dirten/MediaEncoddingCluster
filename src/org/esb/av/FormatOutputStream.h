#ifndef ORG_ESB_AV_FORMATOUTPUTSTREAM_H
#define ORG_ESB_AV_FORMATOUTPUTSTREAM_H
#include "FormatBaseStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Encoder.h"
#include "exports.h"
//#include "AV.h"
struct AVFormatContext;
struct AVOutputFormat;
namespace org {
    namespace esb {
        namespace av {
          typedef std::list<Ptr<AVOutputFormat> > OutputFormatList;
            class AV_EXPORT FormatOutputStream : public FormatBaseStream, public org::esb::io::OutputStream {
              classlogger("org.esb.av.FormatOutputStream");
            public:
                FormatOutputStream(org::esb::io::File * target, const char * fmt = NULL);
                ~FormatOutputStream();
                void write(char * buffer, int length);
                void write(vector<unsigned char>&buffer);
                void write(char * buffer);
                void open();
                bool close();
                void addPacketStream(PacketOutputStream & stream, Encoder & encoder);
                void dumpFormat();
                static OutputFormatList getOutputFormats();
                //		private:
                friend class PacketOutputStream;
                AVFormatContext * _fmtCtx;
                AVOutputFormat * _fmt;
                org::esb::io::File * _file;
            private:
                enum STATUS{
                    OPENED,
                    CLOSED
                };
                int _status;
            };
        }
    }
};
#endif
