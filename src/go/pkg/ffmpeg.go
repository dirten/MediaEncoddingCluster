package ffmpeg


//#include "libavcodec/avcodec.h"
//#include "libavcodec/opt.h"
//#include "libavformat/avformat.h"
import "C"
import "unsafe"
import "sync"
//import "os"
import "strings"
import "math"
import "strconv"

func init(){
  C.avcodec_register_all();
  C.av_register_all();
  C.av_log_set_level(C.AV_LOG_VERBOSE);
}

//export OpenMediaFile
func OpenMediaFile(filename string){
  println("try openning the media file ")
  println(filename)
  cfilename:=C.CString(filename);
  
  C.avcodec_register_all();
  C.av_register_all();
  ctx:=C.avformat_alloc_context();
  
  result:=C.av_open_input_file(&ctx, cfilename, nil,0,nil);
  println(result)
  result=C.av_find_stream_info(ctx)
  C.free(unsafe.Pointer(cfilename));
  C.av_close_input_file(ctx);
  //C.free(unsafe.Pointer(ctx));
}


type MediaLocator struct{
  Filename string;
  Protocol string;
  Reminder string;
  
}

func (loc * MediaLocator) GetProtocol() string{
  lines:=strings.Split(loc.Filename,":",-1)
  if(len(lines)!=2){
    return "file"
  }
  return lines[0]
}

func (loc * MediaLocator) GetReminder()string{
  lines:=strings.Split(loc.Filename,":",-1)
  if(len(lines)!=2){
    return loc.Filename
  }
  return lines[1][2:]
}

type DataSource struct{
  Locator MediaLocator;
  Ctx *C.AVFormatContext
  //Parameter * C.AVFormatParameters 
  Valid bool
}

func (src * DataSource) Connect() bool{
  src.Valid=false
  cfilename:=C.CString(src.Locator.Filename);
  
  src.Ctx=C.avformat_alloc_context();
  
  result:=C.av_open_input_file(&src.Ctx, cfilename, nil,0,nil);
  C.free(unsafe.Pointer(cfilename))
  
  if(result!=0){
    return src.Valid
  }
  result=C.av_find_stream_info(src.Ctx)
  if(result<0){
    return src.Valid
  }
  src.Valid=true
  return src.Valid;
}

func (src * DataSource) Disconnect() bool{
  if(src.Valid){
    C.av_close_input_file(src.Ctx);
  }
  return true;
}

func (src * DataSource) GetContentType() string{
  return "video/ffmpeg";
}


type Packet struct{
  Pts int64
  Dts int64
  Data [] byte
  Size int
  Stream int
  Flags int
  Duration int
  Pos int64
}
type Frame struct{
  C.AVFrame
}

func(self * Frame)GetPts()int64{
  return int64(self.pts)
}	

type Demultiplexer struct{
  Ds DataSource;
  Packet * C.AVPacket
  tracks *[]Track
}

type Track struct{
  *C.AVStream
  stream chan Packet
}

type Format struct{
  
}

func (self * Track)GetFormat()Format{
  return Format{}
}

func (self * Track)GetStreamId()int{
  return int(self.index)
}

func (self * Track)GetStartTime()Timestamp{
  return Timestamp{}
}

func (self * Track)ReadPacket(p * Packet)bool{
  *p=<-self.stream
  if(closed(self.stream)){
    return false;
  }
  return true
}

func (self * Track)GetDecoder()Decoder{
  coder:=Decoder{}//NewCoder()
  coder.Ctx=self.codec//dpx.Ds.Ctx.streams[streamid].codec
  return coder
}

func NewDemultiplexer(d DataSource)*Demultiplexer{
  return &Demultiplexer{d,nil,nil}
}

func (dpx * Demultiplexer)GetTracks()*[]Track{
  scount:=dpx.Ds.Ctx.nb_streams
  var result []Track=make([]Track,int(scount))
  for i:=0;i<int(scount);i++ {
    result[i]=Track{dpx.Ds.Ctx.streams[i], make(chan Packet)}
  }
  dpx.tracks=&result;
  return &result
}

func (dpx * Demultiplexer)GetDecoder(streamid int)Decoder{
  coder:=Decoder{}//NewCoder()
  coder.Ctx=dpx.Ds.Ctx.streams[streamid].codec
  return coder
}

func (dpx * Demultiplexer)GetDuration() Duration {
  return Duration{Timestamp{int64(dpx.Ds.Ctx.duration),Rational{1,1000000}}}
}

func (dpx * Demultiplexer)GetTimestamp() Timestamp{
  return Timestamp{int64(dpx.Ds.Ctx.timestamp),Rational{1,1000000}}
}

