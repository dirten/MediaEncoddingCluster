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
