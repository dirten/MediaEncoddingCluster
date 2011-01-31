package gmf

//import "unsafe"

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
    result[i]=Track{&Stream{dpx.Ds.ctx.ctx.streams[i]}, make(chan  Packet)}
  }
  dpx.tracks=&result;
  return &result
}

func (dpx * Demultiplexer)Start(){
  for true {
    packet:=new(Packet)
    av_init_packet(packet)
    //defer av_free_packet(packet)
    if(av_read_frame(dpx.Ds.ctx, packet)>=0){
      av_dup_packet(packet)
    }else{
      println("end of file reached, closing channels")
      for i:=0;i<len(*dpx.tracks);i++ {
        print("closing channel ")
        println(i)
        close((*dpx.tracks)[i].stream)
      }
      break
    }
    /*
    var re Packet
    
    re.Pts=int64(packet.avpacket.pts)
    re.Dts=int64(packet.avpacket.dts)
    re.Size=int(packet.avpacket.size)
    re.Data=make([]byte, re.Size)
    data:=(*(*[1<<30]byte)(unsafe.Pointer(packet.avpacket.data)))[0:packet.avpacket.size]
    for i:= 0; i < re.Size; i++ {
      re.Data[i] = data[i];
    }
    re.Stream=int(packet.avpacket.stream_index)
    re.Flags=int(packet.avpacket.flags)
    re.Duration=int(packet.avpacket.duration)
    re.Pos=int64(packet.avpacket.pos)
    av_free_packet(packet)*/
    track:=(*dpx.tracks)[packet.avpacket.stream_index]
    packet.Pts=Timestamp{int64(packet.avpacket.pts),Rational{int(track.time_base.num),int(track.time_base.den)}}
    packet.Dts=Timestamp{int64(packet.avpacket.dts),Rational{int(track.time_base.num),int(track.time_base.den)}}
    packet.Duration=Timestamp{int64(packet.avpacket.duration),Rational{int(track.time_base.num),int(track.time_base.den)}}
    track.stream<-*packet
  }
  //re * Packet
}