func (dpx * Demultiplexer)Start(){
  for true {
    dpx.Packet=new(C.AVPacket)
    C.av_init_packet(dpx.Packet)
    
    if(C.av_read_frame(dpx.Ds.Ctx, dpx.Packet)>=0){
      C.av_dup_packet(dpx.Packet)
    }else{
      println("end of file reached, closing channels")
      for i:=0;i<len(*dpx.tracks);i++ {
        print("closing channel ")
        println(i)
        close((*dpx.tracks)[i].stream)
      }
      break
    }
    var re Packet
    
    re.Pts=int64(dpx.Packet.pts)
    re.Dts=int64(dpx.Packet.dts)
    re.Size=int(dpx.Packet.size)
    re.Data=make([]byte, re.Size)
    data:=(*(*[1<<30]byte)(unsafe.Pointer(dpx.Packet.data)))[0:dpx.Packet.size]
    for i:= 0; i < re.Size; i++ {
      re.Data[i] = data[i];
    }
    re.Stream=int(dpx.Packet.stream_index)
    re.Flags=int(dpx.Packet.flags)
    re.Duration=int(dpx.Packet.duration)
    re.Pos=int64(dpx.Packet.pos)
    C.av_free_packet(dpx.Packet)
    (*dpx.tracks)[re.Stream].stream<-re
  }
  //re * Packet
}

func (dpx * Demultiplexer)ReadPacket(re * Packet) bool{
  dpx.Packet=new(C.AVPacket)
  
  C.av_init_packet(dpx.Packet)
  defer C.av_free_packet(dpx.Packet)
  if(C.av_read_frame(dpx.Ds.Ctx, dpx.Packet)>=0){
    C.av_dup_packet(dpx.Packet)
  }else{
    return false
  }
  re.Pts=int64(dpx.Packet.pts)
  re.Dts=int64(dpx.Packet.dts)
  re.Size=int(dpx.Packet.size)
  re.Data=make([]byte, re.Size)
  data:=(*(*[1<<30]byte)(unsafe.Pointer(dpx.Packet.data)))[0:dpx.Packet.size]
  for i:= 0; i < re.Size; i++ {
    re.Data[i] = data[i];
  }
  re.Stream=int(dpx.Packet.stream_index)
  re.Flags=int(dpx.Packet.flags)
  re.Duration=int(dpx.Packet.duration)
  re.Pos=int64(dpx.Packet.pos)
  return true
}

type Coder struct {
  Parameter map[string]string
  Ctx *C.AVCodecContext
  Codec *C.AVCodec
  Valid bool
  
}
func NewCoder()Coder{
  return Coder{Parameter:make(map[string]string),Ctx:nil,Valid:false}
}
/*
 func NewCoder(Parameter map[string]string){
 
 }*/

func(c * Coder)Init(){
  
}

func (self * Coder)SetParameter(key, val string){
  self.Parameter[key]=val
}

func (self * Coder)GetParameter(key string)string{
  return self.Parameter[key]
}

func (self * Coder)GetParameters()map[string]string{
  return self.Parameter
}

var avcodec_mutex sync.Mutex

func(c * Coder)Open(){
  c.prepare()
  c.Codec=C.avcodec_find_decoder(c.Ctx.codec_id)
  if(c.Codec==nil){
    println("could not find Codec")
    return
  }
  avcodec_mutex.Lock()
  res:=C.avcodec_open(c.Ctx,c.Codec)
  avcodec_mutex.Unlock()
  
  if(res<0){
    println("error openning codec")
    return
  }
  println("codec openned")
  c.Valid=true
}

func(self* Coder)prepare(){
  if(self.Ctx==nil){
    self.Ctx=C.avcodec_alloc_context();
    cid,_:=strconv.Atoui64(self.Parameter["codecid"])
    self.Ctx.codec_id=uint32(cid)
  }
  /**
  * @TODO: settng the fixed params like width, height, channels...
  */
}
func(self*Coder)setCodecParams(){
  for mkey,mval:=range self.Parameter {
    var o * C.AVOption=nil
    key:=C.CString(mkey)
    val:=C.CString(mval)
    defer C.free(unsafe.Pointer(key))
    defer C.free(unsafe.Pointer(val))
    C.av_set_string3(unsafe.Pointer(self.Ctx), key, val, 1, &o);
  }
}

type Decoder struct{
  Coder
}

type Encoder struct{
  Coder
}

func(c * Decoder)Decode(p Packet)*Frame{
  if(c.Ctx.codec_type==C.CODEC_TYPE_VIDEO){
    return c.decodeVideo(p)
  }
  if(c.Ctx.codec_type==C.CODEC_TYPE_AUDIO){
    return c.decodeAudio(p)
  }
  return nil
}

func(c * Decoder)decodeAudio(p Packet)*Frame{
  packet:=new(C.AVPacket)
  C.av_init_packet(packet)
  defer C.av_free_packet(packet)
  
  packet.pts=C.int64_t(p.Pts)
  packet.dts=C.int64_t(p.Dts)
  packet.size=C.int(p.Size)
  packet.data=(*C.uint8_t)(unsafe.Pointer(&p.Data[0]))
  packet.stream_index=C.int(p.Stream)
  packet.flags=C.int(p.Flags)
  packet.duration=C.int(p.Duration)
  packet.pos=C.int64_t(p.Pos)
  //size:=packet.size;
  samples_size:= C.int(C.AVCODEC_MAX_AUDIO_FRAME_SIZE);
  //bps := C.av_get_bits_per_sample_fmt(c.Ctx.sample_fmt) >> 3;
  
  outbuf := (*C.uint8_t) (C.av_malloc(C.uint(samples_size)));
  defer C.av_free(unsafe.Pointer(outbuf))
  C.avcodec_decode_audio3(c.Ctx,  (*C.int16_t)(unsafe.Pointer(outbuf)), &samples_size, packet)
  //println(data_len)
  return nil
}

