package gmf

import "testing"

var filename string="/media/video/ChocolateFactory.ts"
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
    if(result!=0){
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
    if(result!=0){
	t.Fatalf("error while find stream info")
    }
    
    var packet * Packet=new(Packet)
    av_init_packet(packet)
    av_read_frame(ctx, packet)
    av_close_input_file(ctx)
}

