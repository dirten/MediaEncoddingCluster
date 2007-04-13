#ifndef FRAMECOMPRESSOR_H
#define  FRAMECOMPRESSOR_H

#include "minilzo.h"
#include <stdio.h>
#include <avformat.h>
#include <iostream>
#include <cstdlib>

#if defined(__LZO_STRICT_16BIT)
#define IN_LEN      (8*1024u)
#elif defined(LZO_ARCH_I086) && !defined(LZO_HAVE_MM_HUGE_ARRAY)
#define IN_LEN      (60*1024u)
#else
#define IN_LEN      (128*1024ul)
#endif
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

using namespace std;
class FrameCompressor{
    private:
	static const char in;
	static const char out;
	
    public:
	unsigned char inflate(unsigned char in);
	unsigned char deflate(unsigned char in);
	int deflateFrame(AVFrame *in, unsigned char * out);
	FrameCompressor();    
	~FrameCompressor();    
};
#endif
