package ffmpeg

//#include "libavcodec/avcodec.h"
import "C"

type Coder struct {
    Parameter []string
    Ctx *C.AVCodecContext
    Valid bool
}


func(c * Coder)Init(){

}

