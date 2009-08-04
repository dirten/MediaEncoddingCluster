-module(config).
-include_lib("stdlib/include/qlc.hrl").
-include("schema_config.hrl").
-compile(export_all).

get([])->
    F=fun()->
            qlc:e(qlc:q([{Key, Val}||{_Name, Key, Val}<-mnesia:table(config)]))
      end,
    {atomic,Data}=mnesia:transaction(F),
    Data,

    lists:filter(
        fun(Entry)->
                if
                    element(1,Entry)=:=included_applications->false;
                    true->true
                end
        end,application:get_all_env(mhive)++Data);
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
    libdb:write(#config{key=Key, val=Val}),
    Val.
