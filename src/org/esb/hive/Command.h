#ifndef ORG_ESB_HIVE_COMMAND_H
#define ORG_ESB_HIVE_COMMAND_H

namespace org{
	namespace esb{
		namespace hive{
			class Command{
                public:
                    Command();
                    ~Command();
                    void setCommand(const char * com);
                    const char * getCommand();
                    void setData(const void * data);
                    const void * getData();
                private:
                    const char * _command;
                    const void * _data;
			};
		}
	}
}
#endif



