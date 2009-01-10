%% @DOC bla fasel
-include("config.hrl").
-module(file_port).
-export([start/1,stop/0, init/1, handle_call/3, handle_cast/2,handle_info/2, code_change/3, terminate/2]).
-behaviour(gen_server).

start(Dir)->
  spawn(?MODULE,init,[Dir]),
  ok.

stop()->
  global:whereis_name(packet_sender) ! stop.
  ok.

init([])->
%  io:format("Starting FileImporter~n", []),
  global:register_name(packet_sender, self()),
  %  register(fileimport, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, ?FILEPORTEXE}, [{packet, 4}, binary]),
%  io:format("FileImporter started~n", []),
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
    {call, Caller} ->
      io:format("{call, Caller}  ~w~n", [Caller]),
      Port ! {self(), {command, term_to_binary({packetgroup, 'c:/2video/heute.ts',0, -1, 12})}},
      loop(Port, Caller);
    {call, Caller, Msg} ->
      Port ! {self(), {command, term_to_binary(Msg)}},
      loop(Port, Caller);
    {Port, {data, Data}} ->
      D={{0,0,0,17,25,1,25,512,256,0,12,0},{0,0,0,17,25,1,25,512,256,0,12,0},binary_to_term(Data)},
      io:format("{Port, {data, Data}}  ~w~n", [D]),
      C ! {packet_sender, D},
      loop(Port,C);
    stop ->
%      io:format("StopSignal~n", []),
      Port ! {self(), close},
      loop(Port,C);
    {Port, closed} ->
      io:format("Port exited  ~w~n", []),
      global:unregister_name(packet_sender),
      exit(normal);
    {'EXIT', Port, Reason} ->
      global:unregister_name(packet_sender),
%      io:format("Port exited  ~w~n", [Reason]),
      exit({port_terminated, Reason})
  end.
