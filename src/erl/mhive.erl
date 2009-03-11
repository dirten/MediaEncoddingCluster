-module(mhive).
-behaviour(application).
-export([start/2, stop/1, startapp/0]).
-include("schema.hrl").

startapp()->
  start([],[]).

start(_Type, StartArgs)->
  {ok,Data}=inet:getaddr(net_adm:localhost(), inet),
  {A1,A2,A3,A4}=Data,
  Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
  Node=list_to_atom("node@"++Ip),
  net_kernel:start([Node]),
  net_adm:world(),
  mnesia:create_schema([node()]),
  mnesia:start(),
  mnesia:wait_for_tables([config],1000),
  libdb:create(),
  application:set_env(mhive,port,config:get(http_port,8080)),
  mhive_supervisor:start_link(StartArgs).

stop(_State)->
%  nitrogen:stop(),
%  net_kernel:stop(),
  ok.
