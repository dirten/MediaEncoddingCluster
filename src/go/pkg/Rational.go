package ffmpeg

//#include "libavutil/rational.h"
import "C"

type Rational struct{
    Numerator int
    Denominator int
}

func NewRational() Rational {
    var result Rational=Rational{1,1000000}
    return result
}

func(r * Rational) GetAVRational() *C.AVRational{
    var a * C.AVRational=new(C.AVRational)
    a.num=_Ctype_int(r.Numerator);
    a.den=_Ctype_int(r.Denominator);
    return a;
}

func (r*Rational) Equals(r2 Rational)bool{
    return C.av_cmp_q(*r.GetAVRational(), *r2.GetAVRational())==0
}

func (r*Rational) Greater(r2 Rational)bool{
    return C.av_cmp_q(*r.GetAVRational(), *r2.GetAVRational())==-1
}

func (r*Rational) Lower(r2 Rational)bool{
    return C.av_cmp_q(*r.GetAVRational(), *r2.GetAVRational())==1
}
