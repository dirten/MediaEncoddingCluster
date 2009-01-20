%% @DOC bla fasel
%-include("config.hrl").
-module(my_gen_server).
-export([start/0, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2,handle_info/2, code_change/3, terminate/2]).
-behaviour(gen_server).

start()->
  spawn(?MODULE,init,[[]]).
  

start_link()->
  gen_server:start_link({global,?MODULE},?MODULE,[],[]).

stop()->
  gen_server:call({global,?MODULE}, stop).

init([])->
  net_kernel:monitor_nodes(true),
  {ok, state}.

handle_call({import_file,Thing},_From,_N)->
  io:format("FileImporter handle_call~w~n", [Thing]),
  {reply, test, state}.


handle_cast(_Msg,N)->
  io:format("FileImporter handle_cast~w~n", [N]),
  {noreply, N}.

handle_info(Info,N)->
  io:format("FileImporter handle_info~w~n", [{Info,N}]),
  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~pstopping packet_sender~n",[?MODULE]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.

