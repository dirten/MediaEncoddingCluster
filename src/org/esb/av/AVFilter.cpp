#include "AVFilter.h"
#include "org/esb/lang/Exception.h"
#include "org/esb/util/StringUtil.h"
#include "Frame.h"
#include "org/esb/util/Log.h"

using org::esb::util::StringUtil;

namespace org {
  namespace esb {
    namespace av {

      AVFilter::AVFilter(AVFilterType type,std::string filter) : _type(type), _filter(filter)
      {
        buffersink_ctx=NULL;
        buffersrc_ctx=NULL;
        filter_graph=NULL;
      }

      AVFilter::~AVFilter(){
        if(buffersink_ctx)
          avfilter_free(buffersink_ctx);
        if(buffersrc_ctx)
          avfilter_free(buffersrc_ctx);
        if(filter_graph)
          avfilter_graph_free(&filter_graph);
      }

      void AVFilter::init(){
        if(_type==VIDEO){
          initVideoSourceSink();
        } else if(_type==AUDIO){
          initAudioSourceSink();
        }
        outFrame=Ptr<Frame>(new Frame());
        outFrame->setFinished(true);
        int num = 0,den = 0;
        sscanf(_input_params["time_base"].c_str(),"%d/%d", &num, &den);

        AVRational r={num, den};

        outFrame->setTimeBase(r);
        outFrame->setDuration(1);
      }

      void AVFilter::setInputParameter(std::string key, std::string value){
        _input_params[key]=value;
      }

      void AVFilter::setOutputParameter(std::string key, std::string value)
      {
        _output_params[key]=value;
      }

