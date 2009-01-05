-module(client).
-export([start/1, listener/1]).




start(Server)->
  io:format("Start Client~n", []),
  %  spawn(client,listener,[Server]),
  listener(Server),
  io:format("Client started~n", []).
  



listener(Server)->
  %  io:format("Client listenerLoop ~w~n", [Server]),
  {complex,Server} ! {call, self(),{foo, "test"}},
  receive    
    stopClient ->
      io:format("Client stopped~n", []);
    {server, P, Data} ->
      %      io:format("{server, P, Data}~w~n", [Data]),
      P ! {clientdata,lists:reverse(Data)},
      listener(Server);
    {complex, Result} ->
      %      io:format("Data: ~w~n", [Result]),
      listener(Server)
  after 5000 ->
      exit(port_terminated)

  end.
