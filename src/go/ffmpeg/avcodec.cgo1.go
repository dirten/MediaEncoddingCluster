// Created by cgo - DO NOT EDIT

//line avcodec.go:1
package gmf


import "unsafe"
import "sync"
import "fmt"


var CODEC_TYPE_VIDEO int32 = _Cconst_CODEC_TYPE_VIDEO
var CODEC_TYPE_AUDIO int32 = _Cconst_CODEC_TYPE_AUDIO
var AV_NOPTS_VALUE int64 = -9223372036854775808
var CODEC_TYPE_ENCODER int = 1
var CODEC_TYPE_DECODER int = 2
var AVCODEC_MAX_AUDIO_FRAME_SIZE int = _Cconst_AVCODEC_MAX_AUDIO_FRAME_SIZE
var TIME_BASE_Q = Rational{1, 1000000}

func init() {
	fmt.Println("Register all Codecs")
	_Cfunc_avcodec_register_all()

}

type AVPacket struct {
	_Ctypedef_AVPacket
}

var av_resample_mutex sync.Mutex

func av_audio_resample_init(trgch, srcch, trgrate, srcrate, trgfmt, srcfmt int) *ResampleContext {
	data := _Cfunc_gmf_audio_resample_init(_Ctype_int(trgch), _Ctype_int(srcch), _Ctype_int(trgrate), _Ctype_int(srcrate), _Ctype_int(1), _Ctype_int(1), 16, 10, 0)

	fmt.Printf("ReSampleContext Data =%p\n", data)

	ctx := ResampleContext{ctx: data}

	return &ctx
}

