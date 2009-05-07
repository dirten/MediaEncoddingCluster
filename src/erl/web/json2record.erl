-module(json2record).
-include("schema_watchfolder.hrl").
-include("schema_profile.hrl").
-compile(export_all).


build_watchfolder_record({"id", Val}, Rec)->
  %  io:format("Setting id to ~p~n",[Val]),
  Id=if is_list(Val)->list_to_integer(Val);true->Val end,
  NewId = if
            Id =:= -1-> libdb:sequence(watchfolder);
            true->Id
          end,
  Rec#watchfolder{id=NewId};
build_watchfolder_record({"profileid", Val}, Rec)->
  %  io:format("Setting id to ~p~n",[Val]),
  Id=if is_list(Val)->list_to_integer(Val);true->Val end,
  Rec#watchfolder{profile=Id};
build_watchfolder_record({"recursive", Val}, Rec)->
  %  io:format("Setting id to ~p~n",[Val]),
  Rec#watchfolder{recursive=Val};
build_watchfolder_record({"status", Val}, Rec)->
  %  io:format("Setting id to ~p~n",[Val]),
  Rec#watchfolder{status=Val};
build_watchfolder_record({"infolder", Val}, Rec)->
  %  io:format("Setting id to ~p~n",[Val]),
  Rec#watchfolder{infolder=Val};
build_watchfolder_record({"outfolder", Val}, Rec)->
  %  io:format("Setting id to ~p~n",[Val]),
  Rec#watchfolder{outfolder=Val};
build_watchfolder_record({"filter", Val}, Rec)->
  %  io:format("Setting id to ~p~n",[Val]),
  Rec#watchfolder{filter=Val}.

%Data:"{\"profileFormat\":\"ac3\",\"profileVideoCodec\":\"13\",\"profileVideoFramerate\":\"25\",\"profileAudioCodec\":\"86016\",
%\"profileAudioSamplerate\":\"48000\",\"profileId\":\"1\",\"profileName\":\"Test Profile\",\"profileExtension\":\"avi\",
%\"profileVideoBitrate\":\"1024000\",\"profileVideoWidth\":\"640\",\"profileVideoHeight\":\"480\",\"profileAudioChannels\":\"\",\"profileAudioBitrate\":\"192000\",
%\"profileMultipass\":\"\",\"profileGop\":\"\",}"

build_profile_record({"profileId",Val}, Rec)->
  Id=if is_list(Val)->list_to_integer(Val);true->Val end,
  NewId = if
            Id =:= -1-> libdb:sequence(profile);
            true->Id
          end,
  Rec#profile{id=NewId};
build_profile_record({"profileFormat",Val}, Rec)->
  Rec#profile{vformat=Val};
build_profile_record({"profileVideoCodec",Val}, Rec)->
  Rec#profile{vcodec=Val};
build_profile_record({"profileVideoFramerate",Val}, Rec)->
  Rec#profile{vframerate=Val};
build_profile_record({"profileAudioCodec",Val}, Rec)->
  Rec#profile{acodec=Val};
build_profile_record({"profileAudioSamplerate",Val}, Rec)->
  Rec#profile{asamplerate=Val};
build_profile_record({"profileName",Val}, Rec)->
  Rec#profile{name=Val};
build_profile_record({"profileExtension",Val}, Rec)->
  Rec#profile{ext=Val};
build_profile_record({"profileVideoBitrate",Val}, Rec)->
  Rec#profile{vbitrate=Val};
build_profile_record({"profileVideoWidth",Val}, Rec)->
  Rec#profile{vwidth=Val};
build_profile_record({"profileVideoHeight",Val}, Rec)->
  Rec#profile{vheight=Val};
build_profile_record({"profileAudioChannels",Val}, Rec)->
  Rec#profile{achannels=Val};
build_profile_record({"profileAudioBitrate",Val}, Rec)->
  Rec#profile{abitrate=Val};
build_profile_record({"profileMultipass",Val}, Rec)->
  Rec#profile{multipass=Val};
build_profile_record({"profileGop",Val}, Rec)->
  Rec#profile{gop=Val}.



