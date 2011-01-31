// Created by cgo - DO NOT EDIT

//line ffmpeg.go:1
package gmf


import "unsafe"
import "fmt"
import "sync"

var CODEC_TYPE_VIDEO uint32 = _Cconst_CODEC_TYPE_VIDEO
var CODEC_TYPE_AUDIO uint32 = _Cconst_CODEC_TYPE_AUDIO
var CODEC_TYPE_ENCODER int = 1
var CODEC_TYPE_DECODER int = 2
var AVCODEC_MAX_AUDIO_FRAME_SIZE int = _Cconst_AVCODEC_MAX_AUDIO_FRAME_SIZE
var TIME_BASE_Q = Rational{1, 1000000}

func init() {
	_Cfunc_avcodec_register_all()
	_Cfunc_av_register_all()
	_Cfunc_av_log_set_level(_Cconst_AV_LOG_VERBOSE)
}

type Rational struct {
	Num	int
	Den	int
}

type Packet struct {
	avpacket	*_Ctypedef_AVPacket
	time_base	Rational
	Dts		Timestamp
	Pts		Timestamp
	Duration	Timestamp
}

func NewPacket() *Packet {
	result := new(Packet)
	result.avpacket = new(_Ctypedef_AVPacket)
	av_init_packet(result)
	return result
}
func (p *Packet) free() {
	av_free_packet(p)

}
func (p *Packet) destroy() {

	println("object destroyed")
}


type Frame struct {
	avframe		*_Ctypedef_AVFrame
	audiobuffer	*[]byte
	isFinished	bool
	width		int
	height		int
	size		int
	duration	int
	Pts		Timestamp
	Duration	Timestamp
}

func (p *Frame) destroy() {

	println("Frame object destroyed")
}

type Stream struct {
	*_Ctypedef_AVStream
}

func (s *Stream) free() {
	_Cfunc_av_free(unsafe.Pointer(s))

}

type FormatContext struct {
	ctx *_Ctypedef_AVFormatContext
}

func av_free(ctx *FormatContext) {
	_Cfunc_av_free(unsafe.Pointer(ctx.ctx))
}

type Codec struct {
	codec *_Ctypedef_AVCodec
}

type CodecContext struct {
	ctx *_Ctypedef_AVCodecContext
}

type SwsContext struct {
	sws *[0]byte
}

type Option struct {
	_Ctypedef_AVOption
}

type InputFormat struct {
	format *_Ctypedef_AVInputFormat
}

type OutputFormat struct {
	format *_Ctypedef_AVOutputFormat
}

type FormatParameters struct {
	params *_Ctypedef_AVFormatParameters
}

