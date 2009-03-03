-module(mhive).
-behaviour(application).
-export([start/2, stop/1, startapp/0]).
-include("schema.hrl").

startapp()->
  start([],[]).

start(_Type, StartArgs)->
  net_kernel:start([node, shortnames]),
  mnesia:create_schema([node()]),
  mnesia:start(),
  mnesia:wait_for_tables([config],1000),
  libdb:create(),
  application:set_env(mhive,port,config:get(http_port,8080)),
  nitrogen:start(),
  mhive_supervisor:start_link(StartArgs).

stop(_State)->
  nitrogen:stop(),
  ok.
