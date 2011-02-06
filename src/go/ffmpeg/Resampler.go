package gmf
import "log"
type Resampler struct{
    ctx * ResampleContext
    isize int
    osize int
    outbuffer []byte
    channels int
    sample_rate int
}

func(self*Resampler)Init(dec * Decoder, enc * Encoder){
    self.ctx=new(ResampleContext)
    self.ctx=av_audio_resample_init(
            int(enc.Ctx.ctx.channels),
            int(dec.Ctx.ctx.request_channels),
            int(enc.Ctx.ctx.sample_rate),
            int(dec.Ctx.ctx.sample_rate),
            int(enc.Ctx.ctx.sample_fmt),
            int(dec.Ctx.ctx.sample_fmt))
            if(self.ctx.ctx==nil){
                println("Could not create resample context!!!!!!!!!!!!!!")
            }
    self.isize=av_get_bits_per_sample_fmt(dec.Ctx.ctx.sample_fmt)/8
    self.osize=av_get_bits_per_sample_fmt(enc.Ctx.ctx.sample_fmt)/8
    self.outbuffer=make([]byte,(2 * 128 * 1024))
    self.channels=int(enc.Ctx.ctx.channels)
    self.sample_rate=int(enc.Ctx.ctx.sample_rate)
    log.Printf("ResampleContext inch:%d;outch:%d;inrate:%d;outrate:%d;infmt:%d,outfmt:%d",int(dec.Ctx.ctx.request_channels),int(enc.Ctx.ctx.channels),int(dec.Ctx.ctx.sample_rate),int(enc.Ctx.ctx.sample_rate),int(dec.Ctx.ctx.sample_fmt),int(enc.Ctx.ctx.sample_fmt))
}

func(self*Resampler)Resample(f * Frame)*Frame{
    //println("inframe"+f.String())
    
    frame:=new(Frame)
    out_size := audio_resample(self.ctx, self.outbuffer, f.buffer, (f.size / (self.channels * self.isize)))
    frame.buffer=self.outbuffer
    //frame.Stream=f.Stream
    frame.size=out_size*self.channels*self.osize
    frame.Duration=Timestamp{int64(out_size),Rational{1,self.sample_rate}}
    //println("outframe"+frame.String())
    return f
            /*
    int isize = av_get_bits_per_sample_fmt(_dec->getSampleFormat()) / 8;
        int osize = av_get_bits_per_sample_fmt(_enc->getSampleFormat()) / 8;
        uint8_t * audio_buf = (uint8_t*) av_malloc(2 * MAX_AUDIO_PACKET_SIZE);

        int out_size = audio_resample(_audioCtx, (short *) audio_buf, (short *) in_frame._buffer, in_frame._size / (in_frame.channels * isize));
        out_frame._allocated = true;
        out_frame._buffer = audio_buf;
        out_frame.pos = in_frame.pos;
        out_frame.stream_index = in_frame.stream_index;
        out_frame._size = out_size * _enc->getChannels() * osize;
        out_frame._type = CODEC_TYPE_AUDIO;
        out_frame.channels = _enc->getChannels();
        out_frame.sample_rate = _enc->getSampleRate();
        out_frame.setDuration(out_size);
*/
    return frame
}