-module(client).
%-compiler(export_all).
-export([fileinfo/2, streaminfo/3]).

  

fileinfo(Server,File)->
  listener(Server,fileinfo,File,0).
  %listener(Server,fileinfo,'c:/1video/20070401 1825 - PREMIERE 3 - Wes Craven praesentiert Dracula III_ Legacy (Dracula III_ Legacy).ts').

streaminfo(Server,File, Stream)->
  listener(Server,streaminfo,File, Stream).

listener(Server, Type, File, Stream)->
  %  io:format("Client listenerLoop ~w~n", [Server]),
  {fileimport,Server} ! {call, self(),{Type, File,Stream}},
  receive    
    stopClient ->
      io:format("Client stopped~n", []);
    {server, P, Data} ->
      io:format("{server, P, Data}~w~n", [Data]),
      P ! {clientdata,lists:reverse(Data)},
      listener(Server,Type,File,Stream);
    {fileimport, Result} ->
      io:format("Data: ~w~n", [Result]),
    Result
%      listener(Server,Type,File,Stream)
  after 5000 ->
      exit(port_terminated)

  end.
