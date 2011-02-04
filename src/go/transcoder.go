package main

import . "gmf"
import "log"
func multiplex_encoder_test(track * Track){
    var encoder Encoder
    //var loc=MediaLocator{Filename:"testmultiplexer.flv",Format:"flv"}
    var sink = NewDatasink(MediaLocator{Filename:"testmultiplexer.flv",Format:"flv"})
    sink.Connect()
    var multiplexer = NewMultiplexer(sink)
    decoder:=track.GetDecoder()
    decoder.Open()
    var outvideoTrack * Track
    var resizer * Resizer
 if(decoder.GetCodecType()==CODEC_TYPE_VIDEO){
    println("Create encoder")
    encoder.SetParameter("codecid","22")
    encoder.SetParameter("time_base","1/25")
    encoder.SetParameter("width","320")
    encoder.SetParameter("height","240")
    encoder.SetParameter("bf","0")
    encoder.SetParameter("b","500000")
    encoder.SetParameter("g","250")
    encoder.SetParameter("qmin","2")
    encoder.SetParameter("qmax","51")
    encoder.SetParameter("qdiff","4")
    encoder.SetParameter("flags","+global_header")
    encoder.Open()
    outvideoTrack=multiplexer.AddTrack(&encoder)
    print(outvideoTrack)
    resizer=new(Resizer)
    resizer.Init(decoder, &encoder)
    go multiplexer.Start()
  }

  var p * Packet=new(Packet)
  for true {
    if(!track.ReadPacket(p)){
	println("stream end reached")
	multiplexer.Stop()
	return
    }
    frame:=decoder.Decode(p)
    p.Free()
    if(frame!=nil&&frame.IsFinished()){
	if(decoder.GetCodecType()==CODEC_TYPE_VIDEO){
	    of:=resizer.Resize(frame)
	    op:=encoder.Encode(of)
	    if(op!=nil){
		outvideoTrack.WritePacket(op)
		//op.Free()
	    }
	}
    }
    p.Free()
  }
}

func main(){
    //loc:=MediaLocator{Filename:"/media/video/ChocolateFactory.ts"}
    //loc:=MediaLocator{Filename:"/media/TREKSTOR/videos/20070401 0140 - PREMIERE 3 - Ein Duke kommt selten allein (The Dukes of Hazzard).ts"}
    //loc:=MediaLocator{Filename:"../../../test.dvd"}
    //loc:=MediaLocator{Filename:"/Users/jholscher/Movies/39,90.avi.divx"}
    loc:=MediaLocator{Filename:"/home/HoelscJ/39,90.avi.divx"}
    source:=NewDataSource(loc)
    if(!source.Connect()){
    	log.Printf("cold not open file : %s", loc.Filename)
    }
    plex:=NewDemultiplexer(source)
    tracks:=*plex.GetTracks()
    for i:=0;i<len(tracks);i++ {
      go multiplex_encoder_test(&tracks[i])
    }
    plex.Start()
    //plex.Stop()
    source.Disconnect()
}

