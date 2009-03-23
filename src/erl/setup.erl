-module(setup).
-compile(export_all).
-include("schema.hrl").


setup()->
  io:format("Configure Cluster Environment~n"),
  Mode=read_data("Cluster Instance Mode {server | client | both}: ",[server,client,both]),
  Auto=read_data("Cluster Instance Autostart at System Boot {yes | no}: ",[yes,no]),
  setup_linux(Mode,Auto).

setup_linux(Mode,Auto)->
  setup_config(Mode),
  setup_auto_start(Auto),
  ok.

setup_win32(Mode, _Auto)->
	[ErtsPath|_]=filelib:wildcard(code:root_dir()++"/erts*"),
	setup_config(Mode),
      Node=libnet:local_name(),
	R=os:cmd(lists:concat([ErtsPath,"/bin/erlsrv add MHiveService -w ",code:root_dir()," -name ",Node," -d console -args \"-mnesia dir 'data' -setcookie default -config releases/0.0.4.1/sys -boot releases/0.0.4.1/start\""])),
	R.

setup_auto_start(AutoStart)->
  case AutoStart of
    yes->
      {ok,Base}=file:get_cwd(),
      _Res=libfile:symlink(filename:join(Base,"bin/mectl"),"/etc/rc.d/rc5.d/S75mectl"),
      _Res=libfile:symlink(filename:join(Base,"bin/mectl"),"/etc/rc.d/rc5.d/K03mectl");
	_->
      ok
  end,
  ok.

setup_config(Mode)->
  Node=libnet:local_name(),
  io:format("Using Local Name ~p~n",[Node]),
  net_kernel:start([Node]),
  mnesia:create_schema([node()]),
  mnesia:start(),
  mnesia:wait_for_tables([config],1000),
  libdb:create(),
  mnesia:dirty_write(#config{key=mode, val=Mode}).

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

