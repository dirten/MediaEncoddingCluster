

struct Frame{
    int length;
    unsigned char * data;
};

class HiveFrameReader{
    private:
	int _framePointer;
    public:
	HiveFrameReader(){
	
	}
	
	~HiveFrameReader(){
	
	}
	
	Frame * getNextFrame(){
	    return getFrame(++_framePointer);
	}
	
	Frame * getFrame(int number){
	    Frame * frame=new Frame();
	    frame->length=10;
	    frame->data=new unsigned char[10];
	    return frame;
	}
};


