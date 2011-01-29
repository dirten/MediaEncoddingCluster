package gmf


import "testing"

func decoder_test(track * Track){
  decoder:=track.GetDecoder()
  decoder.Open()
  var p Packet
  for true {
    if(!track.ReadPacket(&p)){
	    println("stream end reached")
	    return
    }
    frame:=decoder.Decode(p)
    p.free()
    //fmt.Printf("frame:%d codecid:%d\n",frame,decoder.Ctx.ctx.codec_id)
    if(frame!=nil&&frame.isFinished){
	//println("frame finished")
	//ppsWriter(frame)
    }
  }
}

func TestDecoderTracks(t*testing.T){
    println("starting decoder test")
    //loc:=MediaLocator{"/media/video/ChocolateFactory.ts"}
    loc:=MediaLocator{Filename:"/media/TREKSTOR/videos/20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts"}
    source:=DataSource{Locator:loc}
    source.Connect()
    plex:=Demultiplexer{Ds:source}
    //tracks:=plex.GetTracks()
    tracks:=*plex.GetTracks()
    for i:=0;i<len(tracks);i++ {
	//dec:=tracks[i].GetDecoder()
	//dec.Open()
      go decoder_test(&tracks[i])
    }
    plex.Start()
    //println(len(tracks))
    //println(plexer.GetTimestamp().String())
    source.Disconnect()
    println(" decoder test finished")

}