      void AVFilter::initAudioSourceSink(){
        char args[512];
        int ret;
        ::AVFilter *abuffersrc  = avfilter_get_by_name("abuffer");
        ::AVFilter *abuffersink = avfilter_get_by_name("abuffersink");
        AVFilterInOut *outputs = avfilter_inout_alloc();
        AVFilterInOut *inputs  = avfilter_inout_alloc();
        AVABufferSinkParams *buffersink_params;
        filter_graph = avfilter_graph_alloc();

        if(_input_params.find("channel_layout")==_input_params.end()){
          throw Exception("input channel_layout not set");
        }
        if(_input_params.find("sample_format")==_input_params.end()){
          throw Exception("input sample_format not set");
        }
        if(_input_params.find("sample_rate")==_input_params.end()){
          throw Exception("input sample_rate not set");
        }
        if(_input_params.find("time_base")==_input_params.end()){
          throw Exception("input time_base not set");
        }
        std::map<std::string, std::string>::iterator it=_input_params.begin();
        for(;it!=_input_params.end();it++){
          LOGDEBUG("InputKey="<<(*it).first<<" value="<<(*it).second)
        }

        snprintf(args, sizeof(args),
        "time_base=%s:sample_rate=%d:sample_fmt=%s:channel_layout=0x%llx",
        _input_params["time_base"].c_str(), atoi(_input_params["sample_rate"].c_str()),
        _input_params["sample_format"].c_str(), atoll(_input_params["channel_layout"].c_str()));
        //buffersrc_ctx->thread_type=0;
        filter_graph->thread_type=0;
        filter_graph->nb_threads=1;

        ret = avfilter_graph_create_filter(&buffersrc_ctx, abuffersrc, "in", args, NULL, filter_graph);
        if (ret < 0) {
          av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer source with arguments\n%s", args);
          throw Exception(__FILE__, __LINE__,"Cannot create audio buffer source with arguments\n%s", args);
        }
        av_log(NULL, AV_LOG_INFO, "created audio buffer source with arguments\n%s", args);

        if(_output_params.find("channel_layout")==_output_params.end()){
          throw Exception(__FILE__, __LINE__,"output channel_layout not set");
        }
        if(_output_params.find("frame_size")==_output_params.end()){
          throw Exception(__FILE__, __LINE__,"output frame_size not set");
        }
        if(_output_params.find("sample_format")==_output_params.end()){
          throw Exception(__FILE__, __LINE__,"output sample_format not set");
        }
        if(_output_params.find("sample_rate")==_output_params.end()){
          throw Exception(__FILE__, __LINE__,"output sample_rate not set");
        }

        /*evaluate filter placeholder with parameter names from the output map*/
        it=_output_params.begin();
        for(;it!=_output_params.end();it++){
          LOGDEBUG("OutputKey="<<(*it).first<<" value="<<(*it).second)
          _filter=StringUtil::replace(_filter,"%"+(*it).first+"%",(*it).second);
        }
        //av_get_channel_layout(_output_params["channel_layout"].c_str());
        int64_t ch_layout=atoi((_output_params["channel_layout"]).c_str());//av_get_channel_layout((_output_params["channel_layout"]).c_str());

        LOGDEBUG("channel layout :"<<ch_layout)
        int frame_size=atoi(_output_params["frame_size"].c_str());

        AVSampleFormat sample_format=av_get_sample_fmt(_output_params["sample_format"].c_str());

        const enum AVSampleFormat out_sample_fmts[] = { sample_format, AV_SAMPLE_FMT_NONE };

        const int64_t out_channel_layouts[] = { ch_layout, -1 };
        const int out_channel_counts[] = { 2, -1 };

        //static const int64_t out_channel_counts[] = { 2, -1 };

        int out_sample_rates[] = { atoi(_output_params["sample_rate"].c_str()), -1 };
        /*
        snprintf(args, sizeof(args),
                 "channels=2:time_base=%s:sample_rate=%d:sample_fmt=%s:channel_layout=0x%"PRIx64,
                 _input_params["time_base"].c_str(), atoi(_output_params["sample_rate"].c_str()),
            _output_params["sample_format"].c_str(), atoll(_output_params["channel_layout"].c_str()));
         */
        /* buffer audio sink: to terminate the filter chain. */
        av_log(NULL, AV_LOG_INFO, "try creating audio buffer sink\n");

        filter_graph->thread_type=0;
        filter_graph->nb_threads=1;


        /* buffer video sink: to terminate the filter chain. */
        buffersink_params = av_abuffersink_params_alloc();
        buffersink_params->sample_fmts=out_sample_fmts;
        buffersink_params->channel_layouts=out_channel_layouts;
        buffersink_params->channel_counts=out_channel_counts;
        buffersink_params->sample_rates=out_sample_rates;


        ret = avfilter_graph_create_filter(&buffersink_ctx, abuffersink, "out", NULL, buffersink_params, filter_graph);

        av_log(NULL, AV_LOG_INFO, "created audio buffer sink\n");
        if (ret < 0) {
          av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer sink with arguments\n%s", args);
          throw Exception(__FILE__, __LINE__,"Cannot create audio buffer sink with arguments\n%s", args);
        }
        /*
        ret = av_opt_set_int_list(buffersink_ctx, "sample_fmts", out_sample_fmts, -1,
        AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
          av_log(NULL, AV_LOG_ERROR, "Cannot set output sample format\n");
          throw Exception(__FILE__, __LINE__,"Cannot set output sample format\n");
        }

        ret = av_opt_set_int_list(buffersink_ctx, "channel_layouts", out_channel_layouts, -1, AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
          av_log(NULL, AV_LOG_ERROR, "Cannot set output channel layout\n");
          throw Exception(__FILE__, __LINE__,"Cannot set output channel layout\n");
        }
        ret = av_opt_set_int_list(buffersink_ctx, "sample_rates", out_sample_rates, -1,
        AV_OPT_SEARCH_CHILDREN);

        if (ret < 0) {
          av_log(NULL, AV_LOG_ERROR, "Cannot set output sample rate\n");
          throw Exception(__FILE__, __LINE__,"Cannot set output sample rate\n");
        }
        */
        /*
        ret = av_opt_set_int_list(buffersink_ctx, "channel_counts", out_channel_counts, -1,
                                  AV_OPT_SEARCH_CHILDREN);
        if (ret < 0) {
          av_log(NULL, AV_LOG_ERROR, "Cannot set output channel counts\n");
          throw Exception(__FILE__, __LINE__,"Cannot set output channel counts\n");
        }
        */


        /* Endpoints for the filter graph. */
        outputs->name       = av_strdup("in");
        outputs->filter_ctx = buffersrc_ctx;
        outputs->pad_idx    = 0;
        outputs->next       = NULL;

        inputs->name       = av_strdup("out");
        inputs->filter_ctx = buffersink_ctx;
        inputs->pad_idx    = 0;
        inputs->next       = NULL;


        LOGDEBUG("create filter graph")
        if ((ret = avfilter_graph_parse_ptr(filter_graph, _filter.c_str(), &inputs, &outputs, NULL)) < 0){
          throw Exception("could not parse filter graph");
        }
        LOGDEBUG("config filter graph")
        if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0){
          throw Exception("could not configure filter graph");
        }

        av_buffersink_set_frame_size(buffersink_ctx,frame_size);
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);
        LOGDEBUG("filter graph success")
        //exit(0);
      }

