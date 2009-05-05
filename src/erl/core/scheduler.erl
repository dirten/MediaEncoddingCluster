
-module(scheduler).
-include("schema.hrl").
-compile(export_all).
-export([start_link/0, init/1, handle_call/3, handle_cast/2, handle_info/2, code_change/3, terminate/2]).
-behaviour(gen_server).

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

init([])->
  List=libdb:read(scheduler),
  create_timer_from_db(List),
   {ok, started}.


create_timer_from_db([])->ok;
create_timer_from_db([S|T])->
%  io:format("Schedule:~p",[S]),
  create_interval(S#scheduler.name,S#scheduler.interval,S#scheduler.module,S#scheduler.func,S#scheduler.args),
  create_timer_from_db(T).

get_timer(Name)->
  get_timer(get_timer_list(), Name).


cancel_timer(Name)->
  case get_timer(Name) of
    {error,timer_not_found}->{error,timer_not_found};
    {ok,{Name,{interval,Ref}}}->
      timer:cancel(Ref),
      remove_timer({Name,{interval,Ref}});
    Any->io:format("Any:~p",[Any])
  end.

remove_timer(Data)->
  OldList=get_timer_list(),
  NewList=lists:subtract(OldList,[Data]),
  put_timer_list(NewList).


get_timer([],_Name)->{error,timer_not_found};
get_timer([Timer|T],Name)->
  %  io:format("Timer:~p",[Timer]),
  if element(1,Timer)=:=Name ->{ok,Timer};
    true->
      get_timer(T,Name)
  end.

get_timer_list()->
  case get(timer) of
    undefined->[];
    D->D
  end.

put_timer_list(List)->
  put(timer,List).

create_interval(Name, Time,Mod, Func, Args)->
  List=get_timer_list(),
  NewList=create_interval(List, Name, Time, Mod, Func, Args),
  put_timer_list(NewList).

create_interval(List,Name, Time,Mod, Func, Args)->
  {ok,Ref}=timer:apply_interval(Time, Mod, Func, Args),
  lists:append(List,[{Name,Ref}]).
  
handle_call({timer_list},_From,_N)->
  io:format("GetTimerList~n",[]),
  Timer=get_timer_list(),
  {reply, Timer, state};
handle_call(Thing,_From,_N)->
  io:format("~w handle_call unknown~w~n", [?MODULE,Thing]),
  {reply, Thing, state}.


handle_cast(_Msg,N)->
  io:format("~w handle_cast~w~n", [?MODULE,N]),
  {noreply, N}.

handle_info(Info,N)->
  io:format("~p handle_info~w~n", [?MODULE,{Info,N}]),
  {noreply, N}.

terminate(Reason,_N)->
  file_scanner_loop ! stop,
  io:format("~w shutdown ~w~n", [?MODULE, Reason]),
  ok.

code_change(_OldVsn,N,_Extra)->
  io:format("~w CodeChange ~n", [?MODULE]),
  {ok, N}.

