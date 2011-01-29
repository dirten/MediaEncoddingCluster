package gmf


//#include "libavcodec/avcodec.h"
//#include "libavcodec/opt.h"
//#include "libavformat/avformat.h"
import "C"
import "unsafe"
import "fmt"
import "sync"
var CODEC_TYPE_VIDEO uint32=C.CODEC_TYPE_VIDEO
var CODEC_TYPE_AUDIO uint32=C.CODEC_TYPE_AUDIO
var CODEC_TYPE_ENCODER int=1
var CODEC_TYPE_DECODER int=2
var AVCODEC_MAX_AUDIO_FRAME_SIZE int=C.AVCODEC_MAX_AUDIO_FRAME_SIZE

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

func (p * Packet)free(){
    av_free_packet(p)
    //C.av_free(unsafe.Pointer(&p.avpacket))
    //println("object destroyed")
}


type Frame struct{
    avframe * C.AVFrame
    audiobuffer * []byte
    isFinished bool
    width int
    height int
    size int
}

type Stream struct{
    *C.AVStream
}

type FormatContext struct {
    ctx *C.AVFormatContext
}

type Codec struct{
    codec *C.AVCodec
}

type CodecContext struct{
    ctx *C.AVCodecContext
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

func avpicture_fill(frame * Frame, buffer *[] byte, fmt, width, height int)int{
    //var pbuffer * byte=buffer[0]
    frame.avframe=new(C.AVFrame)
    return int(C.avpicture_fill((*C.AVPicture)(unsafe.Pointer(frame.avframe)), (*C.uint8_t)(unsafe.Pointer(buffer)), 0, C.int(width), C.int(height)))
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

func avcodec_decode_audio(ctx * CodecContext, buffer * []byte, size * int, packet * Packet){
    if(packet.avpacket==nil){
	println("null packet received!!!")
	return
    }
  outbuf := (*C.uint8_t) (C.av_malloc(C.uint(*size)));
  defer C.av_free(unsafe.Pointer(outbuf))
  C.avcodec_decode_audio3(
    ctx.ctx,
    (*C.int16_t)(unsafe.Pointer(outbuf)),
    (*C.int)(unsafe.Pointer(size)),
    packet.avpacket)
}
func avcodec_encode_video(ctx * CodecContext,buffer * []byte, size * int,frame * Frame)int{
//  return 0
  outbuf := (*C.uint8_t) (C.av_malloc(C.uint(*size)));
  defer C.av_free(unsafe.Pointer(outbuf))
  return int(C.avcodec_encode_video(ctx.ctx, (*C.uint8_t) (unsafe.Pointer(outbuf)), C.int(*size), (*C.AVFrame)(unsafe.Pointer(frame.avframe))));
  //ret := C.avcodec_encode_video(c.Ctx, buffer, buffer_size, frame);
 }
