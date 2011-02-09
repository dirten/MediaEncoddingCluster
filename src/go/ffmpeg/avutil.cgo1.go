// Created by cgo - DO NOT EDIT

//line avutil.go:1
package gmf


import "unsafe"
import "fmt"


type AVFifoBuffer struct {
	av_fifo *_Ctypedef_AVFifoBuffer
}

type Option struct {
	_Ctypedef_AVOption
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

func av_clip(a, amin, amax int) int {
	if a < amin {
		return amin
	}
	if a > amax {
		return amax
	}
	return a
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


func av_fifo_alloc(size uint) *AVFifoBuffer {
	return &AVFifoBuffer{av_fifo: _Cfunc_av_fifo_alloc(_Ctype_uint(size))}
}

func av_fifo_realloc(fifo *AVFifoBuffer, newsize uint) int {
	return int(_Cfunc_av_fifo_realloc2((*_Ctypedef_AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo)), _Ctype_uint(newsize)))
}

func av_fifo_free(fifo *AVFifoBuffer) {
	_Cfunc_av_fifo_free((*_Ctypedef_AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo)))
}

func av_fifo_size(fifo *AVFifoBuffer) int {
	return int(_Cfunc_av_fifo_size((*_Ctypedef_AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo))))
}

func av_fifo_space(fifo *AVFifoBuffer) int {
	return int(_Cfunc_av_fifo_space((*_Ctypedef_AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo))))
}

func av_fifo_generic_write(fifo *AVFifoBuffer, buffer []byte, size int) int {
	return int(_Cfunc_av_fifo_generic_write(
		(*_Ctypedef_AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo)),
		(unsafe.Pointer(&buffer[0])), _Ctype_int(size), nil))
}

func av_fifo_generic_read(fifo *AVFifoBuffer, buffer []byte, size int) int {
	return int(_Cfunc_av_fifo_generic_read(
		(*_Ctypedef_AVFifoBuffer)(unsafe.Pointer(fifo.av_fifo)),
		(unsafe.Pointer(&buffer[0])), _Ctype_int(size), nil))
}

func av_get_bits_per_sample_fmt(fmt int32) int {
	return int(_Cfunc_av_get_bits_per_sample_fmt(fmt))
}