      void AVFilter::initVideoSourceSink(){
        char args[512];
        int ret;
        ::AVFilter *buffersrc  = avfilter_get_by_name("buffer");
        ::AVFilter *buffersink = avfilter_get_by_name("buffersink");
        AVFilterInOut *outputs = avfilter_inout_alloc();
        AVFilterInOut *inputs  = avfilter_inout_alloc();
        AVBufferSinkParams *buffersink_params;
        filter_graph = avfilter_graph_alloc();

        /*sanity input check*/
        std::list<std::string>check_parameter;
        check_parameter.push_back("height");
        check_parameter.push_back("width");
        check_parameter.push_back("pixel_format");
        check_parameter.push_back("time_base");
        check_parameter.push_back("sample_aspect_ratio");
        sanityCheck(_input_params,check_parameter, "input");

        /*sanity output check*/
        check_parameter.clear();
        check_parameter.push_back("pixel_format");
        sanityCheck(_output_params,check_parameter, "output");


        /* buffer video source: the decoded frames from the decoder will be inserted here. */
        snprintf(args, sizeof(args),
        "video_size=%sx%s:pix_fmt=%s:time_base=%s:pixel_aspect=%s",
        _input_params["width"].c_str(), _input_params["height"].c_str(), _input_params["pixel_format"].c_str(),
        _input_params["time_base"].c_str(),
        _input_params["sample_aspect_ratio"].c_str());

        filter_graph->thread_type=0;
        filter_graph->nb_threads=1;


        ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in", args, NULL, filter_graph);
        if (ret < 0) {
          throw Exception(__FILE__, __LINE__,"Cannot create video buffer source with arguments\n%s", args);
        }
        av_log(NULL, AV_LOG_INFO, "created video buffer source with arguments\n%s", args);

        std::map<std::string, std::string>::iterator it=_output_params.begin();
        for(;it!=_output_params.end();it++){
          LOGDEBUG("OutputKey="<<(*it).first<<" value="<<(*it).second)
          _filter=StringUtil::replace(_filter,"%"+(*it).first+"%",(*it).second);
        }

        /* buffer video sink: to terminate the filter chain. */
        buffersink_params = av_buffersink_params_alloc();
        AVPixelFormat fmt=static_cast<AVPixelFormat>(atoi(_output_params["pixel_format"].c_str()));
        enum AVPixelFormat pix_fmts[] = { fmt, AV_PIX_FMT_NONE };
        buffersink_params->pixel_fmts = pix_fmts;

        //buffersrc_ctx->thread_type=0;
        filter_graph->thread_type=0;
        filter_graph->nb_threads=1;


        ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
        NULL, buffersink_params, filter_graph);
        av_free(buffersink_params);
        if (ret < 0) {
          throw Exception(__FILE__, __LINE__,"Cannot create video buffer sink with arguments\n%s", args);
        }

        /* Endpoints for the filter graph. */
        outputs->name       = av_strdup("in");
        outputs->filter_ctx = buffersrc_ctx;
        outputs->pad_idx    = 0;
        outputs->next       = NULL;

        inputs->name       = av_strdup("out");
        inputs->filter_ctx = buffersink_ctx;
        inputs->pad_idx    = 0;
        inputs->next       = NULL;

        if ((ret = avfilter_graph_parse_ptr(filter_graph, _filter.c_str(), &inputs, &outputs, NULL)) < 0){
          throw Exception(__FILE__, __LINE__,"could not parse filter graph\n");
        }
        if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0){
          throw Exception(__FILE__, __LINE__,"could not configure filter graph\n");
        }
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);
      }

      bool AVFilter::sanityCheck( std::map<std::string, std::string> input_map,std::list<std::string> pList, std::string inout){
        bool result=true;
        std::list<std::string>::iterator it=pList.begin();
        for(;it!=pList.end();it++){
          if(input_map.find(*it)==input_map.end()){
            result=false;
            //throw Exception(__FILE__, __LINE__,"%s %s not set", inout.c_str(), (*it).c_str());
          }
        }
        return result;
      }

      bool AVFilter::newFrame(Ptr<Frame> p)
      {
        bool result=false;
          int64_t filter_ch_layout=atoi((_input_params["channel_layout"]).c_str());
          int64_t frame_ch_layout=av_frame_get_channel_layout(p->getAVFrame());
          if(filter_ch_layout!=frame_ch_layout){
            _input_params["channel_layout"]=StringUtil::toString(frame_ch_layout);

            initAudioSourceSink();
          }




        /**
          * @TODO: have a look into this, why do i need to clone the frame for this operation
          * on some video files it will crash when i do not clone
          *
          * @TODO: Audio Frames should change its parameter like channel_layout during the packets
          *        need to handle changes of the incomming packets
          */
        AVFrame * frame=NULL;
        if(p){
          outFrame->setDuration(p->getDuration());
          frame=av_frame_clone(p->getAVFrame());
        }
        if(!frame){
          LOGDEBUG("flush filter")
        }
        //frame->opaque=new int(p->getDuration());
        if (av_buffersrc_add_frame_flags(buffersrc_ctx, frame, 0) < 0) {
          if(frame)
            av_frame_unref(frame);
          av_frame_free(&frame);
          //throw Exception(__FILE__, __LINE__,"failed to push frame into filter chain");
          return true;
        }
        /* pull filtered frames from the filtergraph */
        while (1) {
          int ret = av_buffersink_get_frame(buffersink_ctx, outFrame->getAVFrame());
          /*when not enough data is available*/
          if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            break;
          }
          result=true;
          LOGDEBUG("push new frame from filter")
          //outFrame->setDuration(*(int*)frame->opaque);
          pushFrame(outFrame);
        }
        if(!result && !frame){
          /*flushing next segment*/
          LOGDEBUG("push empty frame from filter")
          result|=pushFrame(Ptr<Frame>());
        }
        av_frame_unref(outFrame->getAVFrame());
        if(frame)
          av_frame_unref(frame);
        av_frame_free(&frame);
        return result;
      }
    }
  }
}
