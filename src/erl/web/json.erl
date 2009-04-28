-module(json).
-export([get_file_list/3, get_file_detail/3, get_encoding_list/3, get_profile_list/3, get_profile/3, set_profile/3]).
-include_lib("stdlib/include/qlc.hrl").
-include("schema_job.hrl").
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
  Acc;
reformat([H|T], Acc) when is_tuple(H)->
  NewAcc=[{struct,record_to_json(H)}]++Acc,
  reformat(T, NewAcc).

reformat([],_Fields, Acc)->
  Acc;
reformat([H|T], Fields, Acc) when is_tuple(H)->
  NewAcc=[{struct,record_to_json(H, Fields)}]++Acc,
  reformat(T, Fields, NewAcc).

get_file_list(SessionID,_Data2,_Data3)->
  Data=libdb:read(file),
  F=reformat(Data,[]),
  J= mochijson:encode({struct,[{page,1},{total,length(Data)},{data,{array,F}}]}),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, J).

get_file_detail(SessionID,_Data2,Data3)->
  Query=httpd:parse_query(Data3),
  Id=list_to_integer(get_query_param(Query,"id")),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  F=fun()->
        qlc:e(qlc:q([Data||Data<-mnesia:table(file), element(2, Data)=:=Id]))
    end,
  {atomic,Data}=mnesia:transaction(F),
  [F2|_]=reformat(Data,[]),
  J=mochijson:encode(F2),
  mod_esi:deliver(SessionID, J).

get_encoding_list(SessionID,_Data2,_Data3)->
%  io:format("Encodings:",[]),
  NowToString=fun(Now)->
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

get_profile_list(SessionID,_Data2,_Data3)->
  E=libdb:read(profile),
  F=reformat(E,[]),
  J= mochijson:encode({struct,[{page,1},{total,length(E)},{data,{array,F}}]}),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  mod_esi:deliver(SessionID, J).

get_profile(SessionID,_Data2,Data3)->
  Query=httpd:parse_query(Data3),
  Id=list_to_integer(get_query_param(Query,"id")),
  mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n"),
  F=fun()->
        qlc:e(qlc:q([Data||Data<-mnesia:table(profile), element(2, Data)=:=Id]))
    end,
  {atomic,Data}=mnesia:transaction(F),
  [F2|_]=reformat(Data,[]),
  J=mochijson:encode(F2),
  mod_esi:deliver(SessionID, J).

set_profile(SessionID,Data2,Data3)->
  Query=httpd:parse_query(Data3),
  io:format("DataSetProfile:~p~n Get:~p~n",[Data2, Query]),
 mod_esi:deliver(SessionID, "Content-Type:text/plain\r\n\r\n").

get_query_param([],_Param)->
  {error,param_not_found};
get_query_param([H|T],Param)->
  if element(1,H)=:=Param ->element(2,H);
    true->get_query_param(T,Param)
  end.