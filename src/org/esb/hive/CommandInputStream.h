#ifndef ORG_ESB_HIVE_COMMANDINPUTSTREAM_H
#define ORG_ESB_HIVE_COMMANDINPUTSTREAM_H
#include "org/esb/io/InputStream.h"
#include "Command.h"
#include <vector>
using namespace org::esb::io;
namespace org{
namespace esb{
namespace hive{
class CommandInputStream : public InputStream{
    public:
        CommandInputStream(InputStream * is);
        ~CommandInputStream();
        int read(unsigned char * buffer,int length);
        int read(vector<unsigned char>&buffer);
        int available(bool isBlocking=false);
        Command * readCommand();
    private:
        InputStream * _source;
};

}}}
#endif