func(c * Decoder)decodeVideo(p Packet)*Frame{
  if(!c.Valid){
    //println("try to decode with an invalid Decoder")
    return nil
  }
  //println("try to decode with a valid Decoder")
  
  packet:=new(C.AVPacket)
  C.av_init_packet(packet)
  defer C.av_free_packet(packet)
  
  packet.pts=C.int64_t(p.Pts)
  packet.dts=C.int64_t(p.Dts)
  packet.size=C.int(p.Size)
  //packet.data=&C.uint8_t(make([]byte, p.Size)[0])
  //myBytes:=make([]byte,p.Size);
  /*    
   var pb *byte
   if(len(p.Data)>0){
   //println("Data > 0")
   pb=&p.Data[0]
   }*/
  packet.data=(*C.uint8_t)(unsafe.Pointer(&p.Data[0]))
  //    println(p.Data)
  //    println(pb)
  //    println(packet.data)
  //    packet.data=unsafe.Pointer(p.Data)
  //    for i:= 0; i < re.Size; i++ {
  //      re.Data[i] = data[i];
  //    }
  packet.stream_index=C.int(p.Stream)
  packet.flags=C.int(p.Flags)
  packet.duration=C.int(p.Duration)
  packet.pos=C.int64_t(p.Pos)
  
  height:=c.Ctx.height
  width:=c.Ctx.width
  var frame * Frame=new(Frame)
  
  numBytes:= C.avpicture_get_size(0, width, height)
  //_size = numBytes;
  //_buffer := (*C.uint8_t) (C.av_malloc(C.uint(numBytes)));
  if(numBytes>0){
    var buffer []byte=make([]byte,numBytes)
    var pbuffer * byte=&buffer[0]
    // C.memset((*C.uint8_t)(_buffer), 0, C.size_t(numBytes));
    // Assign appropriate parts of buffer to image planes
    //var picture * C.AVPicture=(*C.AVPicture)(unsafe.Pointer(frame))
    //println(frame)
    //println(picture)
    //:=&frame
    //854x480
    C.avpicture_fill((*C.AVPicture)(unsafe.Pointer(frame)), (*C.uint8_t)(unsafe.Pointer(pbuffer)), 0, width, height);
  }
  
  
  //frameFinished:=C.int(0)
  var frameFinished *C.int=new(C.int);
  //bytes_decoded:=
  
  C.avcodec_decode_video2(c.Ctx,(*C.AVFrame)(unsafe.Pointer(frame)),frameFinished,packet)
  frame.pts=packet.pts
  //frame.dts=packet.dts
  
  /*
   //C.avcodec_decode_video2(nil,nil,nil,nil)
   
   header:=fmt.Sprintf("P5\n%d %d\n255\n",width,height)
   
   file, err := os.Open("test.ppm", os.O_CREATE|os.O_TRUNC|os.O_RDWR, 0666)
   
   if(err!=nil){
   println("error while openning file")
   }
   file.WriteString(header)
   linesize:=int(frame.linesize[0])
   //println(linesize)
   data:=make([]byte, width)
   tmpdata:=(*(*[1<<30]byte)(unsafe.Pointer(frame.data[0])))[0:numBytes]
   //    for i:= 0; i < 1; i++ {
   //      data[i] = tmpdata[i];
   //    }
   
   for i := 0; i < int(height); i++{
   for a:= 0; a < int(width); a++ {
   data[a] = tmpdata[(i*linesize)+a];
   }
   file.Write(data);
   }
   
   file.Close()*/
  //ioutil.WriteFile("test.data", header,0755)
  //println("bla fasel")
  //println(bytes_decoded)
  //println(*frameFinished)
  return frame
}

func(self * Encoder)Encode(frame * Frame)*[]Packet{
  if(self.Ctx.codec_type==C.CODEC_TYPE_VIDEO){
    return self.encodeVideo(frame)
  }
  if(self.Ctx.codec_type==C.CODEC_TYPE_AUDIO){
    return self.encodeAudio(frame)
  }
  return nil
  
}

func(self * Encoder)encodeVideo(frame * Frame)*[]Packet{
  var result *[]Packet
  size := self.Ctx.width * self.Ctx.height;
  buffer_size := C.int(math.Fmax(float64(1024 * 256), float64(6 * size + 200)));
  var buffer []byte=make([]byte,int(buffer_size))
  var pbuffer * byte=&buffer[0]
  println(pbuffer)
  ret := C.avcodec_encode_video(self.Ctx, (*C.uint8_t) (unsafe.Pointer(pbuffer)), buffer_size, (*C.AVFrame)(unsafe.Pointer(frame)));
  println(ret)
  return result
}

func(self * Encoder)encodeAudio(frame * Frame)*[]Packet{
  var result *[]Packet
  
  return result
  
}
func(c * Coder)Close(){
  
}

