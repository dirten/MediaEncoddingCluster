/*----------------------------------------------------------------------
 *  File    : AVFilter.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 1. December 2013, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2013   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */
#ifndef AVFILTER_H
#define AVFILTER_H
#include <map>

#include "AVPipe.h"
#include "AV.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace av {
      enum AVFilterType {
        VIDEO,
        AUDIO
      };

      class AV_EXPORT AVFilter: public AVPipe
      {          
        public:
          AVFilter(AVFilterType type,std::string filter);
          virtual ~AVFilter();
          bool newFrame(Ptr<Frame> p);
          bool newPacket(Ptr<Packet>){return false;}
          void setInputParameter(std::string key, std::string value);
          void setOutputParameter(std::string key, std::string value);
          void init();
      private:
          void initAudioSourceSink();
          void initVideoSourceSink();
          bool sanityCheck(std::map<std::string, std::string>,std::list<std::string>, std::string);
          std::string _filter;
          std::map<std::string, std::string> _input_params;
          std::map<std::string, std::string> _output_params;
          AVFilterType _type;
          AVFilterContext *buffersink_ctx;
          AVFilterContext *buffersrc_ctx;
          AVFilterGraph *filter_graph;
          Ptr<Frame> outFrame;

      };

    }
  }
}
#endif // AVFILTER_H
