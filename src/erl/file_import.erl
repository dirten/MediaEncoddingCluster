-module(file_import).
-export([start/1,stop/0, init/1, handle_call/3, handle_cast/2,handle_info/2, code_change/3, terminate/2]).
-behaviour(gen_server).

start(Dir)->
  spawn(?MODULE,init,[[]]).

stop()->
  fileimport ! stop.

init([])->
  io:format("Starting FileImporter~n", []),
  register(fileimport, self()),
  process_flag(trap_exit, true),
  %  Port = open_port({spawn, "/home/jhoelscher/MediaEncodingCluster/build/src/erl/erltest"}, [{packet, 2}, binary]),
  %  Port = open_port({spawn, "C:/devel/MediaEncodingCluster-build/src/erl/RelWithDebInfo/erltest"}, [{packet, 2}, binary]),
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
      io:format("Sending Message ~w~n", [Msg]),
      Port ! {self(), {command, term_to_binary(Msg)}},
      %      io:format("Data Sended~n", []),
      loop(Port, Caller);
    {Port, {data, Data}} ->
      D=binary_to_term(Data),
      %          io:format("Data Received ~w~n", [Data]),
      C ! {fileimport, D},
      loop(Port,C);
    %    {Port, {data, Data}} ->
    %      io:format("Data Received~n", []);
    %      write( {complex, binary_to_term(Data)});
    stop ->
      io:format("StopSignal~n", []),
      Port ! {self(), close},
      receive
        {Port, closed} ->
          unregister(fileimport),
          exit(normal)
      end;
    {'EXIT', Port, Reason} ->
      unregister(fileimport),
      io:format("Port exited abnormal~n", []),
      exit({port_terminated, Reason})
  end.
