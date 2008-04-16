/******************************************************************************
* FOBS C++ wrapper code 
* Copyright (c) 2004 Omnividea Multimedia S.L
* Coded by Jos San Pedro Wandelmer
*
*    This file is part of FOBS.
*
*    FOBS is free software; you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation; either version 2.1 
*    of the License, or (at your option) any later version.
*
*    FOBS is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with FOBS; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

#include "Encoder.h"
#include <iostream>
extern "C"
{
#include "avformat.h"
#include "avcodec.h"
//#include "framehook.h"
}

using namespace omnividea::fobs;
using namespace std;
static Decoder iDecoder("");

const int Encoder::videoBufferSize = 1024*1024;
#define MAX_AUDIO_PACKET_SIZE (128 * 1024)


ReturnCode Encoder::testOpen()
{
   ReturnCode error = opened?OkCode:NotInitializedError;
   return error;
}
ReturnCode Encoder::testClose()
{
   ReturnCode error = (!opened)?OkCode:AlreadyInitializedError;
   return error;
}

int Encoder::getWidth()
{
   return width;
}
int Encoder::getHeight()
{
   return height;
}
double Encoder::getBitRate()
{
   return bitRate;
}
double Encoder::getFrameRate()
{
   return frameRate;
}


ReturnCode Encoder::chooseCodec()
{
   return chooseVideoCodec("mpeg1video");
}
ReturnCode Encoder::chooseFormat()
{
  return chooseFormat("mpeg");
}


ReturnCode Encoder::chooseVideoCodec(char *name)
{
   ReturnCode error = CodecUnsupportedError;
   vcodec = first_avcodec;
   while (vcodec) 
   {
      if (!strcmp(vcodec->name, name) && vcodec->type == CODEC_TYPE_VIDEO)
      {
         error = OkCode;
         break;
      }
      vcodec = vcodec->next;
   }
   return error;
}
ReturnCode Encoder::chooseAudioCodec(char *name)
{
   ReturnCode error = CodecUnsupportedError;
   acodec = first_avcodec;
   while (acodec) 
   {
      if (!strcmp(acodec->name, name) && acodec->type == CODEC_TYPE_AUDIO)
      {
         error = OkCode;
         break;
      }
      acodec = acodec->next;
   }
   return error;
}

/*
ReturnCode Encoder::chooseCodec(Decoder *d)
{

}*/
ReturnCode Encoder::chooseFormat(char *name)
{
   ReturnCode error = FormatUnsupportedError;
   outputFileFormat = guess_format(name,NULL, NULL);
   if(outputFileFormat != NULL) error = OkCode;
   return error;
}

Encoder::Encoder(const char *filename)
{
   pictureToEncodeBuf = NULL;
   transitionPictureBuf = NULL;
   img_resample_ctx = NULL;
   opened = false;
   videoBuffer = new uint8_t[videoBufferSize];
   this->filename = strdup(filename);
   encodeAudio = encodeVideo = 0;
   
   //From static members
   transitionPicture = new AVPicture();
   pictureToEncode = new AVPicture();
}

Encoder::~Encoder()
{
   close();
   delete [] videoBuffer;
   delete transitionPicture;
   delete pictureToEncode;
}

ReturnCode Encoder::setVideoParameters(int width, int height, double bitRate, double frameRate, char *vcodec)
{
   ReturnCode error = OkCode;
   this->width = width;
   this->height = height;
   this->bitRate = bitRate;
   this->frameRate = frameRate;
   vcodecFrameRateBase = 1001000;
   if(vcodec != NULL)
   {
     error = chooseVideoCodec(vcodec);
     vcodecId = (CodecID)this->vcodec->id;
     vcodecType = this->vcodec->type;
     vcodecPixelFormat = PIX_FMT_RGB24;
     encodeVideo = 1;
   }
   else error = CodecUnsupportedError;
   iVideoCodec = NULL;
   return error;
}

