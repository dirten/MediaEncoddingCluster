-module(json).
-export([encoding/3, config/3, file/3, watchfolder/3, codec/3, format/3, profile/3, nodes/3, softwareupdate/3]).
-include_lib("stdlib/include/qlc.hrl").
-include("schema_job.hrl").
-include("schema_profile.hrl").
-include("schema_watchfolder.hrl").
record_to_json(_R,[],_C,Acc)->
  Acc;

record_to_json(Record,[H|T],C,Acc)->
  NewAcc=[{H,element(C, Record)}]++Acc,
  record_to_json(Record,T,(C+1),NewAcc).

record_to_json(Record)->
  Fields=libdb:columns(element(1, Record)),
  lists:reverse(record_to_json(Record, Fields,2,[])).
record_to_json(Record, Fields)->
  %  Fields=libdb:columns(element(1, Record)),
  lists:reverse(record_to_json(Record, Fields,1,[])).

reformat([], Acc)->
  lists:reverse(Acc);
reformat([H|T], Acc) when is_tuple(H)->
  NewAcc=[{struct,record_to_json(H)}]++Acc,
  reformat(T, NewAcc).

reformat([],_Fields, Acc)->
  lists:reverse(Acc);
reformat([H|T], Fields, Acc) when is_tuple(H)->
  NewAcc=[{struct,record_to_json(H, Fields)}]++Acc,
  reformat(T, Fields, NewAcc).


template(SessionID,Data2,Data3)->
  Response =
    case get_request(Data2) of
      "GET"->
        Query=httpd:parse_query(Data3),
        case get_query_param(Query,"id") of
          {error,param_not_found}->do_get_without_id;
          _IdStr->do_get_with_id
        end;
      "POST"->do_post;
      _->
        {struct,[{error,request_method_invalid},{description,"Only request_method GET and POST is Supported by file"}]}
    end,
  R=mochijson:encode(Response),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, R).

config(SessionID,Data2,Data3)->
  Response =
    case get_request(Data2) of
      "GET"->
        Query=httpd:parse_query(Data3),
        case get_query_param(Query,"key") of
          {error,param_not_found}->{struct,config:get([])};
          K->
            Key=list_to_atom(K),
            {struct,[{Key,config:get(Key)}]}
        end;
      "POST"->do_post;
      _->
        {struct,[{error,request_method_invalid},{description,"Only request_method GET and POST is Supported by file"}]}
    end,
  R=mochijson:encode(Response),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, R).

file(SessionID,Data2,Data3)->
  Response =
    case get_request(Data2) of
      "GET"->
        Query=httpd:parse_query(Data3),
        case get_query_param(Query,"id") of
          {error,param_not_found}->
            Data=libdb:read(file),
            F=reformat(Data,[]),
            {struct,[{page,1},{total,length(Data)},{data,{array,F}}]};
          IdStr->
            Id=list_to_integer(IdStr),
            F=fun()->
                  qlc:e(qlc:q([Data||Data<-mnesia:table(file), element(2, Data)=:=Id]))
              end,
            {atomic,Data}=mnesia:transaction(F),
            [F2|_]=reformat(Data,[]),
            F2
        end;
      _->
        {struct,[{error,request_method_invalid},{description,"Only request_method GET is Supported by file"}]}
    end,
  R=mochijson:encode(Response),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, R).

