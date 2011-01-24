package ffmpeg

//#include "libavcodec/avcodec.h"
import "C"
//import "unsafe"
type Frame C.AVFrame

type Coder struct {
    Parameter map[string]string
    Ctx *C.AVCodecContext
    Codec *C.AVCodec
    Valid bool
}

func NewCoderFromContext(ctx *C.AVCodecContext)Coder{
    return Coder{Parameter:make(map[string]string),Ctx:ctx,Valid:false}
}
func NewCoder()Coder{
    return Coder{Parameter:make(map[string]string),Ctx:nil,Valid:false}
}
/*
func NewCoder(Parameter map[string]string){

}*/

func(c * Coder)Init(){

}

func(c * Coder)Open(){
    c.Codec=C.avcodec_find_decoder(c.Ctx.codec_id)
    if(c.Codec==nil){
	println("could not find Codec")
	return
    }
    res:=C.avcodec_open(c.Ctx,c.Codec)
    if(res<0){
	println("error openning codec")
	return
    }
    println("codec openned")
    
}

func(c * Coder)Decode(p Packet){
    packet:=new(C.AVPacket)
    C.av_init_packet(packet)
    defer C.av_free_packet(packet)

    packet.pts=_Ctypedef_int64_t(p.Pts)
    packet.dts=_Ctypedef_int64_t(p.Dts)
    packet.size=_Ctype_int(p.Size)
    //packet.data:=make([]byte, p.Size)
//    myBytes:=make([]byte,p.Size);
//    packet.data=unsafe.Pointer(&myBytes[0])
//    packet.data=unsafe.Pointer(p.Data)
//    for i:= 0; i < re.Size; i++ {
//      re.Data[i] = data[i];
//    }
    packet.stream_index=_Ctype_int(p.Stream)
    packet.flags=_Ctype_int(p.Flags)
    packet.duration=_Ctype_int(p.Duration)
    packet.pos=_Ctypedef_int64_t(p.Pos)


    var frame * C.AVFrame 
    frameFinished:=C.int(0)
    C.avcodec_decode_video2(c.Ctx,frame,&frameFinished,packet)    
}
func(c * Coder)Close(){

}

