package gmf


//#include "libavcodec/avcodec.h"
//#include "libavcodec/opt.h"
//#include "libavformat/avformat.h"
//#include "libswscale/swscale.h"
//#include "libavutil/avutil.h"
//#include "libavutil/fifo.h"
import "C"
import "unsafe"
import "fmt"
import "sync"
import "os"
//import "runtime"
//import "log"
var CODEC_TYPE_VIDEO uint32=C.CODEC_TYPE_VIDEO
var CODEC_TYPE_AUDIO uint32=C.CODEC_TYPE_AUDIO
var CODEC_TYPE_ENCODER int=1
var CODEC_TYPE_DECODER int=2
var AVCODEC_MAX_AUDIO_FRAME_SIZE int=C.AVCODEC_MAX_AUDIO_FRAME_SIZE
var TIME_BASE_Q = Rational{1,1000000}

func init(){
  C.avcodec_register_all();
  C.av_register_all();
  C.av_log_set_level(C.AV_LOG_VERBOSE);
}

type Rational struct{
    Num int
    Den int
}

type Packet struct{
    avpacket * C.AVPacket
    time_base Rational
    Dts Timestamp
    Pts Timestamp
    Duration Timestamp
/*
    Pts int64
  Dts int64
  Data [] byte
  Size int
  Stream int
  Flags int
  Duration int
  Pos int64
*/
}
func NewPacket()*Packet{
    result:=new(Packet)
    result.avpacket=new(C.AVPacket)
    av_init_packet(result)
    return result
}

func (p * Packet)Free(){

    av_free_packet(p)
    //C.av_free(unsafe.Pointer(&p.avpacket))
    //println("packet destroyed")
}
func (p * Packet)destroy(){
    av_free_packet(p)
    //C.av_free(unsafe.Pointer(&p.avpacket))
    //println("packet object destroyed")
}

type AVFifoBuffer struct{
    av_fifo *C.AVFifoBuffer
}

type Frame struct{
    avframe * C.AVFrame
    buffer []byte
    isFinished bool
    width int
    height int
    size int
    duration int
    Pts Timestamp
    Duration Timestamp
}

func (p * Frame)destroy(){
    if(p.avframe!=nil){
	C.av_free(unsafe.Pointer(p.avframe))
    }
    //C.av_free(unsafe.Pointer(&p.avpacket))
    println("Frame object destroyed")
    
}
func (p * Frame)IsFinished()bool{
    return p.isFinished
}
func free_frame(frame * Frame){
    println("free_frame Frame object destroyed")
    //frame.destroy()
}
func NewFrame(fmt, width, height int)*Frame{
  var frame * Frame=new(Frame)
  frame.isFinished=false
  numBytes:= avpicture_get_size(0, width, height)
  if(numBytes>0){
    b:=make([]byte,numBytes)
    frame.buffer =b
    avpicture_fill(frame, frame.buffer, 0, width, height);
  }
  frame.size=numBytes
  frame.width=width
  frame.height=height
  //runtime.SetFinalizer(frame, free_frame)
  return frame
}

type Stream struct{
    *C.AVStream
}

func (s * Stream)free(){
    C.av_free(unsafe.Pointer(s))
    //C.av_free(unsafe.Pointer(&p.avpacket))
    //println("object destroyed")
}
/*
type FormatContext struct {
    ctx *C.AVFormatContext
}*/
func av_free(ctx*FormatContext){
    C.av_free(unsafe.Pointer(ctx.ctx))
}
type Codec struct{
    codec *C.AVCodec
}

type CodecContext struct{
    ctx *C.AVCodecContext
}

type SwsContext struct{
//    sws unsafe.Pointer
    sws *C.struct_SwsContext
    //sws *[0]uint8
}

