package main;

import . "gmf"
import "gob"
import "os"
import "fmt"
import "flag"
import "time"
//import "io/ioutil"





func main(){
    /*openning input file*/
    ctx:=avformat_alloc_context()
    result:=av_open_input_file(ctx, filename,nil,0,nil)
    if(result!=0){
	t.Fatalf("error while openning input file")
    }
    
    result=av_find_stream_info(ctx)
    if(result<0){
	t.Fatalf("error while find stream info")
    }

    /*openning the outputfile*/
    outformat:="flv"
    outfile:="test."+outformat
    outctx:=avformat_alloc_context();
    format:=av_guess_format(outformat,outfile)
    outctx.ctx.oformat=(*_Ctype_struct_AVOutputFormat)(unsafe.Pointer(format.format))

    result=url_fopen(outctx, outfile);
    if(result<0){
	t.Fatalf("error while open outputfile")
    }
    /*add new output stream*/
    stream:=av_new_stream(outctx,0)
    decoder:=ctx.ctx.streams[0]
     coder:=Decoder{Coder{Ctx:CodecContext{ctx:ctx.ctx.streams[0].codec}}}
     
    //coder.Ctx.ctx=self.codec//dpx.Ds.Ctx.streams[streamid].codec

    var encoder Encoder
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

    stream.codec=encoder.Ctx.ctx
    //stream.codec=decoder.codec
    println(decoder.codec)


    result=av_write_header(outctx)
    if(result<0){
	t.Fatalf("error while write output header")
    }
    
    var packet * Packet=new(Packet)
    av_init_packet(packet)
    for(av_read_frame(ctx, packet)>=0){
	println(packet.avpacket.pts)
	if(packet.avpacket.stream_index==0){
	    frame:=coder.Decode(*packet)
	    encoder.Encode(*frame)
	    //av_interleaved_write_frame(outctx,outp)
	}
    }
    av_close_input_file(ctx)
    
    url_fclose(outctx);
    av_free(outctx)
    //av_free(ctx)
}

