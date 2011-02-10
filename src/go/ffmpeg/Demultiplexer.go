package gmf

import "unsafe"
//import "runtime"
type Demultiplexer struct{
  Ds DataSource;
  tracks *[]Track
}

func (dpx * Demultiplexer)GetDuration() Duration {
  return Duration{Timestamp{int64(dpx.Ds.ctx.ctx.duration),Rational{1,1000000}}}
}

func (dpx * Demultiplexer)GetTimestamp() Timestamp{
  return Timestamp{int64(dpx.Ds.ctx.ctx.timestamp),Rational{1,1000000}}
}


func (dpx * Demultiplexer)GetTracks()*[]Track{
  scount:=dpx.Ds.ctx.ctx.nb_streams
  var result []Track=make([]Track,int(scount))
  for i:=0;i<int(scount);i++ {
    result[i]=Track{&Stream{dpx.Ds.ctx.ctx.streams[i]}, make(chan  Packet),0}
  }
  dpx.tracks=&result;
  return &result
}

func (dpx * Demultiplexer)Start(){
    avpacket:=new(AVPacket)
    av_init_packet2(avpacket)
  for true {
    //defer av_free_packet(packet)
    if(av_read_frame(dpx.Ds.ctx, avpacket)>=0){
      //av_dup_packet(avpacket)
    }else{
      println("end of file reached, closing channels")
      for i:=0;i<len(*dpx.tracks);i++ {
        print("closing channel ")
        println(i)
        close((*dpx.tracks)[i].stream)
      }
      break
    }
    
    //var re Packet
    
    //re.Pts=int64(packet.avpacket.pts)
    //re.Dts=int64(packet.avpacket.dts)
    //re.Size=int(packet.avpacket.size)
    //re.Stream=int(packet.avpacket.stream_index)
    //re.Flags=int(packet.avpacket.flags)
    //re.Duration=int(packet.avpacket.duration)
    //re.Pos=int64(packet.avpacket.pos)
    //av_free_packet(packet)
    //runtime.SetFinalizer(packet, av_free_packet)
    track:=(*dpx.tracks)[avpacket.stream_index]
    
    packet:=new(Packet)
    //av_init_packet(packet)

    packet.Size=int(avpacket.size)
    packet.Pts=Timestamp{int64(avpacket.pts),Rational{int(track.time_base.num),int(track.time_base.den)}}
    packet.Dts=Timestamp{int64(avpacket.dts),Rational{int(track.time_base.num),int(track.time_base.den)}}
    packet.Duration=Timestamp{int64(avpacket.duration),Rational{int(track.time_base.num),int(track.time_base.den)}}
    packet.Data=make([]byte, packet.Size+8)
    //packet.Data=(*(*[1<<30]byte)(unsafe.Pointer(packet.avpacket.data)))[0:packet.Size+8]

    data:=(*(*[1<<30]byte)(unsafe.Pointer(avpacket.data)))[0:packet.Size]
    for i:= 0; i < packet.Size; i++ {
      packet.Data[i] = data[i];
    }
    //println(packet.String())
    av_free_packet2(avpacket)
    track.stream<-*packet
  }
  //re * Packet
}
func NewDemultiplexer(ds * DataSource)*Demultiplexer{
    return &Demultiplexer{Ds:*ds}
}