ReturnCode Encoder::setVideoParameters(Decoder *d)
{
   ReturnCode error = OkCode;
   if(d == NULL) return GenericError;
   AVCodecContext *icodec = d->getVideoCodec();
   if(icodec == NULL) return OkCode;
   
   vcodecId = icodec->codec_id;
   vcodecType = icodec->codec_type;
   vcodecTag = icodec->codec_tag;
   //vcodecFrameRateBase = icodec->time_base.num;
   vcodecFrameRateBase = d->inputFile->streams[d->videoStreamIndex]->r_frame_rate.den;
   vcodecPixelFormat = icodec->pix_fmt;

   this->width = icodec->width;
   this->height = icodec->height;
   this->bitRate = icodec->bit_rate/1000.0;
   this->frameRate = (double)d->inputFile->streams[d->videoStreamIndex]->r_frame_rate.num/(double)vcodecFrameRateBase;

   encodeVideo = -1;
   
   videoStream_time_base_num = d->inputFile->streams[d->videoStreamIndex]->time_base.num;
   videoStream_time_base_den = d->inputFile->streams[d->videoStreamIndex]->time_base.den;

   iVideoCodec = icodec;      
   
   start_time = d->inputFile->start_time;
   
   return error;
}

ReturnCode Encoder::setAudioParameters(int sampleRate, int channels, double bitrate, char *acodec)
{
   ReturnCode error = OkCode;
   acodecBitRate = bitrate;
   acodecSampleRate = sampleRate;
   acodecChannels = channels;
   acodecFrameSize = 1;
   if(acodec != NULL)
   {
    ReturnCode error = chooseAudioCodec(acodec);
    acodecId = (CodecID)this->acodec->id;
    acodecType = this->acodec->type;
    //acodecTag = this->acodec->tag;
    encodeAudio = 1;
   }
   else encodeAudio = -1;
   iAudioCodec = NULL;      
   if(encodeAudio == -1) error = CodecUnsupportedError;
   
   return error;
}

ReturnCode Encoder::setAudioParameters(Decoder *d)
{
   ReturnCode error = OkCode;
   if(d == NULL) return GenericError;
   AVCodecContext *icodec = d->getAudioCodec();
   if(icodec == NULL) return OkCode;
   
   acodecId = icodec->codec_id;
   acodecType = icodec->codec_type;
   acodecTag = icodec->codec_tag;
   acodecBitRate = icodec->bit_rate/1000.0;

   acodecSampleRate = icodec->sample_rate;
   acodecChannels = icodec->channels;
   acodecFrameSize = icodec->frame_size;

   encodeAudio = -1;

   audioStream_time_base_num = d->inputFile->streams[d->audioStreamIndex]->time_base.num;
   audioStream_time_base_den = d->inputFile->streams[d->audioStreamIndex]->time_base.den;

   iAudioCodec = icodec;      
   
   start_time = d->inputFile->start_time;
      
   return error;

}

