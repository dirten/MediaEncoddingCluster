-module(mhive).
-behaviour(application).
-export([start/2, stop/1, startapp/0, configure/0]).
-include("schema.hrl").

startapp()->
  start([],[]).

configure()->
  io:format("Configure Cluster Environment~n"),
  Mode=read_data("Cluster Instance Mode {server | client | both}: ",[server,client,both]),
  Auto=read_data("Cluster Instance Autostart at System Boot {yes | no}: ",[yes,no]),
%  io:format("Mode:~p~n",[Mode]),
  case Mode of
    "client"->
      Server=read_data("Cluster Server Instance {IP Address from Server}: ",[]);
      _->
        ok
      end.

read_data(Text,Values)->
  {ok, [Data|_]}=io:fread(Text,"~s"),
    if length(Values)>0->
      case lists:member(list_to_atom(string:to_lower(Data)),Values) of
        true->
          Data;
        false->
          print_failure(Data,[yes,no]),
          read_data(Text, Values)
      end;
      true->Data
    end.

print_failure(Value, Valid)->
  io:format("~nWrong value given ~p, valid values are ~p~n",[Value, Valid]).

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
  case application:get_env(mhive,mode) of
    {ok, server}->
      mhive_supervisor:start_link(StartArgs);
    {ok, client}->
      client_supervisor:start_link(StartArgs);
    {ok, both}->
      mhive_supervisor:start_link(StartArgs),
      client_supervisor:start_link(StartArgs);
    _->wrong_mode_in_config
  end.


stop(_State)->
%  nitrogen:stop(),
%  net_kernel:stop(),
  ok.
