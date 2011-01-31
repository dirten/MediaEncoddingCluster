package gmf


import "testing"
import "fmt"
import "os"
import "unsafe"
//import "number"
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
	 if(decoder.Ctx.ctx.codec_type==CODEC_TYPE_VIDEO){

	    ppsWriter(frame)
	}
    }
  }
}

func TestDecoderTracks(t*testing.T){
    println("starting decoder test")
    //loc:=MediaLocator{"/media/video/ChocolateFactory.ts"}
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
var number int=0

func ppsWriter(frame * Frame){
   header:=fmt.Sprintf("P5\n%d %d\n255\n",frame.width,frame.height)
   
   file, err := os.Open("test.ppm", os.O_CREATE|os.O_TRUNC|os.O_RDWR, 0666)
   
   if(err!=nil){
    println("error while openning file")
   }
   file.WriteString(header)
   //println(frame.avframe)
   linesize:=int(frame.avframe.linesize[0])
   //return 

   //println(linesize)
   data:=make([]byte, frame.width)
   tmpdata:=(*(*[1<<30]byte)(unsafe.Pointer(frame.avframe.data[0])))[0:frame.size]
   //    for i:= 0; i < 1; i++ {
   //      data[i] = tmpdata[i];
   //    }
   
   for i := 0; i < int(frame.height); i++{
   for a:= 0; a < int(frame.width); a++ {
   data[a] = tmpdata[(i*linesize)+a];
   }
   file.Write(data);
   }
   
   file.Close()
    if(number>1000){
    os.Exit(1)
    }
    number++
}
