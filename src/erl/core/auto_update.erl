-module(auto_update).
-include("schema.hrl").
-define(UPDATEURL,"http://www.codergrid.de/releases/").
-compile(export_all).
-include_lib("stdlib/include/qlc.hrl").



init([])->
  ok.

run()->
  update().

update()->
  {ok,{_Status, _Head, Body}}=http:request(?UPDATEURL++"mhive.CURRENT"),
  {ok, Tokens,_}=erl_scan:string(Body),
  {ok,Releases}=erl_parse:parse_term(Tokens),
  F=fun()->
        (qlc:e(qlc:q([V#releases.version||V<-mnesia:table(releases)])))
    end,
  {atomic,LocalVersions}=mnesia:transaction(F),
  RemoteVersions=[element(1,D)||D<-Releases],
  %NewVersions=[{Remote, Local}||Remote<-NewUpdates, Local<-LocalVersions],
  NewVersions=RemoteVersions--LocalVersions,
  %  io:format("NewData123:~p",[Releases]),
  [process(Release)||Version<-NewVersions, Release<-Releases, Version=:=element(1,Release)].
%io:format("DBData:~p",[LocalVersions]),
%ServerVersion=string:strip(Body, both, $\n),
%  CurrentVersion=libcode:get_mhive_version().
%  if ServerVersion/=CurrentVersion->
%      io:format("New Version available:~p~n",[ServerVersion]);
%      download(ServerVersion);
%      install(ServerVersion);
%    true->ok
%  end.

process(Release)->
  io:format("Processing Release:~p~n",[Release]),
  Status = case download(element(1,Release)) of
    {ok,_Version}->
      downloaded;
    {error,What}->
      What
    end,
  R=#releases{
    version=element(1,Release),
    application=element(2,Release),
    desc=element(3,Release),
    status=Status
  },
  libdb:write(R).


download(Version)->
  io:format("Downloading Version ~p",[Version]),
  {ok,{{_HttpVersion,Code,_Msg}, _Head, Body}}=http:request(?UPDATEURL++"mhive-update-"++Version++".tar.gz"),
  io:format("Status:~p",[Code]),
  case Code of
    200->
      {ok,Pid}=file:open(filename:join([code:root_dir(),"releases","mhive-update-"++Version++".tar.gz"]), write),
      file:write(Pid, Body),
      file:close(Pid),
      {ok,Version};
    _->{error,release_not_found}
  end.


install(Version)->
  {ok, Version}=release_handler:unpack_release("mhive-update-"++Version),
  {ok,_OtherVsn, _Desc}=release_handler:install_release(Version),
  ok=release_handler:make_permanent(Version).
  