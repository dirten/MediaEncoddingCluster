package gmf


//#include "libavcodec/avcodec.h"
//#include "libavcodec/opt.h"
//#include "libavformat/avformat.h"
//#include "libswscale/swscale.h"
//#include "libavutil/avutil.h"
//typedef void * SwsContext;
import "C"
import "unsafe"
import "fmt"
import "sync"
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
func (p * Packet)free(){
    av_free_packet(p)
    //C.av_free(unsafe.Pointer(&p.avpacket))
    //println("object destroyed")
}
func (p * Packet)destroy(){
    //C.av_free(unsafe.Pointer(&p.avpacket))
    println("object destroyed")
}


type Frame struct{
    avframe * C.AVFrame
    audiobuffer * []byte
    isFinished bool
    width int
    height int
    size int
    duration int
    Pts Timestamp
    Duration Timestamp
}
func (p * Frame)destroy(){
    //C.av_free(unsafe.Pointer(&p.avpacket))
    println("Frame object destroyed")
}

type Stream struct{
    *C.AVStream
}

func (s * Stream)free(){
    C.av_free(unsafe.Pointer(s))
    //C.av_free(unsafe.Pointer(&p.avpacket))
    //println("object destroyed")
}

type FormatContext struct {
    ctx *C.AVFormatContext
}
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

type InputFormat struct {
    format * C.AVInputFormat
}

type OutputFormat struct {
    format * C.AVOutputFormat
}

type FormatParameters struct {
    params * C.AVFormatParameters
}