ReturnCode Encoder::open()
{
   ReturnCode error = testClose();
   AVCodecContext *video_enc;
   AVCodecContext *audio_enc;

   
   if(isOk(error))
   {
      //outputFile = (AVFormatContext*)av_mallocz(sizeof(AVFormatContext));
      outputFile = av_alloc_format_context();
      if(outputFile == NULL)
      {
         error = MemoryError;
      }
      else
      {
        if(outputFileFormat != NULL) 
        {   
            outputFile->oformat = outputFileFormat;
            outputFile->timestamp = 0;
        }
        else error = FormatUnsupportedError;
      }
   }
   if(isOk(error))
   {
    if(!(encodeVideo || encodeAudio)) error = NoStreamsError;
   }
   
   if(isOk(error) && encodeVideo)
   {
     videoStream = av_new_stream(outputFile, 0);
     if(videoStream == NULL)
     {
        error = MemoryError;
     }

   }
   if(isOk(error) && encodeAudio)
   {
     audioStream = av_new_stream(outputFile, encodeVideo?1:0);
     if(audioStream == NULL)
     {
        error = MemoryError;
     }
   }
   

   if(isOk(error))
   {
      

      if(encodeVideo)
      {
        if(encodeVideo > 0) {
            videoStream->time_base.num = 1;
            videoStream->time_base.den = 90000;
        } else {
            //videoStream->time_base.num = videoStream_time_base_num;
            //videoStream->time_base.den = videoStream_time_base_den;
            videoStream->codec->extradata = iVideoCodec->extradata;
            videoStream->codec->extradata_size = iVideoCodec->extradata_size;
            videoStream->codec->time_base = iVideoCodec->time_base;
            videoStream->codec->has_b_frames = iVideoCodec->has_b_frames;
        }
        
        video_enc = videoStream->codec;

        video_enc->codec_id = (CodecID)vcodecId;
        video_enc->codec_tag = vcodecTag;
        AVCodec *codec;	
        codec = avcodec_find_encoder((CodecID)vcodecId);
        video_enc->bit_rate = (int)(getBitRate()*1000);
        video_enc->bit_rate_tolerance = 4000*1000;
        
        //If codec has a limited set of allowed framerates, choose the most similar
        if(encodeVideo > 0)
        {
            video_enc->time_base.den = (int)(getFrameRate()*vcodecFrameRateBase); //videoCodec_time_base_den;
            video_enc->time_base.num = vcodecFrameRateBase;//videoCodec_time_base_num;

            if(codec && codec->supported_framerates)
            {
                const AVRational *p= codec->supported_framerates;
                AVRational req= (AVRational){video_enc->time_base.den, video_enc->time_base.num};
                const AVRational *best=NULL;
                AVRational best_error= (AVRational){INT_MAX, 1};
                for(; p->den!=0; p++){
                    AVRational error= av_sub_q(req, *p);
                    if(error.num <0) error.num *= -1;
                    if(av_cmp_q(error, best_error) < 0){
                        best_error= error;
                        best= p;
                    }
                }
                video_enc->time_base.den= best->num;
                video_enc->time_base.num= best->den;
            }

            video_enc->pix_fmt = (enum PixelFormat)vcodecPixelFormat;
            //If codec has a limited set of allowed formats, make sure one of them is chosen
            if(codec && codec->pix_fmts)
            {
                const enum PixelFormat *p= codec->pix_fmts;
                for(; *p!=-1; p++){
                    if(*p == video_enc->pix_fmt)
                        break;
                }
                if(*p == -1)
                    video_enc->pix_fmt = codec->pix_fmts[0];
            }
        }
        video_enc->width = getWidth();
        video_enc->height = getHeight();
        
        //video_enc->gop_size = 0;
        video_enc->thread_count = 1;
      }

      if(encodeAudio)
      {
        if(encodeVideo > 0) {
            audioStream->time_base.num = 1;
            audioStream->time_base.den = 90000;
        } else {
            //audioStream->time_base.num = audioStream_time_base_num;
            //audioStream->time_base.den = audioStream_time_base_den;
            audioStream->codec->extradata = iAudioCodec->extradata;
            audioStream->codec->extradata_size = iAudioCodec->extradata_size;
            //audioStream->codec->block_align = iAudioCodec->block_align;
        }

        audio_enc = audioStream->codec;

        audio_enc->codec_id = (enum CodecID)acodecId;
        audio_enc->codec_tag = acodecTag;
        audio_enc->codec_type = CODEC_TYPE_AUDIO;        
        audio_enc->strict_std_compliance = 0;

        audio_enc->bit_rate = (int)(acodecBitRate*1000);
        audio_enc->sample_rate = acodecSampleRate;
        audio_enc->channels = acodecChannels;
        audio_enc->frame_size = acodecFrameSize;
        audio_enc->thread_count = 1;
      
      }

   }
   if(isOk(error))
   {
      outputFile->nb_streams = (encodeVideo?1:0) + (encodeAudio?1:0);
      if(encodeVideo)
      {
        outputFile->streams[0] = videoStream;
        videoStreamIndex = 0;
      }
      else
      {
        videoStream = NULL;
        videoStreamIndex = -1;
      }
      if(encodeAudio)
      {
        audioStreamIndex = outputFile->nb_streams-1;
        outputFile->streams[audioStreamIndex] = audioStream;
      }
      else
      {
        audioStream = NULL;
        audioStreamIndex = -1;
      }

      strcpy(outputFile->filename, filename);
      if (!(outputFile->oformat->flags & AVFMT_NOFILE)) 
      {
         /* open the file */
         if (url_fopen(&outputFile->pb, filename, URL_WRONLY) < 0) 
         {
            error = FileOpenError;
         }
      }
   }
   
   //Set output format parameters
   if(isOk(error))
   {
    AVFormatParameters params, *ap = &params;
    memset(ap, 0, sizeof(*ap));
    //ap->time_base.den = 1000;
    //ap->time_base.num = (int)(getFrameRate()*1000);
    
    //ap->image_format = NULL;//::first_image_format;
    if (av_set_parameters(outputFile, ap) < 0) {
        error = FileOpenError;
    }

    outputFile->packet_size= 0;
    outputFile->mux_rate= 0;
    outputFile->preload= (int)(0.5*AV_TIME_BASE);
    outputFile->max_delay= (int)(0.7*AV_TIME_BASE);
    outputFile->loop_output = AVFMT_NOOUTPUTLOOP;
    
    dump_format(outputFile, 0, outputFile->filename, 1);
   }
   
   if(isOk(error))
   {
      if (encodeVideo > 0) 
      {
         AVCodec *codec;
         codec = avcodec_find_encoder(videoStream->codec->codec_id);
         if (!codec) 
         {
            error = CodecUnsupportedError;
         }
         else if (avcodec_open(videoStream->codec, codec) < 0) 
         {
            error = CodecUnsupportedError;
         }
      }
   }

   if(isOk(error))
   {
      if (encodeAudio > 0) 
      {
         AVCodec *codec;
         codec = avcodec_find_encoder(audioStream->codec->codec_id);
         if (!codec) 
         {
            error = CodecUnsupportedError;
         }
         else if (avcodec_open(audioStream->codec, codec) < 0) 
         {
            error = CodecUnsupportedError;
         }
      }
   }
   
   if(isOk(error) && encodeVideo > 0)
   {
      int size = avpicture_get_size(videoStream->codec->pix_fmt, getWidth(), getHeight());
      pictureToEncodeBuf = (uint8_t*)av_malloc(size);
      if (!pictureToEncodeBuf)
      {
         error = MemoryError;
      }
      else
      {
         avpicture_fill(pictureToEncode, pictureToEncodeBuf, videoStream->codec->pix_fmt, getWidth(), getHeight());
      }
   }
   if(isOk(error))
   {
      audioOutputBufSize = 4*MAX_AUDIO_PACKET_SIZE;
      audioResampler = NULL;
      audioResamplerBuf = NULL;
      audioOutputBuf = NULL;
      if(encodeAudio)
      {
          audioResamplerBuf = (uint8_t *)av_malloc(2*MAX_AUDIO_PACKET_SIZE);
          audioOutputBuf = (uint8_t *)av_malloc(audioOutputBufSize);
          if (!audioResamplerBuf || !audioOutputBuf)
            error = MemoryError;
      }
      //complete with audio buffers
   }
   
   if(isOk(error))
   {
      /* open files and write file headers */
      if (av_write_header(outputFile) < 0) 
      {
         cout << "Cannot write header format.... Bad codec params??" << endl;
         error = FileWriteError;
      }
   }

   if(isOk(error))
   {
      opened = true;
   }

   return error;
}

