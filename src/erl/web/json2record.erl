-module(json2record).
-include("schema_watchfolder.hrl").
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
