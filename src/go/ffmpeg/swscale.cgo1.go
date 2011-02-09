// Created by cgo - DO NOT EDIT

//line swscale.go:1
package gmf


import "unsafe"


type SwsContext struct {
	sws *[0]byte
}

func sws_scale_getcontext(ctx *SwsContext, srcwidth, srcheight, srcfmt, trgwidth, trgheight, trgfmt, flags int) {
	ctx.sws = _Cfunc_sws_getContext(_Ctype_int(srcwidth), _Ctype_int(srcheight), int32(srcfmt), _Ctype_int(trgwidth), _Ctype_int(trgheight), int32(trgfmt), _Ctype_int(flags), nil, nil, nil)
	if ctx.sws == nil {
		println("error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	}
}

func sws_scale(ctx *SwsContext, src *Frame, trg *Frame) int {
	in_data := (**_Ctypedef_uint8_t)(unsafe.Pointer(&src.avframe.data))
	in_line := (*_Ctype_int)(unsafe.Pointer(&src.avframe.linesize))
	out_data := (**_Ctypedef_uint8_t)(unsafe.Pointer(&trg.avframe.data))
	out_line := (*_Ctype_int)(unsafe.Pointer(&trg.avframe.linesize))
	result := int(_Cfunc_sws_scale(ctx.sws,
		in_data,
		in_line,
		0, _Ctype_int(src.height), out_data,
		out_line))
	return result
}
