// Created by cgo - DO NOT EDIT

//line avformat.go:1
package gmf


import "unsafe"

func init() {
	_Cfunc_av_register_all()

}

type Stream struct {
	*_Ctypedef_AVStream
}

type FormatContext struct {
	ctx *_Ctypedef_AVFormatContext
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

func av_close_input_file(ctx *FormatContext) {
	_Cfunc_av_close_input_file(ctx.ctx)
}

func av_find_stream_info(ctx *FormatContext) int {
	return int(_Cfunc_av_find_stream_info(ctx.ctx))
}

func av_read_frame(ctx *FormatContext, packet *AVPacket) int {
	return int(_Cfunc_av_read_frame(ctx.ctx, (*_Ctypedef_AVPacket)(unsafe.Pointer(packet))))
}

func av_interleaved_write_frame(ctx *FormatContext, packet *Packet) int {
	return int(_Cfunc_av_interleaved_write_frame((*_Ctypedef_AVFormatContext)(unsafe.Pointer(ctx.ctx)), (*_Ctypedef_AVPacket)(unsafe.Pointer(packet.avpacket))))
}

func av_write_header(ctx *FormatContext) int {
	return int(_Cfunc_av_write_header(ctx.ctx))
}

func av_write_trailer(ctx *FormatContext) int {
	return int(_Cfunc_av_write_trailer(ctx.ctx))
}

func dump_format(ctx *FormatContext) {
	_Cfunc_dump_format(ctx.ctx, 0, nil, 1)
}

func av_new_stream(ctx *FormatContext, stream_id int) *Stream {
	return &Stream{_Cfunc_av_new_stream(ctx.ctx, _Ctype_int(stream_id))}
}
func av_free(ctx *FormatContext) {
	_Cfunc_av_free(unsafe.Pointer(ctx.ctx))
}
