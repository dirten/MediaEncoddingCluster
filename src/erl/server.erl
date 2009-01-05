-module(server).
-export([start/0, stop/0, listener/0, init/0,process/0]).


stop()->
  myserver ! stopServer,
  complex ! stop.

start()->
  spawn(?MODULE,init,[]).

process()->
  complex ! {call, self(),{foo, "test"}},
    receive
      {complex, Result} ->
      io:format("Data: ~w~n", [Result])
     end.


init()->
  io:format("Starting the Server~n", []),
  register(myserver,spawn(server,listener,[])),
  register(complex, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, "/home/jhoelscher/MediaEncodingCluster/build/src/erl/erltest"}, [{packet, 2}, binary]),
  io:format("Server started~n", []),
  loop(Port).
  
listener()->
  receive
    stopServer ->
      io:format("Server stopped~n", []),
      unregister(myserver),
      unregister(complex),
      io:format("Server unregistered~n", []);
    {client, Pclient} ->
      %      io:format("{client, Pclient}~n", []),
      Pclient ! {server,self(),[1,2,3,4,5,6,7,8,9]},
      listener();
    {clientdata, Data} ->
      %      io:format("Client Answer ~w~n", [Data]),
      listener()
  end.

loop(Port) ->
  receive
    {call, Caller, Msg} ->
      Port ! {self(), {command, term_to_binary(Msg)}},
%      io:format("Data Sended~n", []),
      receive
        {Port, {data, Data}} ->
%        io:format("Data Received ~w~n", [Data]),
          Caller ! {complex, binary_to_term(Data)}
      end,
      loop(Port);
%    {Port, {data, Data}} ->
%      io:format("Data Received~n", []);
%      write( {complex, binary_to_term(Data)});
    stop ->
      io:format("StopSignal~n", []),
      Port ! {self(), close},
      receive
        {Port, closed} ->
          exit(normal)
      end;
    {'EXIT', Port, Reason} ->
      exit(port_terminated)
  end.
  