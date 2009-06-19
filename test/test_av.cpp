




int main(int argc, char ** argv) {

  while (avpkt.size > 0 || (!pkt && ist->next_pts != ist->pts)) {

    static unsigned int samples_size = FFMAX(pkt->size * sizeof (*samples), AVCODEC_MAX_AUDIO_FRAME_SIZE);
    static short *samples = av_malloc(samples_size);

    ret = avcodec_decode_audio3(ist->st->codec, samples, &samples_size, &avpkt);

    avpkt.data += ret;
    avpkt.size -= ret;


    ist->next_pts += ((int64_t) AV_TIME_BASE / 2 * data_size) / (ist->st->codec->sample_rate * ist->st->codec->channels);


    uint8_t *buftmp;
    int isize = av_get_bits_per_sample_format(dec->sample_fmt) / 8;
    int osize = av_get_bits_per_sample_format(enc->sample_fmt) / 8;

    size_out = audio_resample(ost->resample, (short *) buftmp, (short *) samples, samples_size / (ist->st->codec->channels * isize));
    size_out = size_out * enc->channels * osize;

    if (av_fifo_realloc2(ost->fifo, av_fifo_size(ost->fifo) + size_out) < 0) {
      fprintf(stderr, "av_fifo_realloc2() failed\n");
      av_exit(1);
    }

    av_fifo_generic_write(ost->fifo, buftmp, size_out, NULL);

    frame_bytes = enc->frame_size * osize * enc->channels;

    const int audio_out_size= 4*MAX_AUDIO_PACKET_SIZE;

    if (!audio_buf)
      audio_buf = av_malloc(2 * MAX_AUDIO_PACKET_SIZE);
    if (!audio_out)
      audio_out = av_malloc(audio_out_size);

    while (av_fifo_size(ost->fifo) >= frame_bytes) {
      AVPacket pkt;
      av_init_packet(&pkt);

      av_fifo_generic_read(ost->fifo, audio_buf, frame_bytes, NULL);

      //FIXME pass ost->sync_opts as AVFrame.pts in avcodec_encode_audio()

      ret = avcodec_encode_audio(enc, audio_out, audio_out_size,(short *) audio_buf);
      if (ret < 0) {
        fprintf(stderr, "Audio encoding failed\n");
        av_exit(1);
      }
      audio_size += ret;
      pkt.stream_index = ost->index;
      pkt.data = audio_out;
      pkt.size = ret;
      if (enc->coded_frame && enc->coded_frame->pts != AV_NOPTS_VALUE)
        pkt.pts = av_rescale_q(enc->coded_frame->pts, enc->time_base, ost->st->time_base);
      pkt.flags |= PKT_FLAG_KEY;
      write_frame(s, &pkt, ost->st->codec, bitstream_filters[ost->file_index][pkt.stream_index]);

      ost->sync_opts += enc->frame_size;
    }



  }


}

