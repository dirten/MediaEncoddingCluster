-module(scheduler).

-compile(export_all).


init([])->
  ok.


get_timer(Name)->
  get_timer(get_timer_list(), Name).

get_timer([],_Name)->{error,timer_not_found};
get_timer([Timer|T],Name)->
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
  create_interval(List, Name, Time, Mod, Func, Args).

create_interval(List,Name, Time,Mod, Func, Args)->
  {ok,Ref}=timer:apply_intervall(Time, Mod, Func, Args),
  NewList=lists:append(List,[{Name,Ref}]),
  put_timer_list(NewList).
