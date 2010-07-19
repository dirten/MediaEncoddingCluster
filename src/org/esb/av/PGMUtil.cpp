#include "PGMUtil.h"
#include "Frame.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/Log.h"

namespace org {
  namespace esb {
    namespace av {

      Frame * PGMUtil::load(std::string file) {
        org::esb::io::FileInputStream fis(file.c_str());
        std::string data;
        int data_size=fis.read(data);
        char line[1000];
        int offset=0;
        bool read_header=true;
        while(read_header){
          int e = sscanf(data.c_str()+offset, "%999[^\n]\n", line) - 1;
          offset+=strlen(line)+1;
          LOGDEBUG("PGMLine:"<<line<<"e="<<e<<"len="<<strlen(line)<<"offset="<<offset);
        }
	return NULL;
      }

      void PGMUtil::save(std::string file, Frame * frame) {
        org::esb::io::FileOutputStream fos(file.c_str());
        char header[200];
        sprintf(header, "P5\n%d %d\n255\n", frame->getWidth(), frame->getHeight());
        fos.write(header, strlen(header));
        for (int i = 0; i < frame->getHeight(); i++)
          fos.write((char*) frame->getData() + i * frame->getAVFrame()->linesize[0], frame->getWidth());
      }
    }
  }
}
