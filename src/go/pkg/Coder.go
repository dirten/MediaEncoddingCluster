package ffmpeg

////#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
import "C"
//import "unsafe"
//import "fmt"
//import "os"
//type avcontext C.AVCodecContext
type avcontext C.AVRational

type Coder struct {

    Parameter map[string]string
    //Ctx *C.AVCodecContext
    //Codec *C.AVCodec
    Valid bool
}
/*
func NewCoder()Coder{
    return Coder{Parameter:make(map[string]string),Ctx:nil,Valid:false}
}*/
/*
func NewCoder(Parameter map[string]string){

}*/

func(c * Coder)Init(){

}

func(c * Coder)Open(){
/*
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
*/    
}

func(c * Coder)Decode(p Packet){
/*
    packet:=new(C.AVPacket)
    C.av_init_packet(packet)
    defer C.av_free_packet(packet)

    packet.pts=_Ctypedef_int64_t(p.Pts)
    packet.dts=C.int64_t(p.Dts)
    packet.size=C.int(p.Size)
    //packet.data=&C.uint8_t(make([]byte, p.Size)[0])
    //myBytes:=make([]byte,p.Size);
    
    var pb *byte
    if(len(p.Data)>0){
        println("Data > 0")
	pb=&p.Data[0]
    }
    packet.data=(*C.uint8_t)(unsafe.Pointer(pb))
//    println(p.Data)
//    println(pb)
//    println(packet.data)
//    packet.data=unsafe.Pointer(p.Data)
//    for i:= 0; i < re.Size; i++ {
//      re.Data[i] = data[i];
//    }
    packet.stream_index=C.int(p.Stream)
    packet.flags=C.int(p.Flags)
    packet.duration=C.int(p.Duration)
    packet.pos=C.int64_t(p.Pos)


    var frame * C.AVFrame=new(C.AVFrame)
    
    numBytes:= C.avpicture_get_size(0, 854, 480)
    //_size = numBytes;
    //_buffer := (*C.uint8_t) (C.av_malloc(C.uint(numBytes)));
    if(numBytes>0){
	var buffer []byte=make([]byte,numBytes)
	var pbuffer * byte=&buffer[0]
     // C.memset((*C.uint8_t)(_buffer), 0, C.size_t(numBytes));
    // Assign appropriate parts of buffer to image planes
    //var picture * C.AVPicture=(*C.AVPicture)(unsafe.Pointer(frame))
    println(frame)
    //println(picture)
    //:=&frame
    //854x480
      C.avpicture_fill((*C.AVPicture)(unsafe.Pointer(frame)), (*C.uint8_t)(unsafe.Pointer(pbuffer)), 0, 854, 480);
    }

    
    //frameFinished:=C.int(0)
    var frameFinished *C.int=new(C.int);
    bytes_decoded:=C.avcodec_decode_video2(c.Ctx,frame,frameFinished,packet)
    //C.avcodec_decode_video2(nil,nil,nil,nil)

    header:=fmt.Sprintf("P5\n%d %d\n255\n",854,480)
    
    file, err := os.Open("test.ppm", os.O_CREATE|os.O_TRUNC|os.O_RDWR, 0666)
    
    if(err!=nil){
	println("error while openning file")
    }
    file.WriteString(header)
    linesize:=int(frame.linesize[0])
    println(linesize)
    data:=make([]byte, 854)
    tmpdata:=(*(*[1<<30]byte)(unsafe.Pointer(frame.data[0])))[0:numBytes]
//    for i:= 0; i < 1; i++ {
//      data[i] = tmpdata[i];
//    }
    
    for i := 0; i < 480; i++{
        for a:= 0; a < 854; a++ {
    	    data[a] = tmpdata[(i*linesize)+a];
	}
	file.Write(data);
    }

    file.Close()
    //ioutil.WriteFile("test.data", header,0755)
    println("bla fasel")
    println(bytes_decoded)
    println(*frameFinished)*/
}
func(c * Coder)Close(){

}

