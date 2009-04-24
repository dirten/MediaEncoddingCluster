-module(web_index).

-include ("wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).


main() ->
  {ok,Val}=application:get_env(wwwroot),
  Temp=string:concat(Val,"/onecolumn.html"),
  #template { file=Temp, bindings=[
      {'Group', main},
      {'Item', main}
                                  ]}.
title() -> "Welcome".

reformat([], Acc)->Acc;
%reformat()
reformat([H|T], Acc) when is_tuple(H)->
  NewAcc=[{"cell",tuple_to_list(H)}]++Acc,
  reformat(T, NewAcc).

body()->
    Data=libdb:read(file),
    F={struct, reformat(Data,[])},
    io:format("Data:~p",[F]),
%    mod_esi:deliver(SessionID, "This new format is nice12<BR>"),
    J= mochijson:binary_encode(F),
    io:format("JSON:~p",[J]),
  J,mochijson:binary_encode({struct,[{"cell",["file",2,"ChocolateFactory.avi","/media/out/Test_Profile","undefined"]},{"cell",["file",2,<<"ChocolateFactory.avi">>,<<"/media/out/Test_Profile">>,<<"undefined">>]}]}).
  