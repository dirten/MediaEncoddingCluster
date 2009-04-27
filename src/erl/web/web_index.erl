
  -module(web_index).

-include ("wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).


main() ->
  {ok,Val}=application:get_env(wwwroot),
  Temp=string:concat(Val,"/empty.html"),
  #template { file=Temp, bindings=[
    {'Group', main},
    {'Item', main}
                                  ]}.
title() -> "Welcome".



record_to_json(_R,[],_C,Acc)->
  Acc;

record_to_json(Record,[H|T],C,Acc)->
  NewAcc=[{H,element(C, Record)}]++Acc,
  record_to_json(Record,T,(C+1),NewAcc).

record_to_json(Record)->
  Fields=libdb:columns(element(1, Record)),
  lists:reverse(record_to_json(Record, Fields,2,[])).



reformat([], Acc)->Acc;
reformat([H|T], Acc) when is_tuple(H)->
  NewAcc=[{struct,record_to_json(H)}]++Acc,
  reformat(T, NewAcc).
%{"page":"1","total":"101","data":[{object},{object},{object},{object},...]}

body()->
  Data=libdb:read(file),
  F=reformat(Data,[]),
  io:format("Data:~p~n",[F]),
  J= mochijson:encode({struct,[{page,1},{total,length(Data)},{data,{array,F}}]}),
  J.

body2()->
  Data=libdb:read(file),
  F={array, reformat(Data,[])},
  io:format("Data:~p~n",[Data]),
  New={file,1,"ChocolateFactory.ts","/media/video","59899904","mpegts",5,
            "141931367",3376274,undefined,undefined,"41775317889"},
          io:format("JSON:~p~n",[record_to_json(New)]),
  rfc4627:encode({struct,{data,record_to_json(New)}}).

