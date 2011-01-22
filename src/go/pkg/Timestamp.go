package ffmpeg

//#include "libavutil/rational.h"
//#include "libavutil/mathematics.h"
import "C"

import "fmt"
type Timestamp struct{
    Time int64
    Timebase Rational
}



func (time * Timestamp) RescaleTo(base Rational) Timestamp{
    var result Timestamp
    return result
}


func (time * Timestamp) Greater (ts Timestamp) bool{
    return C.av_compare_ts(_Ctypedef_int64_t(time.Time), *time.Timebase.GetAVRational(),_Ctypedef_int64_t(ts.Time),*ts.Timebase.GetAVRational())==1
}

func (time * Timestamp) Lower (ts Timestamp) bool{
    return C.av_compare_ts(_Ctypedef_int64_t(time.Time), *time.Timebase.GetAVRational(),_Ctypedef_int64_t(ts.Time),*ts.Timebase.GetAVRational())==-1
}

func (time * Timestamp) Equals (ts Timestamp) bool{
    return time.Time == ts.Time && C.av_cmp_q(*ts.Timebase.GetAVRational(), *time.Timebase.GetAVRational())==0;
}

func (time Timestamp) String () string{
    return fmt.Sprintf("%d:%d/%d", time.Time,time.Timebase.Numerator, time.Timebase.Denominator)
}

type Duration Timestamp

func (time * Duration) RescaleTo(base Rational) Timestamp{
    var result Timestamp
    
    return result
}


func (time * Duration) Greater (ts Duration) bool{
    return C.av_compare_ts(_Ctypedef_int64_t(time.Time), *time.Timebase.GetAVRational(),_Ctypedef_int64_t(ts.Time),*ts.Timebase.GetAVRational())==1
}

func (time * Duration) Lower (ts Duration) bool{
    return C.av_compare_ts(_Ctypedef_int64_t(time.Time), *time.Timebase.GetAVRational(),_Ctypedef_int64_t(ts.Time),*ts.Timebase.GetAVRational())==-1
}

func (time * Duration) Equals (ts Duration) bool{
    return time.Time == ts.Time && C.av_cmp_q(*ts.Timebase.GetAVRational(), *time.Timebase.GetAVRational())==0;
}

func (time Duration) String () string{
    return fmt.Sprintf("%d:%d/%d", time.Time,time.Timebase.Numerator, time.Timebase.Denominator)
}
