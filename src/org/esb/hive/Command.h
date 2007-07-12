#ifndef ORG_ESB_HIVE_COMMAND_H
#define ORG_ESB_HIVE_COMMAND_H
#define TEXT_MODE 1
#define BINARY_MODE 2
#include <string>
using namespace std;
namespace org
{
	namespace esb
	{
		namespace hive
		{
			class Command
			{
			      public:
				Command ();
				~Command ();
				void setCommand (const char *com);
				void setCommand (string & command);
				  string & getCommand ();
				void setData (const void *data);
				const void *getData ();
				void setDataLength (int datalength);
				int getDataLength ();
				bool getMode ();
			      private:
				  string _command;
				const void *_data;
				int _datalength;
				bool _mode;
			};
		}
	}
}
#endif
