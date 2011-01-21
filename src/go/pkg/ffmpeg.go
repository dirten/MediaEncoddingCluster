package ffmpeg


//#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
import "C"
import "unsafe"

import "strings"

func init(){
    C.avcodec_register_all();
    C.av_register_all();
    C.av_log_set_level(C.AV_LOG_VERBOSE);
}

//export OpenMediaFile
func OpenMediaFile(filename string){
    println("try openning the media file ")
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


type MediaLocator struct{
    Filename string;
    Protocol string;
    Reminder string;
    
}

func (loc * MediaLocator) GetProtocol() string{
    lines:=strings.Split(loc.Filename,":",-1)
    if(len(lines)!=2){
	return "file"
    }
    return lines[0]
}

func (loc * MediaLocator) GetReminder()string{
    lines:=strings.Split(loc.Filename,":",-1)
    if(len(lines)!=2){
	return loc.Filename
    }
    return lines[1][2:]
}

type DataSource struct{
    Locator MediaLocator;
    Ctx *C.AVFormatContext
    //Parameter * C.AVFormatParameters 
    Valid bool
}

func (src * DataSource) Connect() bool{
    src.Valid=false
    cfilename:=C.CString(src.Locator.Filename);
    
    src.Ctx=C.avformat_alloc_context();

    result:=C.av_open_input_file(&src.Ctx, cfilename, nil,0,nil);
    C.free(unsafe.Pointer(cfilename))

    if(result!=0){
	return src.Valid
    }
    result=C.av_find_stream_info(src.Ctx)
    if(result<0){
	return src.Valid
    }
    src.Valid=true
    return src.Valid;
}

func (src * DataSource) Disconnect() bool{
    if(src.Valid){
	C.av_close_input_file(src.Ctx);
    }
    return true;
}

func (src * DataSource) GetContentType() string{
    return "video/ffmpeg";
}

type Packet struct{
    Pts int
    Dts int
    Data [] byte
    Size int
    Stream int
    Flags int
    Duration int
    Pos int64
}

type Demultiplexer struct{
    Ds DataSource;
    Packet * C.AVPacket
}


func (dpx * Demultiplexer)GetDuration() Duration {
    return Duration{int64(dpx.Ds.Ctx.duration),Rational{1,1000000}}
}

func (dpx * Demultiplexer)GetTimestamp() Timestamp{
    return Timestamp{int64(dpx.Ds.Ctx.timestamp),Rational{1,1000000}}
}

func (dpx * Demultiplexer)ReadPacket(re * Packet) bool{
    dpx.Packet=new(C.AVPacket)
    C.av_init_packet(dpx.Packet)
    defer C.av_free_packet(dpx.Packet)
    if(C.av_read_frame(dpx.Ds.Ctx, dpx.Packet)>=0){
	C.av_dup_packet(dpx.Packet)
    }else{
	return false
    }
    re.Pts=int(dpx.Packet.pts)
    re.Dts=int(dpx.Packet.dts)
    re.Size=int(dpx.Packet.size)
    re.Data=make([]byte, re.Size)
    data:=(*(*[1<<30]byte)(unsafe.Pointer(dpx.Packet.data)))[0:dpx.Packet.size]
    for i:= 0; i < re.Size; i++ {
      re.Data[i] = data[i];
    }
    re.Stream=int(dpx.Packet.stream_index)
    re.Flags=int(dpx.Packet.flags)
    re.Duration=int(dpx.Packet.duration)
    re.Pos=int64(dpx.Packet.pos)
    return true
}


