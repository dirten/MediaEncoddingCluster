package gmf


//#include "libavcodec/opt.h"
//#include "libavutil/fifo.h"
//#include "libavutil/avutil.h"
import "C"
import "unsafe"
import "fmt"

/*
type Rational struct{
    Num int
    Den int
}*/

type AVFifoBuffer struct{
    av_fifo *C.AVFifoBuffer
}

type Option struct{
    C.AVOption
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

func av_clip(a, amin, amax int)int{
    if(a < amin){
	 return amin
    }
    if(a > amax){
	return amax
    }
    return a
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

func av_get_bits_per_sample_fmt(fmt int32)int{
    return int(C.av_get_bits_per_sample_fmt(fmt))
}
