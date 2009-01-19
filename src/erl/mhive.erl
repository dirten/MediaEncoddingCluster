-module(mhive).
-behaviour(application).
-export([start/2, stop/1]).

start(_Type, StartArgs)->
  mhive_supervisor:start_link(StartArgs).

stop(_State)->
  ok.


