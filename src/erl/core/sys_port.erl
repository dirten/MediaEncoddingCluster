%% @DOC bla fasel
-include("../include/config.hrl").
-module(sys_port).
-export([start/1, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2, handle_info/2, code_change/3, terminate/2]).
-behaviour(gen_server).

start(Dir)->
  spawn(?MODULE,init,[Dir]),
  ok.

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

stop()->
  global:whereis_name(hive_system_info) ! stop,
  ok.

init([])->
  {ok,SysPortCommand}=application:get_env(sysportexe),
  io:format("Starting ~s ~s ~n", [?MODULE, SysPortCommand]),
  global:register_name(hive_system_info, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, SysPortCommand}, [{packet, 4}, binary]),
  link(Port),
  register(mhive_sys_port,Port),
  io:format("~s started~n", [?MODULE]),
  {ok, state}.

handle_call({Command},_From,_N)->
  mhive_sys_port ! {self(), {command, term_to_binary({Command})}},
  receive
    {_Fileport, {data, Data}} ->
      D=binary_to_term(Data),
      {reply, D, state}
  after 4000 ->
      io:format("System Port Timeout~n",[]),
      {reply, sysport_timeout, state}
  end.



handle_cast(_Msg,N)->
  io:format("handle_cast(Msg,N)~n", []),
  {noreply, N}.

handle_info(Info,N)->
  case Info of
    {'EXIT', _Fileport, Reason2} ->
      io:format("Port exited  ~w~n", [Reason2]),
      exit({normal, Reason2});
    _->
      io:format("handle_info(,N)~w~n", [Info])
  end,
  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~p terminate~n",[?MODULE]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.

