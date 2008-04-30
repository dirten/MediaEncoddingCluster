#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"




using namespace org::esb::av;
int main(int argc, char ** argv){

	if(argc!=2)return 1;
	File file(argv[1]);
	FormatInputStream fis(&file);
	fis.dumpFormat();
	
	int streams=fis.getStreamCount();
	for (int a=0;a<streams;a++){
		AVStream * stream=fis.getFormatContext()->streams[a];
		cout << "TimeBaseStream #"<<a<<"\tnum:"<<stream->time_base.num<<"\tden"<<stream->time_base.den<<endl;
		cout << "TimeBaseCodec  #"<<a<<"\tnum:"<<stream->codec->time_base.num<<"\tden"<<stream->codec->time_base.den<<endl;
	
	}
	
	
	
return 0;
}