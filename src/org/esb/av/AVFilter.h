#ifndef AVFILTER_H
#define AVFILTER_H

#include "AVPipe.h"
#include "AV.h"
namespace org {
  namespace esb {
    namespace av {
      enum AVFilterType {
        VIDEO,
        AUDIO
      };

      class AVFilter: public AVPipe
      {
        public:
          AVFilter(AVFilterType type,std::string filter);
          ~AVFilter();
          bool newFrame(Ptr<Frame> p);
          void setInputParameter(std::string key, std::string value);
          void setOutputParameter(std::string key, std::string value);
          void init();
      private:
          void initAudioSourceSink();
          void initVideoSourceSink();
          void sanityCheck(std::map<std::string, std::string>,std::list<std::string>, std::string);
          std::string _filter;
          std::map<std::string, std::string> _input_params;
          std::map<std::string, std::string> _output_params;
          AVFilterType _type;
          AVFilterContext *buffersink_ctx;
          AVFilterContext *buffersrc_ctx;
          AVFilterGraph *filter_graph;

      };

    }
  }
}
#endif // AVFILTER_H
