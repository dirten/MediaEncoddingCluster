/*----------------------------------------------------------------------
 *  File    : Profile.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 28. Januar 2010, 17:31 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
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


#ifndef _PROFILE_H
#define	_PROFILE_H
#include <string>
#include "org/esb/av/AV.h"
namespace org{
  namespace esb{
    namespace db{
      struct Profile{
        int id;
        std::string name;
        std::string video_format;
        CodecID video_codec;
        int video_bitrate;
        int video_framerate;
        int video_width;
        int video_height;
        std::string video_extra;
        CodecID audio_codec;
        int audio_channels;
        int audio_bitrate;
        int audio_samplerate;
      };
    }
  }
}

#endif	/* _PROFILE_H */

