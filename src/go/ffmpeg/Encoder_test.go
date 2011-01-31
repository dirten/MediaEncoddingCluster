package gmf


import "testing"

func _TestEncoderOpen(t*testing.T){
    var encoder Encoder
    encoder.SetParameter("codecid","13")
    encoder.SetParameter("time_base","1/25")
    encoder.Open()
}

func encoder_test(track * Track){
    var encoder Encoder
  decoder:=track.GetDecoder()
  decoder.Open()
  
 if(decoder.Ctx.ctx.codec_type==CODEC_TYPE_VIDEO){
    println("Create encoder")
    encoder.SetParameter("codecid","17")
    encoder.SetParameter("time_base","1/25")
    encoder.SetParameter("width","320")
    encoder.SetParameter("height","240")
    encoder.SetParameter("bf","0")
    encoder.SetParameter("b","512000")
    encoder.Open()
  }

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
	if(decoder.Ctx.ctx.codec_type==CODEC_TYPE_VIDEO){
	    //frame.avframe.pict_type=0
	    //frame.avframe.key_frame=1
	    encoder.Encode(*frame)
	}

    }
  }
}

func TestEncoder(t*testing.T){
    println("starting encoder test")
    //loc:=MediaLocator{Filename:"/media/video/ChocolateFactory.ts"}
    //loc:=MediaLocator{"/media/TREKSTOR/videos/20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts"}
    loc:=MediaLocator{Filename:"../../../test.dvd"}
    source:=DataSource{Locator:loc}
    if(!source.Connect()){
    	t.Fatalf("cold not open file : %s", loc.Filename)
    }
    plex:=Demultiplexer{Ds:source}
    //tracks:=plex.GetTracks()
    tracks:=*plex.GetTracks()
    for i:=0;i<len(tracks);i++ {
	//dec:=tracks[i].GetDecoder()
	//dec.Open()
      go encoder_test(&tracks[i])
    }
    plex.Start()
    //println(len(tracks))
    //println(plexer.GetTimestamp().String())
    source.Disconnect()
    println(" encoder test finished")
}

