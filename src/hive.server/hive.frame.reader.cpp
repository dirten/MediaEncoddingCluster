extern "C" {
#include <avformat.h>
}
#include "org/esb/io/File.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

using namespace org::esb::io;
using namespace org::esb::av;

class HiveFrameReader{
    private:
	int _frameNumber;

    public:
	HiveFrameReader(char * filename){
		File *file=new File(filename);
    	FormatInputStream *fis=new FormatInputStream(file);
    	PacketInputStream *pis=new PacketInputStream(fis->getStream(0));		
	}
	
	~HiveFrameReader(){
	
	}
	
	Frame * getNextFrame(){
	    return getFrame(++_frameNumber);
	}
	
	Frame * getFrame(int number){

	}
};


