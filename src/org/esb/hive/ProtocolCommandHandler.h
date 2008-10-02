#ifndef ORG_ESB_HIVE_PROTOCOLCOMMAND_H
#define ORG_ESB_HIVE_PROTOCOLCOMMAND_H
#include "org/esb/net/Socket.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#define CMD_NA 0
#define CMD_PROCESS 1
#define CMD_HELP 2

//#define CMD_GET_UNIT  10
//#define CMD_PUT_UNIT  11


enum cmdId {
	CMD_NONE,
	CMD_GET_UNIT,
	CMD_PUT_UNIT,
	CMD_HELP,
};
namespace org{
    namespace esb{
        namespace hive{
            class ProtocolCommandHandler{
                public:
            	    ProtocolCommandHandler(){};
            	    ProtocolCommandHandler(InputStream *is, OutputStream * os){};
/**
 * @deprecated
 */
//             	    ProtocolCommandHandler(org::esb::net::Socket * socket){};
            	    virtual ~ProtocolCommand(){};
            	    virtual int isResponsible(char * command)=0;
            	    virtual int isResponsible(cmdId&)=0;
            	    virtual void process(char * command)=0;
            	    virtual void printHelp(){};
            	protected:
            	    org::esb::net::Socket * socket;
            	    org::esb::io::InputStream * is;
            	    org::esb::io::OutputStream * os;
            };
        }
    }
}
#endif