func sws_scale_getcontext(ctx * SwsContext, srcwidth, srcheight, srcfmt, trgwidth,trgheight,trgfmt,flags int){
          _swsContext :=C.sws_getContext(C.int(srcwidth), C.int(srcheight), uint32(srcfmt), C.int(trgwidth), C.int(trgheight),uint32(trgfmt), C.int(flags), nil, nil, nil)
	if(_swsContext==nil){
	    println("error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	}
	println("init sclae")
	println(_swsContext)
	ctx.sws=_swsContext
	//size []int
}

func sws_scale(ctx * SwsContext,src * Frame, trg * Frame)int{
    in_data:=(**C.uint8_t)(unsafe.Pointer(&src.avframe.data))
    in_line:=(*_Ctype_int)(unsafe.Pointer(&src.avframe.linesize))
    out_data:=(**_Ctypedef_uint8_t)(unsafe.Pointer(&trg.avframe.data))
    out_line:=(*_Ctype_int)(unsafe.Pointer(&trg.avframe.linesize))
	return int(C.sws_scale(ctx.sws, 
	    in_data, 
	    in_line, 
	    0, 
	    C.int(src.height), 
	    out_data, 
	    out_line));

        //sws_scale(_swsContext, in_frame.getAVFrame()->data, in_frame.getAVFrame()->linesize, 0, in_frame.getHeight(), out_frame.getAVFrame()->data, out_frame.getAVFrame()->linesize);

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

func avformat_alloc_context()*FormatContext{
    return &FormatContext{ctx:C.avformat_alloc_context()}
}

func url_fopen(ctx * FormatContext, filename string)int{
    file:=C.CString(filename)
    defer C.free(unsafe.Pointer(file))
    return int(C.url_fopen(&ctx.ctx.pb, file,C.URL_WRONLY))
}

func url_fclose(ctx * FormatContext)int{
    return int(C.url_fclose(ctx.ctx.pb))
}

func av_set_parameters(ctx * FormatContext, params * FormatParameters)int{
    return int(C.av_set_parameters(ctx.ctx, params.params))
}

func av_guess_format(format, filename string )OutputFormat{
    result:=OutputFormat{}
    cfilename:=C.CString(filename)
    cformat:=C.CString(format)
    defer C.free(unsafe.Pointer(cfilename))
    defer C.free(unsafe.Pointer(cformat))
    fmt:=C.av_guess_format(cformat, cfilename, nil);
    result.format=fmt
    return result
}

func av_open_input_file(ctx * FormatContext, filename string, format * InputFormat, bufsize int, params * FormatParameters)int {
  cfilename:=C.CString(filename);
  //defer C.free(unsafe.Pointer(cfilename))
  return int(C.av_open_input_file(
    &ctx.ctx,
    cfilename, 
    /*(*C.AVInputFormat)(unsafe.Pointer(&format))*/nil,
    C.int(bufsize),
    /*(*C.AVFormatParameters)(unsafe.Pointer(&params))*/nil));
}
func av_free_packet(p * Packet){
    C.av_free_packet(p.avpacket)
}

func av_close_input_file(ctx * FormatContext){
    C.av_close_input_file(ctx.ctx);
}

func av_find_stream_info(ctx * FormatContext)int{
  return int(C.av_find_stream_info(ctx.ctx))
}

func avcodec_alloc_context()*CodecContext{
    return &CodecContext{C.avcodec_alloc_context()}
}

func av_read_frame(ctx * FormatContext, packet * Packet)int{
    return int(C.av_read_frame(ctx.ctx, packet.avpacket))
}

func av_interleaved_write_frame(ctx * FormatContext, packet * Packet)int{
    return int(C.av_interleaved_write_frame(ctx.ctx, packet.avpacket))
}

func av_write_header(ctx * FormatContext)int{
    return int(C.av_write_header(ctx.ctx))
}
func dump_format(ctx * FormatContext){
    C.dump_format(ctx.ctx,0,nil,1)
}

func av_new_stream(ctx * FormatContext, stream_id int)*Stream{
    return &Stream{C.av_new_stream(ctx.ctx, C.int(stream_id))}
}

func av_pup_packet(packet * Packet){
      C.av_dup_packet(packet.avpacket)
}

func av_init_packet(packet * Packet){
    packet.avpacket=new(C.AVPacket)
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

type Void interface {}

func av_malloc(size int)*[]byte{
  outbuf := make([]byte, size+128)//(C.av_malloc(C.uint(size)));
  return &outbuf
}

func avpicture_fill(frame * Frame, buffer *[] byte, format, width, height int)int{
    //var pbuffer * byte=buffer[0]
  alloc_avframe(frame)
  
  outbuf := (*C.uint8_t) (C.av_malloc(C.uint(len(*buffer))));
  defer C.av_free(unsafe.Pointer(outbuf))
  data:=(*(*[1<<30]byte)(unsafe.Pointer(outbuf)))[0:len(*buffer)]
  //array:=*buffer
  *buffer=data
  /*for i:= 0; i < len(*buffer); i++ {
    array[i] = data[i];
  }*/
  //fmt.Printf("buffer address %d", &buffer)
  result:= int(C.avpicture_fill((*C.AVPicture)(unsafe.Pointer(frame.avframe)), (*C.uint8_t)(unsafe.Pointer(outbuf)), uint32(format), C.int(width), C.int(height)))
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

func avcodec_decode_video(ctx * CodecContext, frame * Frame, finished * int, packet * Packet){
    if(packet.avpacket==nil){
	println("null packet received!!!")
	return
    }
  C.avcodec_decode_video2(
    ctx.ctx,
    (*C.AVFrame)(unsafe.Pointer(frame.avframe)),
    (*C.int)(unsafe.Pointer(finished)),
    packet.avpacket)
}

func avcodec_decode_audio(ctx * CodecContext, buffer * []byte, size * int, packet * Packet)int{
    if(packet.avpacket==nil){
	println("null packet received!!!")
	return 0
    }
  outbuf := (*C.uint8_t) (C.av_malloc(C.uint(*size)));
  defer C.av_free(unsafe.Pointer(outbuf))
  result:=int(C.avcodec_decode_audio3(
    ctx.ctx,
    (*C.int16_t)(unsafe.Pointer(outbuf)),
    (*C.int)(unsafe.Pointer(size)),
    packet.avpacket))
/*    data:=(*(*[1<<30]byte)(unsafe.Pointer(outbuf)))[0:result]
    array:=*buffer
    //log.Printf("audio result size %d", result)
    for i:= 0; i < result; i++ {
	array[i] = data[i];
    }*/
    return result
}

func avcodec_encode_video(ctx * CodecContext,buffer * []byte, size * int,frame * Frame)int{
//  return 0
  outbuf := (*C.uint8_t) (C.av_malloc(C.uint(*size)));
  defer C.av_free(unsafe.Pointer(outbuf))
  result:=int(C.avcodec_encode_video(ctx.ctx, (*C.uint8_t) (unsafe.Pointer(outbuf)), C.int(*size), (*C.AVFrame)(unsafe.Pointer(frame.avframe))));
  data:=(*(*[1<<30]byte)(unsafe.Pointer(outbuf)))[0:result]
  array:=*buffer
  for i:= 0; i < result; i++ {
    array[i] = data[i];
  }
  return result
  
  //ret := C.avcodec_encode_video(c.Ctx, buffer, buffer_size, frame);
 }
