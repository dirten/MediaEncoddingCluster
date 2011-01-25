package ffmpeg


//#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
import "C"
import "unsafe"
//export OpenMediaFile
func OpenMediaFile(filename string){
    println("try openning the media file")
    println(filename)
    cfilename:=C.CString(filename);
    
    C.avcodec_register_all();
    C.av_register_all();
    ctx:=C.avformat_alloc_context();

    result:=C.av_open_input_file(&ctx, cfilename, nil,0,nil);
    println(result)
    result=C.av_find_stream_info(ctx)
     C.free(unsafe.Pointer(cfilename));
     C.av_close_input_file(ctx);
     //C.free(unsafe.Pointer(ctx));
}

  