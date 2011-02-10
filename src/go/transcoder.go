package main

import . "gmf"
import "log"
import "time"
import "hive"
import "flag"
import "fmt"
import "os"
import "runtime"

func close_decoder(c * Decoder){
    c.Free()
}
func close_encoder(c * Encoder){
    c.Free()
}
//var encoder_map=make(map[int32]*Encoder)
func multiplex_encoder_test(track * Track, multiplexer * Multiplexer, preset * hive.Preset){
    var encoder * Encoder
    var resizer * Resizer
    var resampler * Resampler
    var rate_converter * FrameRateConverter

    decoder:=track.GetDecoder()
    decoder.SetParameter("request_channels","2")
    decoder.SetParameter("request_channel_layout","2")
    decoder.Open()
    
    
    for i:=0;i<len(preset.Codec);i++{
	if(preset.Codec[i].Type=="video"&&decoder.GetCodecType()==CODEC_TYPE_VIDEO){
	    encoder=hive.CreateEncoder(&preset.Codec[i])
	    encoder.Open()
	    resizer=new(Resizer)
	    resizer.Init(decoder, encoder)
	    rate_converter=new(FrameRateConverter)
	    rate_converter.Init(decoder.GetFrameRate(), encoder.GetFrameRate())
	    multiplexer.AddTrack(encoder)
	    break
	}
	if(preset.Codec[i].Type=="audio"&&decoder.GetCodecType()==CODEC_TYPE_AUDIO){
	    encoder=hive.CreateEncoder(&preset.Codec[i])
	    encoder.Open()
	    resampler=new(Resampler)
	    resampler.Init(decoder, encoder)
	    multiplexer.AddTrack(encoder)
	    break
	}
    }
    //runtime.SetFinalizer(decoder, close_decoder)
    if(encoder!=nil){
    runtime.SetFinalizer(encoder, close_encoder)
    }
  var bytecounter=0
  var p * Packet=new(Packet)
  for true {
    if(!track.ReadPacket(p)){
//	println("stream end reached")
	break
    }
    var frame * Frame
    frame=decoder.Decode(p)
    p.Free()
    if(frame!=nil&&frame.IsFinished()&&encoder!=nil){
	if(decoder.GetCodecType()==CODEC_TYPE_VIDEO){
	    frame=resizer.Resize(frame)
	    frame=rate_converter.Convert(frame)
	}else{
            if(decoder.GetCodecType()==CODEC_TYPE_AUDIO){
                frame=resampler.Resample(frame)
            }else{
                continue
            }
        }
        encoder.Encode(frame)
    }
  }
  print("bytes encoded = ")
  println(bytecounter)

  decoder.Close()
  if(encoder!=nil){
    encoder.Close()
  }
}
var presetfile *string = flag.String("p", "", "preset file name")
var inputfile *string = flag.String("i", "", "input file name")
var outfile *string = flag.String("o", "", "output file name")
var outformat *string = flag.String("f", "", "output file format")

func main(){
    flag.Parse()
    
    preset,err:=hive.LoadPreset(*presetfile)
    if(err!=nil){
	fmt.Printf("Preset %s\n", err)
	os.Exit(1)
    }
    if(inputfile==nil){
	fmt.Printf("no inputfile given")
	os.Exit(1)
    }
    if(outfile==nil){
	fmt.Printf("no outputfile given")
	os.Exit(1)
    }
    if true {
    loc:=MediaLocator{Filename:*inputfile}
    source:=NewDataSource(loc)
    if(!source.Connect()){
    	log.Printf("cold not open file : %s\n", loc.Filename)
	os.Exit(1)
    }
    plex:=NewDemultiplexer(source)
    var sink = NewDatasink(MediaLocator{Filename:*outfile,Format:*outformat})
    
    if(!sink.Connect()){
    	log.Printf("cold not open outputfile : %s\n", sink.Locator.Filename)
	os.Exit(1)
    }
    var multiplexer = NewMultiplexer(sink)
    tracks:=*plex.GetTracks()
    
    for i:=0;i<len(tracks);i++ {
      go multiplex_encoder_test(&tracks[i], multiplexer, preset)
    }
    
    time.Sleep(1000000000)
    go multiplexer.Start()
    plex.Start()
    time.Sleep(5000000000)
    multiplexer.Stop()
    //plex.Stop()
    source.Disconnect()
    sink.Disconnect()
    }
}
