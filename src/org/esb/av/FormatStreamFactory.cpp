#include "FormatStreamFactory.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
namespace org
{
  namespace esb
  {
    namespace av
    {
      map<std::string, org::esb::av::FormatInputStream*> FormatStreamFactory::_inputStreams;

      org::esb::av::FormatInputStream *
          FormatStreamFactory::getInputStream(std::string filename, long long int offset) {
                loginfo("FormatStreamFactory::getInputStream : " << filename);
        if (_inputStreams.find(filename) == _inputStreams.end()) {
                    loginfo("FormatStreamFactory::InputSTream exist : " << filename);
          org::esb::io::File file(filename);
          if (!file.exists()) {
            logerror("Could not find file : " << filename.c_str());
            _inputStreams[filename] = NULL;
          } else {
            org::esb::av::FormatInputStream *fis = new org::esb::av::FormatInputStream(&file);
            loginfo("find file : " << filename.c_str());
            if (offset > 0) {
              logdebug("seeking->" << offset);
              fis->seek(-1, offset);
            }
            _inputStreams[filename] = fis;
          }
        }
        return _inputStreams[filename];
      }

    }
  }
}

