package gmf


type Track struct{
  *Stream
  stream chan Packet
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
    //print(p)
  self.stream<-*p
  return true
}

func (self * Track)GetDecoder()Decoder{
  coder:=Decoder{}//NewCoder()
  coder.Ctx.ctx=self.codec//dpx.Ds.Ctx.streams[streamid].codec
  return coder
}
