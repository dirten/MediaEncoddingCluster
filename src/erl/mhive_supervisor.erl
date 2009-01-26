-module(mhive_supervisor).
-behaviour(supervisor).
-export([start/0,start_in_the_shell/0, start_link/1,init/1]).


start()->
  spawn(fun() ->supervisor:start_link({local,?MODULE},?MODULE, _Arg=[])end).

start_in_the_shell()->
  {ok,Pid}=supervisor:start_link({local,?MODULE},?MODULE, _Arg=[]),
  unlink(Pid).

start_link(Args)->
  supervisor:start_link({local,?MODULE},?MODULE, Args).

init([])->
  {ok,{{one_for_one,3,10},
  [{tag1,
    {my_gen_server,start_link,[]},
  permanent,
  10,
  worker,
  [my_gen_server]}]}}.

