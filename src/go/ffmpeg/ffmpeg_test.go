package gmf

import "testing"

//var filename string="/media/video/ChocolateFactory.ts"
var filename="../../../target/dependency/fixtures/testfile.flv"

func TestAVcodecAllocContext(t * testing.T){
    ctx:=avcodec_alloc_context()
    if(ctx==nil){
	t.Fatalf("error while allocating CodecContext")
    }
}

func TestAVCodecContextOptionSet(t*testing.T){
    ctx:=avcodec_alloc_context()
    if(ctx==nil){
	t.Fatalf("error while allocating CodecContext")
    }
    av_set_string(ctx,"flags","pass1")
}

func TestAVFormatAllocContext(t * testing.T){
    ctx:=avformat_alloc_context()
    if(ctx==nil){
	t.Fatalf("error while allocating FormatContext")
    }
}

func TestAVOpenInputFile(t*testing.T){
    ctx:=avformat_alloc_context()
    result:=av_open_input_file(ctx, filename,nil,0,nil)
    if(result!=0){
	t.Fatalf("error while openning input file")
    }
    av_close_input_file(ctx)
}

func TestAVFindStreamInfo(t*testing.T){
    ctx:=avformat_alloc_context()
    result:=av_open_input_file(ctx, filename,nil,0,nil)
    if(result!=0){
	t.Fatalf("error while openning input file")
    }
    
    result=av_find_stream_info(ctx)
    if(result<0){
	t.Fatalf("error while find stream info")
    }
    av_close_input_file(ctx)
}
func TestAVReadPacket(t*testing.T){
    ctx:=avformat_alloc_context()
    result:=av_open_input_file(ctx, filename,nil,0,nil)
    if(result!=0){
	t.Fatalf("error while openning input file")
    }
    
    result=av_find_stream_info(ctx)
    if(result<0){
	t.Fatalf("error while find stream info")
    }
    
    var packet * Packet=new(Packet)
    av_init_packet(packet)
    av_read_frame(ctx, packet)
    av_close_input_file(ctx)
}
func TestUrlFopen(t*testing.T){
    ctx:=avformat_alloc_context()
    result:=url_fopen(ctx,"test.mp4")
    if(result<0){
	t.Fatalf("could not open output file : %s")
    }

}
func TestUrlClose(t*testing.T){
    ctx:=avformat_alloc_context()
    result:=url_fopen(ctx,"test.mp4")
    if(result<0){
	t.Fatalf("could not open output file : %s")
    }
    url_fclose(ctx)
    av_free(ctx)
}

func TestSwsContext(t*testing.T){
    var ctx SwsContext
    sws_scale_getcontext(&ctx, 100,100,0,100,100,0,1)
}
func TestSwsScale(t*testing.T){
    var ctx SwsContext
    sws_scale_getcontext(&ctx, 100,100,0,100,100,0,1)
    var in, out * Frame=new(Frame),new(Frame)
    alloc_avframe(in)
    alloc_avframe(out)
    
    sws_scale(&ctx, in, out)
}