func sws_scale_getcontext(ctx *SwsContext, srcwidth, srcheight, srcfmt, trgwidth, trgheight, trgfmt, flags int) {
	_swsContext := _Cfunc_sws_getContext(_Ctype_int(srcwidth), _Ctype_int(srcheight), uint32(srcfmt), _Ctype_int(trgwidth), _Ctype_int(trgheight), uint32(trgfmt), _Ctype_int(flags), nil, nil, nil)
	if _swsContext == nil {
		println("error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	}
	println("init sclae")
	println(_swsContext)
	ctx.sws = _swsContext

}

func sws_scale(ctx *SwsContext, src *Frame, trg *Frame) int {
	in_data := (**_Ctypedef_uint8_t)(unsafe.Pointer(&src.avframe.data))
	in_line := (*_Ctype_int)(unsafe.Pointer(&src.avframe.linesize))
	out_data := (**_Ctypedef_uint8_t)(unsafe.Pointer(&trg.avframe.data))
	out_line := (*_Ctype_int)(unsafe.Pointer(&trg.avframe.linesize))
	return int(_Cfunc_sws_scale(ctx.sws,
		in_data,
		in_line,
		0, _Ctype_int(src.height), out_data,
		out_line))

}

func av_set_string(ctx *CodecContext, key, val string) bool {
	result := true
	ckey := _Cfunc_CString(key)
	cval := _Cfunc_CString(val)
	defer _Cfunc_free(unsafe.Pointer(ckey))
	defer _Cfunc_free(unsafe.Pointer(cval))
	var o *_Ctypedef_AVOption = new(_Ctypedef_AVOption)
	if _Cfunc_av_set_string3(unsafe.Pointer(ctx.ctx), ckey, cval, 1, &o) != 0 {
		result = false
		if o == nil {
			fmt.Printf("option for %s not found!\n", key)
		}
		fmt.Printf("Error while setting option '%s' = '%s'\n", key, val)
	}
	return result
}

func av_cmp_q(left, right Rational) int {
	var a _Ctypedef_AVRational = _Ctypedef_AVRational{_Ctype_int(left.Num), _Ctype_int(left.Den)}
	var b _Ctypedef_AVRational = _Ctypedef_AVRational{_Ctype_int(right.Num), _Ctype_int(right.Den)}
	return int(_Cfunc_av_cmp_q(a, b))
}

func av_rescale_q(time int64, src, trg Rational) int64 {
	var a _Ctypedef_AVRational = _Ctypedef_AVRational{_Ctype_int(src.Num), _Ctype_int(src.Den)}
	var b _Ctypedef_AVRational = _Ctypedef_AVRational{_Ctype_int(trg.Num), _Ctype_int(trg.Den)}

	cresult := _Cfunc_av_rescale_q(_Ctypedef_int64_t(time), a, b)
	return int64(cresult)
}

func av_compare_ts(leftts int64, leftbase Rational, rightts int64, rightbase Rational) int {
	var a _Ctypedef_AVRational = _Ctypedef_AVRational{_Ctype_int(leftbase.Num), _Ctype_int(leftbase.Den)}
	var b _Ctypedef_AVRational = _Ctypedef_AVRational{_Ctype_int(rightbase.Num), _Ctype_int(rightbase.Den)}
	return int(_Cfunc_av_compare_ts(_Ctypedef_int64_t(leftts), a, _Ctypedef_int64_t(rightts), b))
}

func avformat_alloc_context() *FormatContext {
	return &FormatContext{ctx: _Cfunc_avformat_alloc_context()}
}

func url_fopen(ctx *FormatContext, filename string) int {
	file := _Cfunc_CString(filename)
	defer _Cfunc_free(unsafe.Pointer(file))
	return int(_Cfunc_url_fopen(&ctx.ctx.pb, file, _Cconst_URL_WRONLY))
}

func url_fclose(ctx *FormatContext) int {
	return int(_Cfunc_url_fclose(ctx.ctx.pb))
}

func av_set_parameters(ctx *FormatContext, params *FormatParameters) int {
	return int(_Cfunc_av_set_parameters(ctx.ctx, params.params))
}

func av_guess_format(format, filename string) OutputFormat {
	result := OutputFormat{}
	cfilename := _Cfunc_CString(filename)
	cformat := _Cfunc_CString(format)
	defer _Cfunc_free(unsafe.Pointer(cfilename))
	defer _Cfunc_free(unsafe.Pointer(cformat))
	fmt := _Cfunc_av_guess_format(cformat, cfilename, nil)
	result.format = fmt
	return result
}

func av_open_input_file(ctx *FormatContext, filename string, format *InputFormat, bufsize int, params *FormatParameters) int {
	cfilename := _Cfunc_CString(filename)

	return int(_Cfunc_av_open_input_file(
		&ctx.ctx,
		cfilename,
		nil, _Ctype_int(bufsize), nil))
}
func av_free_packet(p *Packet) {
	_Cfunc_av_free_packet(p.avpacket)
}

func av_close_input_file(ctx *FormatContext) {
	_Cfunc_av_close_input_file(ctx.ctx)
}

func av_find_stream_info(ctx *FormatContext) int {
	return int(_Cfunc_av_find_stream_info(ctx.ctx))
}

func avcodec_alloc_context() *CodecContext {
	return &CodecContext{_Cfunc_avcodec_alloc_context()}
}

func av_read_frame(ctx *FormatContext, packet *Packet) int {
	return int(_Cfunc_av_read_frame(ctx.ctx, packet.avpacket))
}

func av_interleaved_write_frame(ctx *FormatContext, packet *Packet) int {
	return int(_Cfunc_av_interleaved_write_frame(ctx.ctx, packet.avpacket))
}

func av_write_header(ctx *FormatContext) int {
	return int(_Cfunc_av_write_header(ctx.ctx))
}
func dump_format(ctx *FormatContext) {
	_Cfunc_dump_format(ctx.ctx, 0, nil, 1)
}

func av_new_stream(ctx *FormatContext, stream_id int) *Stream {
	return &Stream{_Cfunc_av_new_stream(ctx.ctx, _Ctype_int(stream_id))}
}

func av_pup_packet(packet *Packet) {
	_Cfunc_av_dup_packet(packet.avpacket)
}

func av_init_packet(packet *Packet) {
	packet.avpacket = new(_Ctypedef_AVPacket)
	_Cfunc_av_init_packet(packet.avpacket)
}

func av_dup_packet(packet *Packet) {
	_Cfunc_av_dup_packet(packet.avpacket)
}

func avcodec_find_decoder(codec_id uint32) Codec {
	var codec Codec
	codec.codec = _Cfunc_avcodec_find_decoder(codec_id)
	return codec
}
func avcodec_find_encoder(codec_id uint32) Codec {
	var codec Codec
	codec.codec = _Cfunc_avcodec_find_encoder(codec_id)
	return codec
}

func avpicture_get_size(fmt uint32, width, height int) int {
	return int(_Cfunc_avpicture_get_size(fmt, _Ctype_int(width), _Ctype_int(height)))
}

func avcodec_get_frame_defaults(frame *Frame) {
	alloc_avframe(frame)
	_Cfunc_avcodec_get_frame_defaults((*_Ctypedef_AVFrame)(unsafe.Pointer(frame.avframe)))
}

func avpicture_alloc(frame *Frame, fmt, width, height int) int {
	return int(_Cfunc_avpicture_alloc((*_Ctypedef_AVPicture)(unsafe.Pointer(frame.avframe)), uint32(fmt), _Ctype_int(width), _Ctype_int(height)))
}

type Void interface{}

func av_malloc(size int) *[]byte {
	outbuf := make([]byte, size+128)
	return &outbuf
}

func avpicture_fill(frame *Frame, buffer *[]byte, format, width, height int) int {

	alloc_avframe(frame)

	outbuf := (*_Ctypedef_uint8_t)(_Cfunc_av_malloc(_Ctype_uint(len(*buffer))))
	defer _Cfunc_av_free(unsafe.Pointer(outbuf))
	data := (*(*[1 << 30]byte)(unsafe.Pointer(outbuf)))[0:len(*buffer)]

	*buffer = data

	result := int(_Cfunc_avpicture_fill((*_Ctypedef_AVPicture)(unsafe.Pointer(frame.avframe)), (*_Ctypedef_uint8_t)(unsafe.Pointer(outbuf)), uint32(format), _Ctype_int(width), _Ctype_int(height)))
	return result
}

func alloc_avframe(frame *Frame) {
	if frame.avframe == nil {
		frame.avframe = new(_Ctypedef_AVFrame)
	}
}

var avcodec_mutex sync.Mutex

func avcodec_open(cctx CodecContext, codec Codec) int {
	avcodec_mutex.Lock()
	res := _Cfunc_avcodec_open(cctx.ctx, codec.codec)
	avcodec_mutex.Unlock()
	return int(res)

}

func avcodec_decode_video(ctx *CodecContext, frame *Frame, finished *int, packet *Packet) {
	if packet.avpacket == nil {
		println("null packet received!!!")
		return
	}
	_Cfunc_avcodec_decode_video2(
		ctx.ctx,
		(*_Ctypedef_AVFrame)(unsafe.Pointer(frame.avframe)),
		(*_Ctype_int)(unsafe.Pointer(finished)),
		packet.avpacket)
}

func avcodec_decode_audio(ctx *CodecContext, buffer *[]byte, size *int, packet *Packet) int {
	if packet.avpacket == nil {
		println("null packet received!!!")
		return 0
	}
	outbuf := (*_Ctypedef_uint8_t)(_Cfunc_av_malloc(_Ctype_uint(*size)))
	defer _Cfunc_av_free(unsafe.Pointer(outbuf))
	result := int(_Cfunc_avcodec_decode_audio3(
		ctx.ctx,
		(*_Ctypedef_int16_t)(unsafe.Pointer(outbuf)),
		(*_Ctype_int)(unsafe.Pointer(size)),
		packet.avpacket))

	return result
}

func avcodec_encode_video(ctx *CodecContext, buffer *[]byte, size *int, frame *Frame) int {

	outbuf := (*_Ctypedef_uint8_t)(_Cfunc_av_malloc(_Ctype_uint(*size)))
	defer _Cfunc_av_free(unsafe.Pointer(outbuf))
	result := int(_Cfunc_avcodec_encode_video(ctx.ctx, (*_Ctypedef_uint8_t)(unsafe.Pointer(outbuf)), _Ctype_int(*size), (*_Ctypedef_AVFrame)(unsafe.Pointer(frame.avframe))))
	data := (*(*[1 << 30]byte)(unsafe.Pointer(outbuf)))[0:result]
	array := *buffer
	for i := 0; i < result; i++ {
		array[i] = data[i]
	}
	return result

}
