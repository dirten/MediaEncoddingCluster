-module(file_scanner).
-behaviour(gen_server).

-export([start/0, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2,handle_info/2, code_change/3, terminate/2]).

start()->
  spawn(?MODULE,init,[[]]).

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

stop()->
  gen_server:call({local,?MODULE}, stop).

init([])->
  io:format("~w start up~n", [?MODULE]),
  process_flag(trap_exit, true),
  {ok, state}.

handle_call({import_file,Thing},_From,_N)->
  io:format("~w handle_call~w~n", [?MODULE,Thing]),
  {reply, Thing, state}.


handle_cast(_Msg,N)->
  io:format("~w handle_cast~w~n", [?MODULE,N]),
  {noreply, N}.

handle_info(Info,N)->
  io:format("~w handle_info~w~n", [?MODULE,{Info,N}]),
  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~w shutdown~n", [?MODULE]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.

