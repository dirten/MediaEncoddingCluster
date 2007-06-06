extern "C" {
#include <avformat.h>
}
struct Frame{
    int length;
    unsigned char * data;
};

class HiveFrameReader{
    private:
	int _frameNumber;
	AVFormatContext *pFormatCtx;

    public:
	HiveFrameReader(char * filename){
	    if(av_open_input_file(&pFormatCtx, filename, NULL, 0, NULL)!=0){
		cout << "Konnte Datei " << filename << " nicht oeffnen" <<endl;
//		return -1; // Couldn't open file
	    }
	}
	
	~HiveFrameReader(){
	
	}
	
	Frame * getNextFrame(){
	    return getFrame(++_frameNumber);
	}
	
	Frame * getFrame(int number){
	    Frame * frame=new Frame();
	    frame->length=10;
	    frame->data=new unsigned char[10];
	    return frame;
	}
};


