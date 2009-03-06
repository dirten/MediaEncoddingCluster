-module (web_system).
-include ("nitrogen/include/wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).

main() ->
  {ok,Val}=application:get_env(wwwroot),
  Temp=string:concat(Val,"/onecolumn.html"),
  #template { file=Temp, bindings=[
    {'Group', media},
    {'Item', system}
                                  ]}.
title() -> "System Overview".
%headline() -> "".
%right() -> "ok".%linecount:render().



