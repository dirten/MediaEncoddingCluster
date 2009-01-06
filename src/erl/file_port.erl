-module(file_port).
-export([start/1,stop/0, init/1, handle_call/3, handle_cast/2,handle_info/2, code_change/3, terminate/2]).
-behaviour(gen_server).

start(Dir)->
  spawn(?MODULE,init,[Dir]).

stop()->
  fileimport ! stop.

init([])->
  io:format("Starting FileImporter~n", []),
  register(fileimport, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, "C:/devel/MediaEncodingCluster-build/src/erl/Debug/erltest"}, [{packet, 2}, binary]),
  io:format("FileImporter started~n", []),
  loop(Port,[]).

handle_call({import_file,_Thing},_From,_N)->
  ok.

handle_cast(_Msg,_N)->
  ok.

handle_info(_Info,_N)->
  ok.

terminate(_Reason,_N)->
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.

loop(Port, C) ->
  receive
    {call, Caller, Msg} ->
      Port ! {self(), {command, term_to_binary(Msg)}},
      loop(Port, Caller);
    {Port, {data, Data}} ->
      D=binary_to_term(Data),
      C ! {fileimport, D},
      loop(Port,C);
    stop ->
      io:format("StopSignal~n", []),
      Port ! {self(), close},
      loop(Port,C);
    {Port, closed} ->
      unregister(fileimport),
      exit(normal);
    {'EXIT', Port, Reason} ->
      unregister(fileimport),
      io:format("Port exited  ~w~n", [Reason]),
      exit({port_terminated, Reason})
  end.
