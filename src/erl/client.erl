-include("config.hrl").
-module(client).
%-compiler(export_all).
-export([stop/0, start/0, init/0, fileinfo/2, streaminfo/3, packet/3, packet/4, packetgroup/4, packetgroup/5]).

start()->
  spawn(?MODULE,init,[]),
  ok.

stop()->
  encodeclient ! stop,
  ok.

init()->
  register(encodeclient, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, ?ENCODECLIENTEXE}, [{packet, 4}, binary]),
  loop(Port,[]).

fileinfo(Server,File)->
  listener(Server,fileinfo,File,0,0,0).

streaminfo(Server,File, Stream)->
  listener(Server,streaminfo,File, Stream,0,0).

packet(Server,File, Stream, Seek)->
  listener(Server,packet,File, Stream, Seek,0).

packet(Server,File, Stream)->
  listener(Server,packet,File, Stream, -1,0).

packetgroup(Server,File, Stream, PacketCount, Seek)->
  listener(Server,packetgroup,File, Stream, Seek, PacketCount).

packetgroup(Server,File, Stream, PacketCount)->
  listener(Server,packetgroup,File, Stream, -1, PacketCount).

listener(Server, Type, File, Stream, Seek, PacketCount)->
  %  io:format("Client listenerLoop ~w~n", [Server]),
  {fileimport,Server} ! {call, self(),{Type, File,Stream, Seek, PacketCount}},
  receive    
    stopClient ->
      io:format("Client stopped~n", []);
    {fileimport, Result} ->
    encodeclient ! {call, self(),{encode,Result}},
    receive
      {encodeddata, D} ->
        D
      after 5000 ->
        exit(port_terminated)
      end
%,      listener(Server,Type,File,Stream, Seek, PacketCount)
  after 5000 ->
      exit(port_terminated)
  end.

loop(Port, C) ->
  receive
    {call, Caller, Msg} ->
      Port ! {self(), {command, term_to_binary(Msg)}},
      loop(Port, Caller);
    {Port, {data, Data}} ->
      D=binary_to_term(Data),
      C ! {encodeddata, D},
      loop(Port,C);
    stop ->
%      io:format("StopSignal~n", []),
      Port ! {self(), close},
      loop(Port,C);
    {Port, closed} ->
      unregister(encodeclient),
      exit(normal);
    {'EXIT', Port, Reason} ->
      unregister(encodeclient),
%      io:format("Port exited  ~w~n", [Reason]),
      exit({port_terminated, Reason})
  end.
