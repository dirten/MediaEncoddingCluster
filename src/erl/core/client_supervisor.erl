-module(client_supervisor).
-behaviour(supervisor).
-export([start/0,start_in_the_shell/0, start_link/1,init/1]).


start()->
  spawn(fun() ->supervisor:start_link({local,?MODULE},?MODULE, _Arg=[])end).

start_in_the_shell()->
  {ok,Pid}=supervisor:start_link({local,?MODULE},?MODULE, _Arg=[]),
  unlink(Pid).

start_link(Args)->
  supervisor:start_link({local,?MODULE},?MODULE, Args).

init(_Args)->
  {ok,
   {
     {one_for_one,3,10},
     [
%     {node_finder,{node_finder,start_link,[]},  permanent,  10,  worker,  [node_finder]},
     {encoding_client,{encoding_client,start_link,[]},  permanent,  10,  worker,  [encoding_client]},
     {webserver, {nitrogen,start,[]},  permanent,  10,  supervisor,  [webserver]},
     {node_watcher, {node_watcher,start_link,[]},  permanent,  10,  worker,  [node_watcher]}
     ]
   }
  }.

