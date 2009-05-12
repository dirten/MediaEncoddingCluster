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

format([],Acc)->Acc;
format([H|T],Acc)->
  NewAcc=format(H, Acc),
  format(T,NewAcc);

format({Key, Val},Acc)->
  case libutil:is_string(Val) of
    true->
      [{Key, Val}]++Acc;
    false->
      format_list({Key, Val},Acc)
  end.

format_list({Key, Val},Acc) when is_list(Val)->
  [{Key,{array, Val}}]++Acc;
format_list({Key, Val},Acc) ->
  [{Key,Val}]++Acc.


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
          {error,param_not_found}->
            Dat={struct,format(config:get([]),[])},
            Dat;
          K->
            Key=list_to_atom(K),
            Dat={struct,format([{Key,config:get(Key)}],[])},
            Dat
        end;
      "POST"->do_post;
      _->
        {struct,[{error,request_method_invalid},{description,"Only request_method GET and POST is Supported by file"}]}
    end,
  R=try mochijson:encode(Response) of
      Any->Any
    catch
      _:_->mochijson:encode({struct,[{error,internal_server_error},{description,"Internal server Error"}]})
    end,
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
  MyData= libutil:trim(Data3),
  MyD=try mochijson:decode(MyData) of
        {struct,JsonStruct}->
          Profile = lists:foldl(fun json2record:build_record/2, #profile{}, JsonStruct ),
          libdb:write(Profile),
          {struct,[{"ok", true}, {"id", Profile#profile.id}]};
        _Any->
          {struct,[{error,unknown_json_format}]}
      catch
        _M:_F->
          {struct,[{error,parsing_json_data}]}
      end,
  J= mochijson:encode(MyD),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, J).

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
            {struct,JsonStruct}->
              Folder = lists:foldl(fun json2record:build_watchfolder_record/2, #watchfolder{}, JsonStruct ),
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

nodes(SessionID,Data2,Data3)->
  Response =
    case get_request(Data2) of
      "GET"->
        Query=httpd:parse_query(Data3),
        case get_query_param(Query,"id") of
          _Any->
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
            {struct,[{page,1},{total,length(E)},{data,{array,F}}]}
        end;
      "POST"->
        MyData= libutil:trim(Data3),
        io:format("Data:~p~n",[MyData]),
        try mochijson:decode(MyData) of
          {array,[{struct,[{"command","connect"}]},
                       {struct,[{"host",H}]},
                       {struct,[{"name",N}]}]}->
            case libnet:connect(H,N) of
              {ok,_Node}->{struct,[{"ok", true}]};
              {error,Msg}->{error,Msg}
            end;
          Any->
            io:format("Unknown Format:~p~n",[Any]),
            {struct,[{error,unknown_json_format}]}
        catch
          _:_->
            {struct,[{error,parsing_json_data}]}
        end;
      _->
        {struct,[{error,request_method_invalid},{description,"Only request_method GET and POST is Supported by file"}]}
    end,
  R=mochijson:encode(Response),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, R).

softwareupdate2(SessionID,Data2,_Data3)->
  Response =
    case get_request(Data2) of
      "GET"->
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
        {struct,[{page,1},{total,length(E)},{data,{array,F}}]};
      "POST"->do_post;
      _->
        {struct,[{error,request_method_invalid},{description,"Only request_method GET and POST is Supported by file"}]}
    end,
  R=mochijson:encode(Response),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, R).

softwareupdate(SessionID,Data2,Data3)->
  Response =
    case get_request(Data2) of
      "GET"->
        Query=httpd:parse_query(Data3),
        case get_query_param(Query,"id") of
          {error,param_not_found}->
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
            {struct,[{page,1},{total,length(E)},{data,{array,F}}]};
          IdStr->
            Fun = fun() ->
                      Q = qlc:q([{
                        libutil:to_string(element(2,E)),
                        libutil:to_string(element(4,E)),
                        libutil:to_string(element(5,E))
                                 } || E <-qlc:keysort(2, mnesia:table(releases)), element(2,E)=:=IdStr]),
                      qlc:e(Q)
                  end,
            {atomic,E}=mnesia:transaction(Fun),
            [Data4|_]=reformat(E,[version,description,state],[]),
            Data4
            
        end;
      "POST"->
        io:format("POST COMMAND:~p",[Data3]),
        MyData= libutil:trim(Data3),
        io:format("Data:~p~n",[MyData]),
        MyD=try mochijson:decode(MyData) of
              {struct,[{"command","check_for_updates"}]}->
                auto_update:run(),
                {struct,[{"ok", true}]};
              {struct,[{"install",Version}]}->
                case auto_update:install(Version) of
                  ok->{struct,[{"ok", true}, {"version", Version}]};
                  {error,{D,A}}->{struct,[{"error", D},{"description",A}]};
                  {error,D}->{struct,[{"error", D}]}
                end;
              _Any->
                %                        io:format("Unknown Format:~p~n",[Any]),
                {struct,[{error,unknown_json_format}]}
            catch
              _:_->
                {struct,[{error,parsing_json_data}]}
            end,
        io:format("POST COMMAND:~p",[MyD]),
        MyD;
      _->
        {struct,[{error,request_method_invalid},{description,"Only request_method GET and POST is Supported by softwareupdate"}]}
    end,
  R=mochijson:encode(Response),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, R).

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