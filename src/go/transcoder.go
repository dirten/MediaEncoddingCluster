package main

import . "gmf"
import "log"
import "time"
func multiplex_encoder_test(track * Track, multiplexer * Multiplexer){
    var encoder Encoder
    decoder:=track.GetDecoder()
    decoder.Open()
    var resizer * Resizer
 if(decoder.GetCodecType()==CODEC_TYPE_VIDEO){
    println("Create encoder")
    encoder.SetParameter("codecid","13")
    encoder.SetParameter("time_base","1/25")
    //encoder.SetParameter("aspect","64/45")
    encoder.SetParameter("width","320")
    encoder.SetParameter("height","240")
    //encoder.SetParameter("bf","0")
    encoder.SetParameter("b","500000")
    encoder.SetParameter("g","250")
    encoder.SetParameter("qmin","2")
    encoder.SetParameter("qmax","51")
    encoder.SetParameter("qdiff","4")
    encoder.SetParameter("flags","+global_header")
    encoder.Open()
    resizer=new(Resizer)
    resizer.Init(decoder, &encoder)
    multiplexer.AddTrack(&encoder)

  }
  if(decoder.GetCodecType()==CODEC_TYPE_AUDIO){
    println("Create encoder")
    encoder.SetParameter("codecid","86017")
    encoder.SetParameter("channels","2")
    encoder.SetParameter("ar","48000")
    encoder.SetParameter("ab","128000")
    encoder.SetParameter("flags","+global_header")
    encoder.SetParameter("request_channels","2")
    encoder.SetParameter("request_channel_layout","2")
    encoder.Open()
    multiplexer.AddTrack(&encoder)
  }

  var bytecounter=0
  var p * Packet=new(Packet)
  for true {
    if(!track.ReadPacket(p)){
	println("stream end reached")
	break
    }
    frame:=decoder.Decode(p)
    p.Free()
    if(frame!=nil&&frame.IsFinished()){
	if(decoder.GetCodecType()==CODEC_TYPE_VIDEO){
	    frame=resizer.Resize(frame)
	}
        encoder.Encode(frame)
    }
    //p.Free()
  }
  print("bytes encoded = ")
  println(bytecounter)
}

func main(){
    //loc:=MediaLocator{Filename:"/media/video/ChocolateFactory.ts"}
    //loc:=MediaLocator{Filename:"/media/TREKSTOR/videos/20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts"}
    //loc:=MediaLocator{Filename:"../../test.dvd"}
    //loc:=MediaLocator{Filename:"/Users/jholscher/Movies/39,90.avi.divx"}
    loc:=MediaLocator{Filename:"/Users/jholscher/Movies/ChocolateFactory.ts"}
    //loc:=MediaLocator{Filename:"/home/HoelscJ/39,90.avi.divx"}
    source:=NewDataSource(loc)
    if(!source.Connect()){
    	log.Printf("cold not open file : %s", loc.Filename)
    }
    plex:=NewDemultiplexer(source)
    var sink = NewDatasink(MediaLocator{Filename:"testmultiplexer.avi",Format:"avi"})
    sink.Connect()
    var multiplexer = NewMultiplexer(sink)
    tracks:=*plex.GetTracks()
    for i:=0;i<len(tracks);i++ {
      go multiplex_encoder_test(&tracks[i], multiplexer)
    }
    time.Sleep(500000000)
    go multiplexer.Start()
    plex.Start()
    time.Sleep(10000000)
    multiplexer.Stop()
    //plex.Stop()
    source.Disconnect()
}

