-module(config).
-include("schema_config.hrl").
-compile(export_all).


get(Key)->get(Key,[]).
get(Key, Def)->
  case application:get_env(mhive,Key) of
    {ok, Value}->Value;
    _->
      C = #config{val = '$1', key=Key},
      E=mnesia:dirty_select(config,[{C,[],['$1']}]),
      if
        length(E) > 0 ->
          [Val|_]=E,
          Val;
        true->
          Def
      end
  end
.

set(Key, Val)->
  mnesia:dirty_write(#config{key=Key, val=Val}),
  Val.
