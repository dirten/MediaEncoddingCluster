#include <iostream>
#include <AV.h>
using namespace std;
namespace org{
namespace esb{
namespace av{
class Global{
	public:
		static boost::mutex ffmpeg_global_mutex;
	};
/*
void __attribute__ ((constructor)) my_init(void);
//void _init() __attribute__((constructor));

void my_init(){
//    cout << "Init AV Package2"<<endl;
//    av_register_all();
}
*/
}}}


