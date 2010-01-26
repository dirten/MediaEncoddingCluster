#include "PPSWriter.h"
#include "org/esb/av/Frame.h"
#include "org/esb/io/FileOutputStream.h"

using namespace org::esb::av;
using namespace org::esb::io;
PPSWriter::PPSWriter(const char * fileNamePattern){
    _filePattern=fileNamePattern;
}


void PPSWriter::writeFrame(Frame * frame){
    char filename[32];
    sprintf(filename, _filePattern,_fileCounter);
    FileOutputStream fos(filename);
    char header[200];
    sprintf(header, "P6\n%d %d\n255\n", frame->getWidth(), frame->getHeight());
    fos.write(header, strlen(header));
    for(int i=0;i<frame->getHeight();i++)
      fos.write((char*)frame->getData() + i * frame->getWidth(), frame->getWidth());

}
