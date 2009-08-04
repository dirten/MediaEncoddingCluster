-module(json2record).
-include("schema_watchfolder.hrl").
-include("schema_profile.hrl").
-include("schema_setup.hrl").
-include("schema_request.hrl").
-compile(export_all).

-import(libutil,[toInteger/1, toString/1, toNumber/1]).

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
    Rec#watchfolder{filter=Val};
build_watchfolder_record({_Any, Val}, Rec)->
    Rec.


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
    Rec#profile{vcodec=toInteger(Val)};
build_profile_record({"profileVideoFramerate",Val}, Rec)->
    Rec#profile{vframerate=toInteger(Val)};
build_profile_record({"profileAudioCodec",Val}, Rec)->
    Rec#profile{acodec=toInteger(Val)};
build_profile_record({"profileAudioSamplerate",Val}, Rec)->
    Rec#profile{asamplerate=toInteger(Val)};
build_profile_record({"profileName",Val}, Rec)->
    Rec#profile{name=Val};
build_profile_record({"profileExtension",Val}, Rec)->
    Rec#profile{ext=Val};
build_profile_record({"profileVideoBitrate",Val}, Rec)->
    Rec#profile{vbitrate=toInteger(Val)};
build_profile_record({"profileVideoWidth",Val}, Rec)->
    Rec#profile{vwidth=toInteger(Val)};
build_profile_record({"profileVideoHeight",Val}, Rec)->
    Rec#profile{vheight=toInteger(Val)};
build_profile_record({"profileAudioChannels",Val}, Rec)->
    Rec#profile{achannels=toInteger(Val)};
build_profile_record({"profileAudioBitrate",Val}, Rec)->
    Rec#profile{abitrate=toInteger(Val)};
build_profile_record({"profileMultipass",Val}, Rec)->
    Rec#profile{multipass=toInteger(Val)};
build_profile_record({"profileGop",Val}, Rec=#profile{})->
    Rec#profile{gop=toInteger(Val)}.

build_record({"profileId",Val}, Rec=#profile{})->
    Id=toInteger(Val),
    NewId = if
                Id =:= -1-> libdb:sequence(profile);
                true->Id
            end,
    Rec#profile{id=NewId};
build_record({"profileFormat",Val}, Rec=#profile{})->
    Rec#profile{vformat=toString(Val)};
build_record({"profileVideoCodec",Val}, Rec=#profile{})->
    Rec#profile{vcodec=toInteger(Val)};
build_record({"profileVideoFramerate",Val}, Rec=#profile{})->
    Rec#profile{vframerate=toNumber(Val)};
build_record({"profileAudioCodec",Val}, Rec=#profile{})->
    Rec#profile{acodec=toInteger(Val)};
build_record({"profileAudioSamplerate",Val}, Rec=#profile{})->
    Rec#profile{asamplerate=toInteger(Val)};
build_record({"profileName",Val}, Rec=#profile{})->
    Rec#profile{name=toString(Val)};
build_record({"profileExtension",Val}, Rec=#profile{})->
    Rec#profile{ext=toString(Val)};
build_record({"profileVideoBitrate",Val}, Rec=#profile{})->
    Rec#profile{vbitrate=toInteger(Val)};
build_record({"profileVideoWidth",Val}, Rec=#profile{})->
    Rec#profile{vwidth=toInteger(Val)};
build_record({"profileVideoHeight",Val}, Rec=#profile{})->
    Rec#profile{vheight=toInteger(Val)};
build_record({"profileAudioChannels",Val}, Rec=#profile{})->
    Rec#profile{achannels=toInteger(Val)};
build_record({"profileAudioBitrate",Val}, Rec=#profile{})->
    Rec#profile{abitrate=toInteger(Val)};
build_record({"profileMultipass",Val}, Rec=#profile{})->
    Rec#profile{multipass=toInteger(Val)};
build_record({"profileGop",Val}, Rec=#profile{})->
    Rec#profile{gop=toInteger(Val)};

build_record({"instance_mode",Val}, Rec=#setup{})->
    Rec#setup{instance_mode=Val};
build_record({"auto_node_discovery",Val}, Rec=#setup{})->
    Rec#setup{auto_node_discovery=Val};
build_record({"http_port",Val}, Rec=#setup{})->
    Rec#setup{http_port=Val};
%build_record({_Unknown,Val}, Rec=#setup{})->
%    Rec;

build_record({"action",Val}, Rec=#request{})->
    Rec#request{action=Val};
build_record({_Any,_Val}, Rec)->
    Rec.



