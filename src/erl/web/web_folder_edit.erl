-module(web_folder_edit).

-include ("wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).


main() ->
    {ok,Val}=application:get_env(wwwroot),
    Temp=string:concat(Val,"/onecolumn.html"),
    #template { file=Temp, bindings=[
	{'Group', media},
	{'Item', folder}
]}.
title() -> "Profile Edit".


body()->
    io:format("PostData:~s",[wf:q(id)]),
    #hr{}.