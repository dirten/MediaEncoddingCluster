#ifndef ORG_ESB_HIVE_PROTOCOLCOMMAND_H
#define ORG_ESB_HIVE_PROTOCOLCOMMAND_H
#define CMD_NA 0
#define CMD_PROCESS 1
#define CMD_HELP 2
namespace org{
    namespace esb{
        namespace hive{
            class ProtocolCommand{
                public:
            	    virtual ~ProtocolCommand(){};
            	    virtual int isResponsible(char * command)=0;
            	    virtual void process(char * command)=0;
            	    virtual void printHelp(){};
            };
        }
    }
}
#endif