void Encoder::close()
{
   if(isOk(testOpen()))
   {
      opened = false;
      av_write_trailer(outputFile);
      /* close each encoder */
      if(encodeVideo > 0)
      {
        avcodec_close(videoStream->codec);
      }
      if(encodeAudio > 0)
      {
        avcodec_close(audioStream->codec);
      }
      if(audioResampler) audio_resample_close(audioResampler);
      audioResampler = NULL;
      av_free(audioResamplerBuf);
      av_free(audioOutputBuf);

      //av_freep(&outputStream.st->codec.stats_in);
      /* write the trailer if needed and close file */
      //av_write_trailer(outputFile);
      url_fclose(outputFile->pb);
      if(videoStreamIndex >= 0) av_free(outputFile->streams[videoStreamIndex]);
      if(audioStreamIndex >= 0) av_free(outputFile->streams[audioStreamIndex]);

      av_free(outputFile);

      //maybe these are not allocated
      av_free(transitionPictureBuf);
      av_free(pictureToEncodeBuf);
      if(img_resample_ctx)img_resample_close(img_resample_ctx);
      
      //free audio buffers
   }
}

ReturnCode Encoder::reallocTransitionPicture(int newWidth, int newHeight)
{
   ReturnCode error = OkCode;
   if(transitionPictureBuf == NULL || 
      transitionPictureWidth != newWidth || 
      transitionPictureHeight != newHeight)
   {
      av_free(transitionPictureBuf);
      if(img_resample_ctx)img_resample_close(img_resample_ctx);
      int size = avpicture_get_size(videoStream->codec->pix_fmt, newWidth, newHeight);
      transitionPictureBuf = (uint8_t*)av_malloc(size);
      if (!transitionPictureBuf)
      {
         error = MemoryError;
      }
      if(isOk(error))
      {
         img_resample_ctx = img_resample_init( 
                             this->getWidth(), this->getHeight(),
                             newWidth, newHeight);
         if(img_resample_ctx == NULL)
         {
            error = MemoryError;
         }
      }
      if(isOk(error))
      {
         avpicture_fill(transitionPicture, transitionPictureBuf, videoStream->codec->pix_fmt, newWidth, newHeight);
         transitionPictureWidth = newWidth;
         transitionPictureHeight = newHeight;
      }
   }
   return error;
}

