#include "FrameCompressor.h"
#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);

/*****************************************************************/
FrameCompressor::FrameCompressor(){
    if (lzo_init() != LZO_E_OK)
    {
        printf("internal error - lzo_init() failed !!!\n");
        printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable `-DLZO_DEBUG' for diagnostics)\n");
//        return 3;
    }else{
        printf("FrameCompressor started !!!\n");    
    }
}

/*****************************************************************/
FrameCompressor::~FrameCompressor(){


}

/*****************************************************************/
unsigned char FrameCompressor::inflate(unsigned char in){


}

/*****************************************************************/
unsigned char FrameCompressor::deflate(unsigned char in){


}

int FrameCompressor::deflateFrame(AVFrame * in, unsigned char * out){
    int height=256;
    int width=512;
    int bufSize=width*height*3;    
    unsigned char * buffer = new unsigned char[bufSize];
    for(int y=0; y<height; y++){
      memcpy(buffer+(in->linesize[0]*y),in->data[0]+(y*in->linesize[0]),width*3);
    }
    lzo_uint in_len=bufSize;
    lzo_uint out_len;
    lzo_uint new_len;
//    out=buffer;

    int r = lzo1x_1_compress(buffer,in_len,out,&out_len,wrkmem);
//    delete buffer;
    return out_len;
//    printf("EingabeGröße:%d\t AusgabeGröße:%d\n",in_len, out_len);
}

