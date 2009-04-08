%% @DOC bla fasel
-include("config.hrl").
-module(file_port).
-export([start/1, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2, handle_info/2, code_change/3, terminate/2]).
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
  BinPath=libcode:get_mhivesys_exe(),
  io:format("Starting ~w with ~p~n", [?MODULE,BinPath]),
  global:register_name(packet_sender, self()),
  %  register(fileimport, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, BinPath}, [{packet, 4}, binary]),
  link(Port),
  register(fileport,Port),
  io:format("FileImporter started~n", []),
  {ok, state}.

handle_call({Command,File,Stream,Seek,PacketCount},_From,_N)->
%  io:format("~w-handle_call~p~n", [?MODULE,{Command,File,Stream,Seek,PacketCount}]),
  fileport ! {self(), {command, term_to_binary({Command,list_to_atom(File),Stream,Seek,PacketCount})}},
  receive
    {_Fileport, {data, Data}} ->
      D=binary_to_term(Data),
      %io:format("Any Data ~w",[D]),
      {reply, D, state};
    {'EXIT', _Fileport, Reason2} ->
      io:format("Port exited  ~w~n", [Reason2]),
      exit({normal, Reason2})
  after 40000 ->
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
      io:format("handle_info(,N)~w~n", [?MODULE])

  end,
  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~p terminate~n",[?MODULE]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.


