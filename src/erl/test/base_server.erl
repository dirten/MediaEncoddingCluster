-module(base_server).
-behaviour(gen_server).


start()->
  spawn(?MODULE,init,[[]]).

start_link()->
  gen_server:start_link({global,?MODULE},?MODULE,[],[]).

stop()->
  gen_server:call({global,?MODULE}, stop).

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