type Option struct{
    C.AVOption
}
/*
type InputFormat struct {
    format * C.AVInputFormat
}

type OutputFormat struct {
    format * C.AVOutputFormat
}

type FormatParameters struct {
    params * C.AVFormatParameters
}
*/
func sws_scale_getcontext(ctx * SwsContext, srcwidth, srcheight, srcfmt, trgwidth,trgheight,trgfmt,flags int){
        ctx.sws=C.sws_getContext(C.int(srcwidth), C.int(srcheight), uint32(srcfmt), C.int(trgwidth), C.int(trgheight),uint32(trgfmt), C.int(flags), nil, nil, nil)
	if(ctx.sws==nil){
	    println("error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	}
}

func sws_scale(ctx * SwsContext,src * Frame, trg * Frame)int{
    in_data:=(**C.uint8_t)(unsafe.Pointer(&src.avframe.data))
    in_line:=(*_Ctype_int)(unsafe.Pointer(&src.avframe.linesize))
    out_data:=(**_Ctypedef_uint8_t)(unsafe.Pointer(&trg.avframe.data))
    out_line:=(*_Ctype_int)(unsafe.Pointer(&trg.avframe.linesize))
	result:= int(C.sws_scale(ctx.sws, 
	    in_data, 
	    in_line, 
	    0, 
	    C.int(src.height), 
	    out_data, 
	    out_line));
   return result
}

func av_set_string(ctx * CodecContext, key, val string)bool{
    result:=true
    ckey:=C.CString(key)
    cval:=C.CString(val)
    defer C.free(unsafe.Pointer(ckey))
    defer C.free(unsafe.Pointer(cval))
    var o * C.AVOption=new(C.AVOption)
    if(C.av_set_string3(unsafe.Pointer(ctx.ctx), ckey, cval, 1, &o)!=0){
	result=false
	if(o==nil){
	    fmt.Printf("option for %s not found!\n", key)
	}
	fmt.Printf("Error while setting option '%s' = '%s'\n", key, val)
    }
    return result
}

func av_cmp_q(left, right Rational) int {
    var a C.AVRational=C.AVRational{C.int(left.Num), C.int(left.Den)}
    var b C.AVRational=C.AVRational{C.int(right.Num), C.int(right.Den)}
    return int(C.av_cmp_q(a, b))
}

func av_rescale_q(time int64, src, trg Rational)int64{
    var a C.AVRational=C.AVRational{C.int(src.Num), C.int(src.Den)}
    var b C.AVRational=C.AVRational{C.int(trg.Num), C.int(trg.Den)}

    cresult:=C.av_rescale_q(C.int64_t(time),a, b)
    return int64(cresult)
}

func av_compare_ts(leftts int64, leftbase Rational, rightts int64, rightbase Rational )int {
    var a C.AVRational=C.AVRational{C.int(leftbase.Num), C.int(leftbase.Den)}
    var b C.AVRational=C.AVRational{C.int(rightbase.Num), C.int(rightbase.Den)}
    return int(C.av_compare_ts(C.int64_t(leftts), a,C.int64_t(rightts),b))
}


func av_free_packet(p * Packet){
    if(p.avpacket!=nil){
	C.av_free_packet(p.avpacket)
    }
}

func avcodec_alloc_context()*CodecContext{
    return &CodecContext{C.avcodec_alloc_context()}
}

func avcodec_get_context_defaults2(ctx  CodecContext,codec  Codec ){
    C.avcodec_get_context_defaults2(ctx.ctx,codec.codec._type);
}

func av_pup_packet(packet * Packet){
      C.av_dup_packet(packet.avpacket)
}

func av_init_packet(packet * Packet){
    if(packet.avpacket==nil){
	packet.avpacket=new(C.AVPacket)
    }
    C.av_init_packet(packet.avpacket)
}

func av_dup_packet(packet * Packet){
    C.av_dup_packet(packet.avpacket)
}

func avcodec_find_decoder(codec_id uint32)Codec{
    var codec Codec
  codec.codec=C.avcodec_find_decoder(codec_id)
  return codec
}

func avcodec_find_encoder(codec_id uint32)Codec{
  var codec Codec
  codec.codec=C.avcodec_find_encoder(codec_id)
  return codec
}

func avpicture_get_size(fmt uint32, width, height int)int{
    return int(C.avpicture_get_size(fmt, C.int(width), C.int(height)))
}

func avcodec_get_frame_defaults(frame * Frame){
  alloc_avframe(frame)
  C.avcodec_get_frame_defaults((*C.AVFrame)(unsafe.Pointer(frame.avframe)))
}

func avpicture_alloc(frame * Frame, fmt, width, height int)int{
    return int(C.avpicture_alloc((*C.AVPicture)(unsafe.Pointer(frame.avframe)),uint32(fmt),C.int(width),C.int(height)))
}

func av_malloc_dont_use_this(size int)(*C.uint8_t){
    return (*C.uint8_t) (C.av_malloc(C.uint(size)));
}

func avpicture_fill(frame * Frame, buffer [] byte, format, width, height int)int{
    //var pbuffer * byte=buffer[0]
  avcodec_get_frame_defaults(frame)
  //alloc_avframe(frame)
  //outbuf := (*C.uint8_t) (C.av_malloc(C.uint(len(*buffer))));
  //defer C.av_free(unsafe.Pointer(outbuf))
  //fmt.Printf("buffer address %d", &buffer)
  result:= int(C.avpicture_fill((*C.AVPicture)(unsafe.Pointer(frame.avframe)), (*C.uint8_t)(unsafe.Pointer(&buffer[0])), uint32(format), C.int(width), C.int(height)))
  //data:=(*(*[1<<30]byte)(unsafe.Pointer(outbuf)))[0:result]
  //array:=*buffer
  //*buffer=data
//  for i:= 0; i < result; i++ {
//    array[i] = data[i];
//  }
  return result
}

func alloc_avframe(frame * Frame){
    if(frame.avframe==nil){
	frame.avframe=new(C.AVFrame)
    }
}

var avcodec_mutex sync.Mutex

func avcodec_open(cctx CodecContext, codec Codec)int{
  avcodec_mutex.Lock()
  res:=C.avcodec_open(cctx.ctx,codec.codec)
  avcodec_mutex.Unlock()
  return int(res)

}

func avcodec_decode_video(ctx * CodecContext, frame * Frame, finished * int, packet * Packet)int{
  return int(C.avcodec_decode_video2(
    ctx.ctx,
    (*C.AVFrame)(unsafe.Pointer(frame.avframe)),
    (*C.int)(unsafe.Pointer(finished)),
    packet.avpacket))
}

func avcodec_decode_audio(ctx * CodecContext, buffer []byte, size * int, packet * Packet)int{
  return int(C.avcodec_decode_audio3(
    ctx.ctx,
    (*C.int16_t)(unsafe.Pointer(&buffer[0])),
    (*C.int)(unsafe.Pointer(size)),
    packet.avpacket))
}

func avcodec_encode_video(ctx * CodecContext,buffer []byte, size * int,frame * Frame)int{
  return int(C.avcodec_encode_video(
    ctx.ctx, 
    (*C.uint8_t)(unsafe.Pointer(&buffer[0])), 
    C.int(*size), 
    (*C.AVFrame)(frame.avframe)))
 }

func avcodec_encode_audio(ctx * CodecContext,buffer []byte, size * int,frame * Frame)int{
    return 0
}

func av_fifo_alloc(size uint)*AVFifoBuffer{
    return &AVFifoBuffer{av_fifo:C.av_fifo_alloc(C.uint(size))}
}

func av_fifo_realloc(fifo * AVFifoBuffer, newsize uint)int{
    return int(C.av_fifo_realloc2((*C.AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo)),C.uint(newsize)))
}

func av_fifo_free(fifo * AVFifoBuffer){
    C.av_fifo_free((*C.AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo)))
}
func av_fifo_size(fifo * AVFifoBuffer)int{
    return int(C.av_fifo_size((*C.AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo))))
}