ReturnCode Encoder::addFrame(char *rgb, int width, int height)
{
   AVPicture tempPic;
   enum PixelFormat pix_fmt=PIX_FMT_RGB24;

   tempPic.data[0] = (uint8_t*)rgb;
   tempPic.linesize[0] = width*3;//getWidth() * 3;
   
   return addRawFrame(&tempPic, width, height, pix_fmt);
}


ReturnCode Encoder::reallocAudioResampler(int newChannels, int newSampleRate)
{
   ReturnCode error = OkCode;
   if(audioResampler == NULL || 
      audioResamplerChannels != newChannels || 
      audioResamplerSampleRate != newSampleRate)
   {
   
      if(audioResampler) audio_resample_close(audioResampler);
      audioResampler = audio_resample_init(
        acodecChannels, newChannels,
        acodecSampleRate, 
        newSampleRate);


      if(!audioResampler)
      {
         error = MemoryError;
      }
      else
      {
        audioResamplerChannels = newChannels;
        audioResamplerSampleRate = newSampleRate;
      }
   }
   return error;
}


ReturnCode Encoder::addAudioFrame(uint8_t *samples, int samplesSize, int channels, int sampleRate)
{
    uint8_t *buftmp;
    int size_out, frame_bytes, ret;
    int nSamples = samplesSize / channels / 2;
    ReturnCode error = testOpen();
    
    if (isOk(error))
    {
       if(channels != acodecChannels || sampleRate != acodecSampleRate)
       {
         error = reallocAudioResampler(channels, sampleRate);
         if(isOk(error))
         {
            buftmp = audioResamplerBuf;
            size_out = audio_resample(audioResampler, 
                                  (short *)buftmp, (short *)samples, nSamples);
                                  
//                                  size / (ist->st->codec.channels * 2));

            size_out = size_out * acodecChannels * 2;
        }
      }
      else {
        buftmp = samples;
        size_out = nSamples * channels * 2;
      }
    }
    if(isOk(error))
    {
       /* now encode as many frames as possible */
       if (audioStream->codec->frame_size > 1) {
         //TODO: Recode this case... ffmpeg code acts differently
         frame_bytes = audioStream->codec->frame_size * 2 * acodecChannels;
         uint8_t *p = buftmp;
         int l = 0;
         for(;l < size_out;)
         {
            AVPacket pkt;
            av_init_packet(&pkt);

            ret = avcodec_encode_audio(audioStream->codec, audioOutputBuf, audioOutputBufSize, 
                                       (short *)p);
//            audio_size += ret;
            l += ret;
            p += ret;
            pkt.stream_index= audioStream->index;
            pkt.data= audioOutputBuf;
            pkt.size= ret;
            if(audioStream->codec->coded_frame)
                pkt.pts= audioStream->codec->coded_frame->pts;
            pkt.flags |= PKT_FLAG_KEY;
            av_interleaved_write_frame(outputFile, &pkt);
        
         }
       } else {
         AVPacket pkt;
         av_init_packet(&pkt);
         /* output a pcm frame */
         /* XXX: change encoding codec API to avoid this ? */
         switch(audioStream->codec->codec->id) {
         case CODEC_ID_PCM_S16LE:
         case CODEC_ID_PCM_S16BE:
         case CODEC_ID_PCM_U16LE:
         case CODEC_ID_PCM_U16BE:
             break;
         default:
             size_out = size_out >> 1;
             break;
         }
         ret = avcodec_encode_audio(audioStream->codec, audioOutputBuf, size_out, 
	 			   (short *)buftmp);
         //audio_size += ret;
         pkt.stream_index= audioStream->index;
         pkt.data= audioOutputBuf;
         pkt.size= ret;
         if(audioStream->codec->coded_frame)
             pkt.pts= audioStream->codec->coded_frame->pts;
         pkt.flags |= PKT_FLAG_KEY;
         av_interleaved_write_frame(outputFile, &pkt);
      }
    }
    
    return error;
}