func audio_resample(ctx *ResampleContext, outbuffer, inbuffer []byte, size int) int {
	av_resample_mutex.Lock()
	if ctx.ctx == nil {
		fmt.Printf("no ReSampleContext here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
		return 0
	}
	result := int(_Cfunc_audio_resample(ctx.ctx,
		(*_Ctype_short)(unsafe.Pointer(&outbuffer[0])),
		(*_Ctype_short)(unsafe.Pointer(&inbuffer[0])), _Ctype_int(size)))
	av_resample_mutex.Unlock()
	return result
}

func audio_resample_close(ctx *ResampleContext) {
	_Cfunc_gmf_audio_resample_close(ctx.ctx)
}

func av_resample_compensate(ctx *ResampleContext, delta, distance int) {
	_Cfunc_gmf_resample_compensate(ctx.ctx, _Ctype_int(delta), _Ctype_int(distance))
}

type Packet struct {
	avpacket	*_Ctypedef_AVPacket
	time_base	Rational
	Dts		Timestamp
	Pts		Timestamp
	Duration	Timestamp
	Data		[]byte
	Size		int
	Stream		int
	Flags		int
	Pos		int64
}
type AVFrame struct {
	_Ctypedef_AVFrame
}

type Frame struct {
	avframe		*_Ctypedef_AVFrame
	buffer		[]byte
	isFinished	bool
	width		int
	height		int
	size		int
	duration	int
	Pts		Timestamp
	Duration	Timestamp
	frame_count	int
}

type Codec struct {
	codec *_Ctypedef_AVCodec
}

type CodecContext struct {
	ctx *_Ctypedef_AVCodecContext
}

type ResampleContext struct {
	ctx *[0]byte
}

func NewPacket() *Packet {
	result := new(Packet)
	result.avpacket = new(_Ctypedef_AVPacket)
	av_init_packet(result)
	return result
}

func (p *Packet) String() string {
	return fmt.Sprintf("S:%d;Pts:%s;Dts:%s;Idx:%d;Dur:%s|avp|S:%d;Pts:%d;Dts:%d;Idx:%d;Dur:%d", p.Size, p.Pts, p.Dts, p.Stream, p.Duration, p.avpacket.size, int64(p.avpacket.pts), int64(p.avpacket.dts), p.avpacket.stream_index, int(p.avpacket.duration))
}

func (p *Packet) Free() {
	av_free_packet(p)
}

func (p *Frame) String() string {
	return fmt.Sprintf("Size:%d:pts:%s", p.size, p.Pts)
}

func (p *Frame) destroy() {
	if p.avframe != nil {
		_Cfunc_av_free(unsafe.Pointer(p.avframe))
		p.avframe = nil
	}
}

func (p *Frame) IsFinished() bool {
	return p.isFinished
}

func free_frame(frame *Frame) {
	frame.destroy()
}

func NewFrame(fmt, width, height int) *Frame {
	var frame *Frame = new(Frame)
	frame.avframe = new(_Ctypedef_AVFrame)
	frame.isFinished = false
	numBytes := avpicture_get_size(int32(fmt), width, height)
	if numBytes > 0 {
		b := make([]byte, numBytes)
		frame.buffer = b
		avpicture_fill(frame, frame.buffer, fmt, width, height)

	}
	frame.size = numBytes
	frame.width = width
	frame.height = height
	frame.frame_count = 1
	return frame
}

var avcodec_mutex sync.Mutex

func avcodec_open(cctx CodecContext, codec Codec) int {
	avcodec_mutex.Lock()
	res := _Cfunc_avcodec_open(cctx.ctx, codec.codec)
	avcodec_mutex.Unlock()
	return int(res)

}

func avcodec_close(cctx CodecContext) {
	if cctx.ctx != nil {
		avcodec_mutex.Lock()
		_Cfunc_avcodec_close(cctx.ctx)
		avcodec_mutex.Unlock()
	}
}

func av_free_packet(p *Packet) {
	if p.avpacket != nil {
		_Cfunc_av_free_packet(p.avpacket)
		p.avpacket = nil
	}
}
func av_free_packet2(p *AVPacket) {
	if p != nil {
		_Cfunc_av_free_packet((*_Ctypedef_AVPacket)(unsafe.Pointer(p)))
		p = nil
	}
}

func av_free_codec_context(p *Coder) {
	if p.Ctx.ctx != nil {
		_Cfunc_av_free(unsafe.Pointer(p.Ctx.ctx))
	}
}

func avcodec_alloc_context() *CodecContext {
	return &CodecContext{ctx: _Cfunc_avcodec_alloc_context()}
}

func avcodec_get_context_defaults2(ctx *CodecContext, codec Codec) {
	_Cfunc_avcodec_get_context_defaults2(ctx.ctx, codec.codec._type)
}

func avpicture_fill(frame *Frame, buffer []byte, format, width, height int) int {
	return int(_Cfunc_avpicture_fill((*_Ctypedef_AVPicture)(unsafe.Pointer(frame.avframe)), (*_Ctypedef_uint8_t)(unsafe.Pointer(&buffer[0])), int32(format), _Ctype_int(width), _Ctype_int(height)))
}

func alloc_avframe(frame *Frame) {
	if frame.avframe == nil {
		frame.avframe = new(_Ctypedef_AVFrame)
	}
}

func av_init_packet(packet *Packet) {
	if packet.avpacket == nil {
		packet.avpacket = new(_Ctypedef_AVPacket)
	}
	_Cfunc_av_init_packet(packet.avpacket)
}
func av_init_packet2(packet *AVPacket) {
	_Cfunc_av_init_packet((*_Ctypedef_AVPacket)(unsafe.Pointer(packet)))
}

func av_dup_packet(packet *AVPacket) {
	_Cfunc_av_dup_packet((*_Ctypedef_AVPacket)(unsafe.Pointer(packet)))
}

func avcodec_find_decoder(codec_id int32) Codec {
	var codec Codec
	codec.codec = _Cfunc_avcodec_find_decoder(uint32(codec_id))
	return codec
}
func avcodec_find_decoder_by_name(name string) Codec {
	cname := _Cfunc_CString(name)
	defer _Cfunc_free(unsafe.Pointer(cname))
	return Codec{codec: _Cfunc_avcodec_find_decoder_by_name(cname)}
}
func avcodec_find_encoder(codec_id int32) Codec {
	var codec Codec
	codec.codec = _Cfunc_avcodec_find_encoder(uint32(codec_id))
	return codec
}

func avcodec_find_encoder_by_name(name string) Codec {
	cname := _Cfunc_CString(name)
	defer _Cfunc_free(unsafe.Pointer(cname))
	return Codec{codec: _Cfunc_avcodec_find_encoder_by_name(cname)}
}

func avpicture_get_size(fmt int32, width, height int) int {
	return int(_Cfunc_avpicture_get_size(fmt, _Ctype_int(width), _Ctype_int(height)))
}

func avcodec_get_frame_defaults(frame *Frame) {
	alloc_avframe(frame)
	_Cfunc_avcodec_get_frame_defaults((*_Ctypedef_AVFrame)(unsafe.Pointer(frame.avframe)))
}

func avpicture_alloc(frame *Frame, fmt, width, height int) int {
	return int(_Cfunc_avpicture_alloc((*_Ctypedef_AVPicture)(unsafe.Pointer(frame.avframe)), int32(fmt), _Ctype_int(width), _Ctype_int(height)))
}

func avcodec_decode_video(ctx *CodecContext, frame *Frame, finished *int, packet *AVPacket) int {
	return int(_Cfunc_avcodec_decode_video2(
		ctx.ctx,
		(*_Ctypedef_AVFrame)(unsafe.Pointer(frame.avframe)),
		(*_Ctype_int)(unsafe.Pointer(finished)),
		(*_Ctypedef_AVPacket)(unsafe.Pointer(packet))))
}

func avcodec_decode_audio(ctx *CodecContext, buffer []byte, size *int, packet *AVPacket) int {
	return int(_Cfunc_avcodec_decode_audio3(
		ctx.ctx,
		(*_Ctypedef_int16_t)(unsafe.Pointer(&buffer[0])),
		(*_Ctype_int)(unsafe.Pointer(size)),
		(*_Ctypedef_AVPacket)(unsafe.Pointer(packet))))
}

func avcodec_encode_video(ctx *CodecContext, buffer []byte, size *int, frame *Frame) int {
	return int(_Cfunc_avcodec_encode_video(
		ctx.ctx,
		(*_Ctypedef_uint8_t)(unsafe.Pointer(&buffer[0])), _Ctype_int(*size), (*_Ctypedef_AVFrame)(frame.avframe)))
}

func avcodec_encode_audio(ctx *CodecContext, outbuffer []byte, size *int, inbuffer []byte) int {
	out_size := _Cfunc_avcodec_encode_audio(
		ctx.ctx,
		(*_Ctypedef_uint8_t)(unsafe.Pointer(&outbuffer[0])), _Ctype_int(*size), (*_Ctype_short)(unsafe.Pointer(&inbuffer[0])))

	return int(out_size)
}


func avpicture_deinterlace(outframe, inframe *Frame, fmt, width, height int) int {
	return int(_Cfunc_avpicture_deinterlace(
		(*_Ctypedef_AVPicture)(unsafe.Pointer(outframe.avframe)),
		(*_Ctypedef_AVPicture)(unsafe.Pointer(inframe.avframe)),
		int32(fmt), _Ctype_int(width), _Ctype_int(height)))
}
