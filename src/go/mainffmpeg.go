package main;

import . "ffmpeg"
import "gob"
import "os"
import "fmt"
import "flag"
import "time"
//import "io/ioutil"

type sometype struct {
    n    int
    name string
} 



func decoder(track * Track){
    decoder:=track.GetDecoder()
    decoder.Open()
    var p Packet
    for true {
	if(!track.ReadPacket(&p)){
	    println("stream end reached")
	    return
	}
	decoder.Decode(p)
    }
}

func main(){
    ///media/video/big_buck_bunny_480p_surround-fix.avi
    //ffmpeg.OpenMediaFile("/media/video/ChocolateFactory.ts")
//     var R ffmpeg.Rational=ffmpeg.NewRational(1,1)
    for i:=0;i<0;i++ {
     var R Rational=Rational{1,2}
     var R2 Rational=Rational{3,4}
     var R3 Rational=Rational{1,2}
     var R4 Rational=NewRational()
     R.Numerator=100
     R.Denominator=200
     println(R.Equals(R2))
     println(R.Equals(R3))
     println(R.Lower(R2))
     println(R.Greater(R2))
     println(R.Denominator)
     println(R4.Numerator)
     println(R4.Denominator)
     }
    
    for i:=0;i<1;i++ {
	filename:=flag.Arg(0)
	println(len(filename))
	if(len(filename)==0){
	    filename="/media/video/big_buck_bunny_480p_surround-fix.avi";
	}
	
        m:=MediaLocator{Filename:filename}
        //m:=ffmpeg.MediaLocator{Filename:"/media/video/ChocolateFactory.ts"}
        //println(m.GetProtocol())
        //println(m.GetReminder())
	d:=DataSource{Locator:m}
        d.Connect();
        dem:=NewDemultiplexer(d)
        var p Packet
        var i int=0
        var count int64=0
        tracks:=*dem.GetTracks()
        for i:=0;i<len(tracks);i++ {
    	    go decoder(&tracks[i])
        }
        dem.Start()
        println("waiting 5 sec. to complete")
        time.Sleep(5*1000000000)
	return 
        //decoder(&tracks[1])
        println(len(tracks))
	coder:=(tracks)[1].GetDecoder()
	coder.Open()
        for(dem.ReadPacket(&p)==true){
    	    i++
    	    count+=int64(p.Size)
//    	    println(len(p.Data))
//    	    println(p.Size)
	
	//some:=sometype{1,"bla"}
    	    if(&p==nil){
		println("nil packet reached")
		return
	    }
	if(p.Stream==1){
	    coder.Decode(p)
	    //println(frame.GetPts())
	}

	if(false){
	filename:=fmt.Sprintf("data/test.%d.data",i)
	file,err := os.Open(filename,os.O_RDWR|os.O_CREAT ,0755);
	e:=gob.NewEncoder(file)
	if(err!=nil){
	    println(err.String())
	}

	//println(p.Dts)
	error:=e.Encode(p)
	if(error!=nil){
	    println("error while encoding")
	    println(error.String())
	    return
	}
	}
    	//ioutil.WriteFile("test.data", p.Data,0755)

    	    if(i==5510){
    		//return
    	    }
    	    //println(p.Dts)
        }
        println(i)
        println(count)
        println(dem.GetDuration().String())
        dur:=dem.GetDuration()
        println(dur.String())
	println(dur.RescaleTo(Rational{1,25}).String())
        println(dem.GetTimestamp().String())
        println(i)
        d.Disconnect()
    }
}

