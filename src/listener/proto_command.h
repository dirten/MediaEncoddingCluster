


#include <iostream>
using namespace std;
class ProtoCommand{
    public:
	virtual ~ProtoCommand(){};
	virtual bool isResponsible(char * command){return  false;};
	virtual void process(char * command){};
};

