#ifndef ORG_ESB_HIVE_COMMAND_H
#define ORG_ESB_HIVE_COMMAND_H
#define TEXT_MODE 1
#define BINARY_MODE 2

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
                    bool getMode();
                private:
                    const char * _command;
                    const void * _data;
                    bool _mode;
			};
		}
	}
}
#endif



