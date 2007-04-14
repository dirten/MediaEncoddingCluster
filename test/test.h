#include <iostream>
using namespace std;
class Test{
    public:
	Test(char*);
	void echo(const char *);
	void read(istream&);
//	virtual ~Test();
//	friend ostream& operator << (ostream& os, const Test& te);
	Test* operator<<(const char *te);
	const char * operator>>(char * te);
	char* wert;
};
/*
ostream& operator<< (ostream& os, const Test& te){
    cout << "operator"<< endl;
    os << te.wert;
    return os;
}
*/