encoding(SessionID,_Data2,_Data3)->
  NowToString=
    fun(Now)->
        case Now of
          undefined->
            no;
          _->
            {{Year,Month,Day},{Hour, Min, Sec}}=calendar:now_to_local_time(Now),
            integer_to_list(Year)++"-"++integer_to_list(Month)++"-"++integer_to_list(Day)++" "++integer_to_list(Hour)++":"++integer_to_list(Min)++":"++integer_to_list(Sec)
        end
    end,
  F = fun() ->
          Q = qlc:q([{
            libutil:to_string(element(3,E)),
            libutil:to_string(element(4,E)),
            libutil:to_string(element(5,E)),
            libutil:to_string(NowToString(element(6,E))),            %file.start_time                 %file.duration
            libutil:to_string(round(((list_to_integer(E#job.last_ts)-list_to_integer(element(12,F)))/list_to_integer(element(8,F)))*100))
                      %            Transform(E#job.last_ts),
                     } || E <- qlc:keysort(2,mnesia:table(job)),F<-mnesia:table(file),E#job.infile==element(2,F)]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
  %  io:format("Encodings:~p",[E]),
  F2=reformat(E,[source,target,start,endtime,complete],[]),
  J=mochijson:encode({struct,[{page,1},{total,length(E)},{data,{array,F2}}]}),
  %  io:format("Encodings:~p",[J]),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, J).

profile(SessionID,Data2,Data3)->
  %  io:format("Data:~p Data2:~p",[Data2, Data3]),
  case get_request(Data2) of
    "GET"->
      Query=httpd:parse_query(Data3),
      E=case get_query_param(Query,"id") of
          {error,param_not_found}->
            D=libdb:read(profile),
            %            io:format("Get"),
            F=reformat(D,[]),
            %            io:format("DbData:~p",[F]),
            mochijson:encode({struct,[{page,1},{total,length(F)},{data,{array,F}}]});

          IdStr->
            Id=list_to_integer(IdStr),
            {atomic,Data}=mnesia:transaction(
              fun()->
                  qlc:e(qlc:q([Data||Data<-mnesia:table(profile), element(2, Data)=:=Id]))
              end),
            [F|_]=reformat(Data,[]),
            mochijson:encode(F)
        end,
      mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
      mod_esi:deliver(SessionID, E);
    "POST"->set_profile(SessionID,Data2,Data3);

    _->
      J= mochijson:encode({struct,[{error,request_method_invalid},{description,"Only request_method GET and POST is Supported by profile"}]}),
      mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
      mod_esi:deliver(SessionID, J)
  end.

set_profile(SessionID,_Data2,Data3)->
  Query=httpd:parse_query(Data3),
  Id=list_to_integer(get_query_param(Query,"profileId")),
  ProfileId=if Id=:=-1->libdb:sequence(profile);true->Id end,
  ProfileName=get_query_param(Query,"profileName"),
  ProfileExtension=get_query_param(Query,"profileExtension"),
  ProfileFormat=get_query_param(Query,"profileFormat"),
  ProfileVideoCodec=list_to_integer(get_query_param(Query,"profileVideoCodec","13")),
  ProfileVideoBitrate=list_to_integer(get_query_param(Query,"profileVideoBitrate","512000")),
  ProfileVideoFramerate=list_to_float(get_query_param(Query,"profileVideoFramerate","25")),
  ProfileVideoWidth=list_to_integer(get_query_param(Query,"profileVideoWidth","0")),
  ProfileVideoHeight=list_to_integer(get_query_param(Query,"profileVideoHeight","0")),
  ProfileAudioChannels=list_to_integer(get_query_param(Query,"profileAudioChannels","2")),
  ProfileAudioCodec=list_to_integer(get_query_param(Query,"profileAudioCodec","86016")),
  ProfileAudioBitrate=list_to_integer(get_query_param(Query,"profileAudioBitrate","128000")),
  ProfileAudioSamplerate=list_to_integer(get_query_param(Query,"profileAudioSamplerate","44100")),
  ProfileMultipass=list_to_integer(get_query_param(Query,"profileMultipass","0")),
  ProfileGop=list_to_integer(get_query_param(Query,"profileGop","20")),
  Profile=#profile{
    id=ProfileId,
    name=ProfileName,
    ext=ProfileExtension,
    vformat=ProfileFormat,
    vcodec=ProfileVideoCodec,
    vbitrate=ProfileVideoBitrate,
    vframerate=ProfileVideoFramerate,
    vwidth=ProfileVideoWidth,
    vheight=ProfileVideoHeight,
    achannels=ProfileAudioChannels,
    acodec=ProfileAudioCodec,
    abitrate=ProfileAudioBitrate,
    asamplerate=ProfileAudioSamplerate,
    multipass=ProfileMultipass,
    gop=ProfileGop},
  io:format("Profile:~p",[Profile]),
  libdb:write(Profile),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n").

watchfolder(SessionID,Data2,Data3)->  
  case get_request(Data2) of
    "GET"->
      Query=httpd:parse_query(Data3),
      E=
        case get_query_param(Query,"id") of
          {error,param_not_found}->
            Data=libdb:read(watchfolder),
            F=reformat(Data,[]),
            mochijson:encode({struct,[{page,1},{total,length(Data)},{data,{array,F}}]});
          IdStr->
            Id=list_to_integer(IdStr),
            {atomic,Data}=mnesia:transaction(
              fun()->
                  qlc:e(qlc:q([Data||Data<-mnesia:table(watchfolder), element(2, Data)=:=Id]))
              end),
            NewData= case Data of
                       []->[#watchfolder{id=-1}];
                       Any->Any
                     end,
            [F|_]=reformat(NewData,[]),
            mochijson:encode(F)
        end,
      mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
      mod_esi:deliver(SessionID, E);
    "POST"->
      MyData= libutil:trim(Data3),
      %                io:format("Data:~p~n",[MyData]),
      MyD=try mochijson:decode(MyData) of
            {struct,JsonArray}->
              Folder = lists:foldl(fun json2record:build_watchfolder_record/2, #watchfolder{}, JsonArray ),
              %                        io:format("Folder:~p",[Folder]),
              libdb:write(Folder),
              {struct,[{"ok", true}, {"id", Folder#watchfolder.id}]};
            _Any->
              %                        io:format("Unknown Format:~p~n",[Any]),
              {struct,[{error,unknown_json_format}]}
          catch
            _:_->
              {struct,[{error,parsing_json_data}]}
          end,
      %                io:format("JSON:~p~n",[MyD]),
      J= mochijson:encode(MyD),
      mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
      mod_esi:deliver(SessionID, J);
    _->
      J= mochijson:encode({struct,[{error,unknown_request_method}]}),
      mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
      mod_esi:deliver(SessionID, J)
  end.

codec(SessionID,Data2,Data3)->
  case get_request(Data2) of
    "GET"->
      Query=httpd:parse_query(Data3),
      E=
        case get_query_param(Query,"id") of
          {error,param_not_found}->sys_port:get_codec_list();
          IdStr->
            _Id=list_to_integer(IdStr),
            sys_port:get_codec_list()
        end,
      F=reformat(E,[name,id,type, encoder, decoder,capabilities],[]),
      J= mochijson:encode({struct,[{page,1},{total,length(E)},{data,{array,F}}]}),
      mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
      mod_esi:deliver(SessionID, J);
    _->
      J= mochijson:encode({struct,[{error,request_method_invalid},{description,"Only request_method GET is Supported by codecs"}]}),
      mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
      mod_esi:deliver(SessionID, J)
  end.

format(SessionID,Data2,Data3)->
  case get_request(Data2) of
    "GET"->
      Query=httpd:parse_query(Data3),
      E=
        case get_query_param(Query,"id") of
          {error,param_not_found}->sys_port:get_format_list();
          IdStr->
            _Id=list_to_integer(IdStr),
            sys_port:get_format_list()
        end,
      %          io:format("Format:~p",[E]),
      F=reformat(E,[name,long_name,mime_type, extensions],[]),
      %          io:format("Format:~p",[F]),
      J= mochijson:encode({struct,[{page,1},{total,length(E)},{data,{array,F}}]}),
      mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
      mod_esi:deliver(SessionID, J);
    _->
      J= mochijson:encode({struct,[{error,request_method_invalid},{description,"Only request_method GET is Supported by format"}]}),
      mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
      mod_esi:deliver(SessionID, J)
  end.

nodes(SessionID,_Data2,_Data3)->
  Fun=fun(Data)->
          case Data of
            client->
              "Client";
            server->
              "Server";
            both->
              "Server / Client";
            _->
              "Not running"
          end
      end,
  E=[{atom_to_list(X),Fun(rpc:call(X,config,get,[mode])),rpc:call(X,libcode,get_mhive_version,[])}||X<-[node()|nodes()]],
  F=reformat(E,[name,mode,version],[]),
  J= mochijson:encode({struct,[{page,1},{total,length(E)},{data,{array,F}}]}),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, J).

softwareupdate(SessionID,_Data2,_Data3)->
  Fun = fun() ->
            Q = qlc:q([{
              libutil:to_string(element(2,E)),
              libutil:to_string(element(4,E)),
              libutil:to_string(element(5,E))
                       } || E <-qlc:keysort(2, mnesia:table(releases)), element(5,E)=:=downloaded orelse element(5,E)=:= installed]),
            qlc:e(Q)
        end,
  {atomic,E}=mnesia:transaction(Fun),
  F=reformat(E,[version,description,state],[]),
  J= mochijson:encode({struct,[{page,1},{total,length(E)},{data,{array,F}}]}),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, J).



get_request(Data)->
  get_query_param(Data,request_method).

get_query_param(Query,Key)->
  get_query_param(Query,Key,{error,param_not_found}).

get_query_param([],_Param, Default)->
  Default;
get_query_param([H|T],Param, Default)->
  if element(1,H)=:=Param ->element(2,H);
    true->get_query_param(T,Param, Default)
  end.