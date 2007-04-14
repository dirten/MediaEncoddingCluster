//#include <iostream.h>
#include "test.h"
//using namespace std;
//friend ostream& operator << (ostream& os, const Test& te);

Test::Test(char* zahl){
    wert=zahl;
};
void Test::echo(const char *bla){
    cout << bla <<endl;
}
void Test::read(istream &bla){
    bla>>wert;
}
/*
ostream& operator<< (ostream& os, const Test& te){
    os << te.wert;
    return os;
}
*/
Test* Test::operator<< (const char *te){
    cout << "links operator" <<endl;
//    os.echo(te);
    return this;
}

const char * Test::operator>> (char* te){
    cout << te << " rechts operator" <<this->wert<< endl;
//    te=os.wert;
    return this->wert;
//    os.wert=(char*)te;
}

/*
ostream& operator<< (ostream& os, const Test& te){
    cout << "operator"<< endl;
    os << te.wert;
    return os;
}
*/
/*int main(int argc, char**argv){
    Test *t=new Test("11223");
    cout << *t <<endl;
//    delete t;
*/

