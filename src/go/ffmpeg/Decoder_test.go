package gmf


import "testing"
//import "number"
func decoder_test(track * Track, finish chan bool){
  decoder:=track.GetDecoder()
  decoder.Open()
  var p * Packet=new(Packet)
  for true {
    if(!track.ReadPacket(p)){
	    println("stream end reached")
	    break
    }
    frame:=decoder.Decode(p)
    
    //fmt.Printf("frame:%d codecid:%d\n",frame,decoder.Ctx.ctx.codec_id)
    if(frame!=nil&&frame.isFinished){
	 if(decoder.Ctx.ctx.codec_type==CODEC_TYPE_VIDEO){
	    //ppsWriter(frame)
	}
    }
  }
  finish<-true
}

func TestDecoderTracks(t*testing.T){
    println("starting decoder test")
    //loc:=MediaLocator{Filename:"/media/video/ChocolateFactory.ts"}
    loc:=MediaLocator{Filename:"../../../target/dependency/fixtures/testfile.flv"}

    //loc:=MediaLocator{Filename:"/media/TREKSTOR/videos/20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts"}
    source:=DataSource{Locator:loc}
    result:=source.Connect()
    println(result)
    if(!result){
    	t.Fatalf("cold not open file : %s", loc.Filename)
    
    }
    plex:=Demultiplexer{Ds:source}
    //tracks:=plex.GetTracks()
    tracks:=*plex.GetTracks()
    finish:=make(chan bool)
    for i:=0;i<len(tracks);i++ {
	//dec:=tracks[i].GetDecoder()
	//dec.Open()
      go decoder_test(&tracks[i], finish)
    }
    plex.Start()
    for i:=0;i<len(tracks);i++ {
	/*waiting for all tracks to be finished*/
	<-finish
    }
    //println(len(tracks))
    //println(plexer.GetTimestamp().String())
    source.Disconnect()
    println(" decoder test finished")

}
