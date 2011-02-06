package gmf
import "log"
import "fmt"
type Track struct{
  *Stream
  stream chan Packet
  next_pts  int64
}
func (self * Track)String()string{
    
    return fmt.Sprintf("Idx:%d;CTB:%d/%d;STB:%d/%d",self.index, self.codec.time_base.num,self.codec.time_base.den,self.time_base.num,self.time_base.den)
    
    }
func (self * Track)GetFormat()Format{
  return Format{}
}

func (self * Track)GetStreamId()int{
  return int(self.index)
}

func (self * Track)GetStartTime()Timestamp{
  return Timestamp{}
}

func (self * Track)ReadPacket(p * Packet)bool{
  *p=<-self.stream
  if(closed(self.stream)){
    return false;
  }
  return true
}

func (self * Track)WritePacket(p * Packet)bool{
  if(self.stream ==nil||closed(self.stream)){
    return false;
  }
  p.Stream=int(self.index)
  if(self.next_pts>0&&p.Pts.Time!=self.next_pts){
      log.Printf("Fail: next_pts=%d incoming pts=", self.next_pts, p.Pts.Time)
  }else{
      self.next_pts=p.Pts.Time
  }
  self.next_pts+=p.Duration.Time
    //print(p)
  self.stream<-*p
  return true
}

func (self * Track)GetDecoder()*Decoder{
  coder:=Decoder{}//NewCoder()
  coder.Ctx.ctx=self.codec//dpx.Ds.Ctx.streams[streamid].codec
  coder.frame_rate=Rational{int(self.r_frame_rate.num), int(self.r_frame_rate.den)}
  coder.time_base=Rational{int(self.codec.time_base.num), int(self.codec.time_base.den)}
  return &coder
}
