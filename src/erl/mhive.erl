-module(mhive).
-behaviour(application).
-export([start/2, stop/1, startapp/0, stopapp/0, configure/0]).
-include("schema.hrl").

startapp()->
  Node=libnet:local_name(),
  Self=libnet:local_name("starter"),
  net_kernel:start([Self]),
  S=rpc:call(Node,application,start,[sasl]),
  io:format("Startup ~p",[S]),
  M=rpc:call(Node,application,start,[mhive]),
  io:format("Startup ~p",[M]),
  init:stop().

stopapp()->
  Node=libnet:local_name(),
  Self=libnet:local_name("stopper"),
  io:format("Using Local Name ~p~n",[Node]),
  net_kernel:start([Self]),
  R=rpc:call(Node,init,stop,[]),
  io:format("Shutdown ~p",[R]),
  init:stop().

configure()->
  setup:setup().
%  init:stop().

start(_Type, StartArgs)->
  Node=libnet:local_name(),
  io:format("Self node = ~p",[Node]),
%  net_kernel:start([Node]),
%  net_adm:world(),
  %  application:set_env(mhive,wwwroot,filename:join(libcode:get_privdir(),"wwwroot")),
  %% TODO libcode wieder einbinden
  %%application:set_env(mhive,wwwroot,"wwwroot"),

  case config:get(mode) of
    server->
      mnesia:start(),
      mnesia:wait_for_tables([config, scheduler],5000),
      application:set_env(mhive,port,config:get(http_port,8080)),
      mhive_supervisor:start_link(StartArgs);
    client->
      client_supervisor:start_link(StartArgs);
    both->
      mnesia:start(),
      mnesia:wait_for_tables([config, scheduler],5000),
      application:set_env(mhive,port,config:get(http_port,8080)),
      mhive_supervisor:start_link(StartArgs),
      client_supervisor:start_link(StartArgs);
    _->system_not_configured
  end.


stop(_State)->
  %  nitrogen:stop(),
  %  net_kernel:stop(),
  ok.
