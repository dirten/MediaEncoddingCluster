package gmf


//#include "libavformat/avformat.h"
import "C"
import "unsafe"
import "fmt"
func init(){
	fmt.Println("Register all Formats")
  C.av_register_all();

//  C.av_log_set_level(48);
}

type Stream struct{
    *C.AVStream
}

type FormatContext struct {
    ctx *C.AVFormatContext
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

func av_close_input_file(ctx * FormatContext){
    C.av_close_input_file(ctx.ctx);
}

func av_find_stream_info(ctx * FormatContext)int{
  return int(C.av_find_stream_info(ctx.ctx))
}

func av_read_frame(ctx * FormatContext, packet * AVPacket)int{
    return int(C.av_read_frame(ctx.ctx, (*C.AVPacket)(unsafe.Pointer(packet))))
}

func av_interleaved_write_frame(ctx * FormatContext, packet * Packet)int{
    return int(C.av_interleaved_write_frame((*C.AVFormatContext)(unsafe.Pointer(ctx.ctx)),(*C.AVPacket)(unsafe.Pointer( packet.avpacket))))
}

func av_write_header(ctx * FormatContext)int{
    return int(C.av_write_header(ctx.ctx))
}

func av_write_trailer(ctx * FormatContext)int{
    return int(C.av_write_trailer(ctx.ctx))
}

func dump_format(ctx * FormatContext){
    C.dump_format(ctx.ctx,0,nil,1)
}

func av_new_stream(ctx * FormatContext, stream_id int)*Stream{
    return &Stream{C.av_new_stream(ctx.ctx, C.int(stream_id))}
}

func av_free_format_context(ctx*FormatContext){
    C.av_free(unsafe.Pointer(ctx.ctx))
}
