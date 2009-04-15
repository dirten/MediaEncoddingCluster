-module(auto_update).


-compile(export_all).



init([])->
  ok.

run()->
  update().

update()->
  {ok,{_Status, _Head, Body}}=http:request("http://www.codergrid.de/releases/mhive.CURRENT"),
  ServerVersion=string:strip(Body, both, $\n),
  CurrentVersion=libcode:get_mhive_version(),
  if ServerVersion/=CurrentVersion->
      io:format("New Version available:~p~n",[ServerVersion]),
      download(ServerVersion),
      install(ServerVersion);
    true->ok
  end.

download(Version)->
  {ok,{_Status, _Head, Body}}=http:request("http://www.codergrid.de/releases/mhive-update-"++Version++".tar.gz"),
  Body,
  {ok,Pid}=file:open(filename:join([code:root_dir(),"releases","mhive-update-"++Version++".tar.gz"]), write),
  file:write(Pid, Body),
  file:close(Pid).

install(Version)->
  {ok, Version}=release_handler:unpack_release("mhive-update-"++Version),
  {ok,_OtherVsn, _Desc}=release_handler:install_release(Version),
  ok=release_handler:make_permanent(Version).
  