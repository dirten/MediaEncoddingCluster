-module(config).
-include("schema_config.hrl").
-compile(export_all).


get(Key)->get(Key,[]).
get(Key, Def)->
  C = #config{val = '$1', key=Key},
  E=mnesia:dirty_select(config,[{C,[],['$1']}]),
  if
    length(E) > 0 ->
      [Val|_]=E,
    Val;
    true->
      Def
  end.

set(Key, Val)->
  mnesia:dirty_write(#config{key=Key, val=Val}),
  Val.
