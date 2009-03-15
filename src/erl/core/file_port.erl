%% @DOC bla fasel
-include("config.hrl").
-module(file_port).
-export([start/1, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2, handle_info/2, code_change/3, terminate/2, loop/2]).
-behaviour(gen_server).

start(Dir)->
  spawn(?MODULE,init,[Dir]),
  ok.

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

stop()->
  global:whereis_name(packet_sender) ! stop,
  ok.

init([])->
  {ok,SysPortCommand}=application:get_env(sysportexe),
  io:format("Starting ~w with ~p~n", [?MODULE,SysPortCommand]),
  global:register_name(packet_sender, self()),
  %  register(fileimport, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, SysPortCommand}, [{packet, 4}, binary]),
  link(Port),
  register(fileport,Port),
  io:format("FileImporter started~n", []),
  %  spawn(?MODULE,loop,[Port,[]]),
  {ok, state}.

handle_call({Command,File,Stream,Seek,PacketCount},_From,_N)->
  io:format("~w-handle_call~p~n", [?MODULE,{Command,File,Stream,Seek,PacketCount}]),
  fileport ! {self(), {command, term_to_binary({Command,list_to_atom(File),Stream,Seek,PacketCount})}},
  receive
    {_Fileport, {data, Data}} ->
      D=binary_to_term(Data),
      %io:format("Any Data ~w",[D]),
      {reply, D, state}
      %    {'EXIT', _Fileport, Reason2} ->
      %        global:unregister_name(packet_sender),
      %          io:format("Port exited  ~w~n", [Reason2]),
      %         exit({normal, Reason2})
  after 4000 ->
      io:format("FilePort Timeout~n",[]),
      {reply, hivetimeout, state}
  end.



handle_cast(_Msg,N)->
  io:format("handle_cast(Msg,N)~n", []),
  {noreply, N}.

handle_info(Info,N)->
%  io:format("handle_info(~w,N)~n", [Info]),
  case Info of
    {'EXIT', _Fileport, Reason2} ->
      io:format("Port exited  ~w~n", [Reason2]),
      exit({normal, Reason2});
     _->
         io:format("handle_info(,N)~w~n", [Info])

   end,
  {noreply, N}.

terminate(_Reason,_N)->
  %  global:unregister_name(packet_sender, self()),
  io:format("~p terminate~n",[?MODULE]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.

loop(Port, C) ->
  receive
    {call, Caller} ->
      io:format("{call, Caller}  ~w~n", [Caller]),
      Port ! {self(), {command, term_to_binary({packetgroup, 'c:/2video/heute.ts',0, -1, 12})}},
      loop(Port, Caller);
    {call, Caller, Msg} ->
      io:format("{call, Caller, Msg}  ~w~n", [Caller]),
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
      io:format("Port exited  ~n", []),
      global:unregister_name(packet_sender),
      exit(normal);
    {'EXIT', Port, Reason} ->
      global:unregister_name(packet_sender),
      io:format("Port exited  ~w~n", [Reason]),
      exit({port_terminated, Reason})
  end.
