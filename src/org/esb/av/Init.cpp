#include <iostream>
#include <avformat.h>
using namespace std;
namespace org{
namespace esb{
namespace av{

void __attribute__ ((constructor)) my_init(void);
//void _init() __attribute__((constructor));

void my_init(){
    cout << "Init AV Package"<<endl;
    av_register_all();
}
}}}