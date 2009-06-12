%%%----------------------------------------------------------------------
%%% File    : av.hrl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : defines for the ffmpeg av bridge
%%% Created : 18 Feb 2009 by Jan Hölscher <jan.hoelscher@esblab.com>
%%%
%%%
%%% MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
%%%
%%% This program is free software; you can redistribute it and/or
%%% modify it under the terms of the GNU General Public License as
%%% published by the Free Software Foundation; either version 2 of the
%%% License, or (at your option) any later version.
%%%
%%% This program is distributed in the hope that it will be useful,
%%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%%% General Public License for more details.
%%%
%%% You should have received a copy of the GNU General Public License
%%% along with this program; if not, write to the Free Software
%%% Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
%%% 02111-1307 USA
%%%
%%%----------------------------------------------------------------------

-define(AVFMT_NOFILE,1).
-define(AVFMT_NEEDNUMBER,2).
-define(AVFMT_SHOW_IDS,8).
-define(AVFMT_RAWPICTURE,32).
-define(AVFMT_GLOBALHEADER,64).
-define(AVFMT_NOTIMESTAMPS,128).
-define(AVFMT_GENERIC_INDEX,256).
-define(AVFMT_TS_DISCONT,512).
-define(AVFMT_VARIABLE_FPS,1024).

-define(AVCODEC_FLAG_GLOBAL_HEADER,4194304).

-define(CODEC_TYPE_UNKNOWN,-1).
-define(CODEC_TYPE_VIDEO,0).
-define(CODEC_TYPE_AUDIO,1).
-define(CODEC_TYPE_DATA,2).
-define(CODEC_TYPE_SUBTITLE,3).
-define(CODEC_TYPE_ATTACHMENT,4).
-define(CODEC_TYPE_NB,5).

