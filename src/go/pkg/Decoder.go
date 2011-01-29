package ffmpeg

//#include "libavcodec/avcodec.h"
import "C"
//import "unsafe"

//import "os"

type Fr * C.AVFrame

type Decoder2 struct {
    
}
func NewDecoder()Decoder2{
    decoder:=Decoder2{}
    return decoder
}

func (self * Decoder2)Decode(p Packet)bool{
    return true
}

func (self * Decoder2)decodeVideo(p Packet)bool{
    return true
}
