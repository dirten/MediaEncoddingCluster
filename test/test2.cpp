//#include <iostream>
#include <string>
#include "test.h"


class bla{
    public:
     string test(){
	string s ;//= new string();
	
	return s;
    }

};
int main(int argc, char**argv){
    Test * t = new Test("1232323");
    char *test="blaf";
    char *bla;
    int l=0;    
//    *t << test;
//    *t >> test;
    cin>>bla;    
    cout << bla << endl;
    delete t;
}
