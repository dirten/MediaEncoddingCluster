package gmf
import "log"
type Multiplexer struct{
  Ds DataSink;
  tracks *[]Track
  ch chan Packet
}

func(self * Multiplexer)AddTrack(enc * Encoder)*Track{
    if(self.ch==nil){
	self.ch=make(chan Packet)
    }
    result:=Track{av_new_stream(self.Ds.ctx,0),self.ch}
    result.codec=enc.Ctx.ctx
    log.Printf("TrackData %s",result)
    return &result
    //stream.codec=track.codec
//    track.stream=self.ch
}

func(self * Multiplexer)Start(){
    av_write_header(self.Ds.ctx)
    dump_format(self.Ds.ctx)
    for true {
	var p Packet=<-self.ch
	//println(p.avpacket.dts)
	//println(self.Ds.ctx.ctx.preload)
	if(p.avpacket.data==nil){
	    println("nil packet")
	    return
	}
	//println("try writing frame")
	result:=av_interleaved_write_frame(self.Ds.ctx,&p)
	if(result!=0){
	    log.Printf("failed write packet to stream")
	}
	p.Free()
	//println("frame written")

    }
}

func(self * Multiplexer)Stop(){
    log.Printf("Writing Trailer")
    //av_write_trailer(self.Ds.ctx);
}
func NewMultiplexer(sink *DataSink)*Multiplexer{
    return &Multiplexer{Ds:*sink}
}