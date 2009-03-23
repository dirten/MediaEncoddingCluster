-module(mhive).
-behaviour(application).
-export([start/2, stop/1, startapp/0, stopapp/0, configure/0, configure/2]).
-include("schema.hrl").

startapp()->
  start([],[]).

stopapp()->
  Node=libnet:local_name(),
  Self=libnet:local_name("stopper"),
  io:format("Using Local Name ~p~n",[Node]),
  net_kernel:start([Self]),
  R=rpc:call(Node,init,stop,[]),
  io:format("Shutdown ~p",[R]),
  init:stop().

configure()->
  io:format("Configure Cluster Environment~n"),
  Mode=read_data("Cluster Instance Mode {server | client | both}: ",[server,client,both]),
  Auto=read_data("Cluster Instance Autostart at System Boot {yes | no}: ",[yes,no]),
  %  io:format("Mode:~p~n",[Mode]),
  case Mode of
    "client_not_now"->
      _Server=read_data("Cluster Server Instance {IP Address from Server}: ",[]);
    _->
      ok
  end,
  configure(list_to_atom(Mode), list_to_atom(Auto)),
init:stop().

configure(Mode, Auto) when is_atom(Mode) and is_atom(Auto)->
  Node=libnet:local_name(),
  io:format("Using Local Name ~p~n",[Node]),
  net_kernel:start([Node]),
%  net_adm:world(),
  mnesia:create_schema([node()]),
  mnesia:start(),
  mnesia:wait_for_tables([config],1000),
  libdb:create(),
  mnesia:dirty_write(#config{key=mode, val=Mode}),
  case Auto of
    yes->
      {ok,Base}=file:get_cwd(),
      _Res=libfile:symlink(filename:join(Base,"bin/mectl"),"/etc/rc.d/rc5.d/S75mectl"),
      _Res=libfile:symlink(filename:join(Base,"bin/mectl"),"/etc/rc.d/rc5.d/K03mectl");
%      io:format("Copy ~p",[Res]);
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

%  {ok,Data}=inet:getaddr(net_adm:localhost(), inet),
%  {A1,A2,A3,A4}=Data,
%  Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
%  list_to_atom("node@"++Ip),
  Node=libnet:local_name(),
  net_kernel:start([Node]),
  net_adm:world(),
%  mnesia:create_schema([node()]),
  mnesia:start(),
  mnesia:wait_for_tables([config],1000),
%  libdb:create(),
  application:set_env(mhive,port,config:get(http_port,8080)),
  application:set_env(mhive,wwwroot,filename:join(libcode:get_privdir(),"wwwroot")),
  
  case config:get(mode) of
    server->
      mhive_supervisor:start_link(StartArgs);
    client->
      client_supervisor:start_link(StartArgs);
    both->
      mhive_supervisor:start_link(StartArgs),
      client_supervisor:start_link(StartArgs);
    _->system_not_configured
  end.


stop(_State)->
  %  nitrogen:stop(),
  %  net_kernel:stop(),
  ok.