ReturnCode Encoder::addRawFrame(AVPicture *pict, int width, int height, int pix_fmt)
{
   ReturnCode error = testOpen();
   AVPicture *transitionPict, *finalPict;
   AVCodecContext *enc = videoStream->codec;
   if(isOk(error))
   {
      reallocTransitionPicture(width, height);
      /* convert pixel format if needed */
      if (enc->pix_fmt != pix_fmt) 
      {
         transitionPict = transitionPicture;
         /* create temporary picture if requiered*/
         //reallocTransitionPicture(width, height);
         if (img_convert(transitionPicture, enc->pix_fmt, pict, pix_fmt, width, height) < 0) 
         {
            error = FrameActionError;
         }
      }
      else 
      {
         transitionPict = pict;
      }
   }
   if(isOk(error))
   {
      //Resample size if needed
      if(width != this->getWidth() || height != this->getHeight())
      {
         finalPict = pictureToEncode;
         img_resample(img_resample_ctx, pictureToEncode, transitionPict);                                      
      }
      else
      {
         finalPict = transitionPict;
      }
   }
   if(isOk(error))
   {
      AVFrame big_picture;
      int ret;
      memset(&big_picture, 0, sizeof(AVFrame));
      *(AVPicture*)&big_picture= *finalPict;

      AVPacket pkt;
      av_init_packet(&pkt);
      pkt.stream_index= videoStreamIndex;
      big_picture.quality = (int)videoStream->quality;
      big_picture.pts = AV_NOPTS_VALUE;

      ret = avcodec_encode_video(enc, videoBuffer, videoBufferSize, &big_picture);
      //av_write_frame(outputFile, outputStream.index, videoBuffer, ret);
      if(ret){
        pkt.data= videoBuffer;
        pkt.size= ret;
        if(enc->coded_frame)
          pkt.pts= enc->coded_frame->pts;
        if(enc->coded_frame && enc->coded_frame->key_frame)
          pkt.flags |= PKT_FLAG_KEY;
        av_interleaved_write_frame(outputFile, &pkt);
        //*frame_size = ret;
      }

    }
    return error;
}

ReturnCode Encoder::addPacket(AVPacket *pkt)
{
    static unsigned counter = 0;
    printf("Output packet size=%d pts=%lu dts=%lu counter=%d\n",pkt->size,(long)pkt->pts,(long)pkt->dts, ++counter);
    av_interleaved_write_frame(outputFile, pkt);
}

ReturnCode Encoder::addSequence(Decoder* decoder, TimeStamp start, TimeStamp length)
{
   return addSequence(
            decoder, 
            (FrameIndex)(start*decoder->getFrameRate()/1000.0), 
            (FrameIndex)(length*decoder->getFrameRate()/1000.0));
}
ReturnCode Encoder::addSequence(Decoder* decoder, FrameIndex startFrame, FrameIndex length)
{
   if(startFrame == 0) startFrame++;
   FrameIndex index = 0;
   FrameIndex nextIndex = 0;
   FrameIndex stopFrame = startFrame + length;
   
   ReturnCode error = testOpen();
   if(isOk(error))
   {
      error = decoder->setFrame(startFrame);
   }
   while(isOk(error) && decoder->getFrameIndex() < stopFrame)
   {
      if(decoder->getFrameIndex() < startFrame + nextIndex)
      {
         error = decoder->nextFrame();
         continue;
      }
      AVPicture *pict;
      enum PixelFormat pix_fmt;
      if(isOk(error))
      {
         error = decoder->getRawFrame(&pict, (int*)&pix_fmt);
      }
      if(isOk(error))
      {
         error = this->addRawFrame(pict, decoder->getWidth(), decoder->getHeight(), pix_fmt);
      }
      if(isOk(error))
      {
         index ++;
         nextIndex = (FrameIndex)(index / getFrameRate() * decoder->getFrameRate());
         printf("Encoding %ld index:%ld\n", nextIndex+startFrame, index);
      }
   }
   return error;
}
