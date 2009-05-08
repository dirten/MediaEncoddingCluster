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
build_profile_record({"profileGop",Val}, Rec=#profile{})->
  Rec#profile{gop=Val}.

build_record({"profileId",Val}, Rec=#profile{})->
  Id=if is_list(Val)->list_to_integer(Val);true->Val end,
  NewId = if
            Id =:= -1-> libdb:sequence(profile);
            true->Id
          end,
  Rec#profile{id=NewId};
build_record({"profileFormat",Val}, Rec=#profile{})->
  Rec#profile{vformat=Val};
build_record({"profileVideoCodec",Val}, Rec=#profile{})->
  Rec#profile{vcodec=Val};
build_record({"profileVideoFramerate",Val}, Rec=#profile{})->
  Rec#profile{vframerate=Val};
build_record({"profileAudioCodec",Val}, Rec=#profile{})->
  Rec#profile{acodec=Val};
build_record({"profileAudioSamplerate",Val}, Rec=#profile{})->
  Rec#profile{asamplerate=Val};
build_record({"profileName",Val}, Rec=#profile{})->
  Rec#profile{name=Val};
build_record({"profileExtension",Val}, Rec=#profile{})->
  Rec#profile{ext=Val};
build_record({"profileVideoBitrate",Val}, Rec=#profile{})->
  Rec#profile{vbitrate=Val};
build_record({"profileVideoWidth",Val}, Rec=#profile{})->
  Rec#profile{vwidth=Val};
build_record({"profileVideoHeight",Val}, Rec=#profile{})->
  Rec#profile{vheight=Val};
build_record({"profileAudioChannels",Val}, Rec=#profile{})->
  Rec#profile{achannels=Val};
build_record({"profileAudioBitrate",Val}, Rec=#profile{})->
  Rec#profile{abitrate=Val};
build_record({"profileMultipass",Val}, Rec=#profile{})->
  Rec#profile{multipass=Val};
build_record({"profileGop",Val}, Rec=#profile{})->
  Rec#profile{gop=Val}.