func av_fifo_space(fifo * AVFifoBuffer)int{
    return int(C.av_fifo_space((*C.AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo))))
}

func av_fifo_generic_write(fifo * AVFifoBuffer,buffer []byte, size int)int{
    return int(C.av_fifo_generic_write(
	(*C.AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo)),
	(unsafe.Pointer(&buffer[0])),
	C.int(size),nil))
}

func av_fifo_generic_read(fifo * AVFifoBuffer,buffer []byte, size int)int{
    return int(C.av_fifo_generic_read(
	(*C.AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo)),
	(unsafe.Pointer(&buffer[0])),
	C.int(size),nil))
}

func av_get_bits_per_sample_fmt(fmt uint32)int{
    print("sample fmt")
    println(fmt)
    return int(C.av_get_bits_per_sample_fmt(fmt))
}

var number int=0

func ppsWriter(frame * Frame){
   header:=fmt.Sprintf("P5\n%d %d\n255\n",frame.width,frame.height)
   
   file, err := os.Open("test.ppm", os.O_CREATE|os.O_TRUNC|os.O_RDWR, 0666)
   
   if(err!=nil){
    println("error while openning file")
   }
   file.WriteString(header)
   //println(frame.avframe)
   linesize:=int(frame.avframe.linesize[0])

   //println(linesize)
   data:=make([]byte, frame.width)
   tmpdata:=(*(*[1<<30]byte)(unsafe.Pointer(frame.avframe.data[0])))[0:frame.size]
   //    for i:= 0; i < 1; i++ {
   //      data[i] = tmpdata[i];
   //    }
   //println(linesize)
   //log.Printf("Frame data %d",frame.avframe.data)
   //println(tmpdata)
   for i := 0; i < int(frame.height); i++{
   for a:= 0; a < int(frame.width); a++ {
   data[a] = tmpdata[(i*linesize)+a];
   }
   file.Write(data);
   }
//   return 
   
   file.Close()
    if(number>250){
//    os.Exit(1)
    }
    number++
}
