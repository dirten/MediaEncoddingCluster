//#include <iostream>
#include <string>
#include "test.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


class Byte {
    private:
	uint8_t _byte;
    public:
	~Byte(){}
	Byte(){}
	
	Byte(char byte){
	    _byte=static_cast<uint8_t>(byte);
	}
	Byte(int byte){
	    _byte=static_cast<uint8_t>(byte);
	}
	
	uint8_t byteValue(){
	    return _byte;
	}
	short shortValue()const {
	    return _byte;
	}
	long longValue(){
	    return _byte;
	}
	float floatValue(){
	    return _byte;
	}
	double doubleValue(){
	    return _byte;
	}
	
    


};

int testSize(const vector<Byte>&bytes){
    cout << "Size:"<<(bytes[0].shortValue())<<endl;
}

int main(int argc, char**argv){




	    fstream FileBin("frame10.ppm",ios::in|ios::out|ios::binary);
	    FileBin.seekg(0,ios::end);
	    unsigned long filesize=streamoff(FileBin.tellg());
	    FileBin.seekg(0,ios::beg);
	    string strBuffer="";
	    char * buffer=new char [filesize];
	    FileBin.read(buffer, filesize);
	    string bla(buffer, filesize);
	    cout <<bla.size()<<"Buffer:"<< bla << endl;

//    cout.precision(8);
//    cout << 000.000 <<endl;
    vector<Byte>hier(10, Byte('a'));
    cout << "testSize:"<<sizeof(hier)<<endl;
    cout << "testSize:"<<hier[0].doubleValue()<<endl;
    testSize(hier);

}
