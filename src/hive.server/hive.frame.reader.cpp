

struct Frame{
    int length;
    unsigned char * data;
};

class HiveFrameReader{
    private:
	int _frameNumber;
    public:
	HiveFrameReader(){
	
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


