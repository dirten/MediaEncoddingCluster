-module(db).
-export([init/0, delete/0, sequence/1, init_sequence/2]).
-include("schema.hrl").

init()->
	mnesia:create_table(packet,[{type, bag}, {disc_only_copies, [node()]},{attributes, record_info(fields, packet)}]),
	create_sequence().
%	mnesia:add_table_index(packet, pts).
	
delete()->
	mnesia:delete_table(packet),
	mnesia:delete_table(sequence).
	
	

%% sequence table record
-record(sequence, {key, index}).

%% Creates sequence table. Performed once
create_sequence() ->
  create_sequence([node()]).
  
create_sequence(Nodes) ->
  mnesia:create_table(sequence, [{type, set}, {ram_copies, Nodes}, {attributes, record_info(fields, sequence)}]).


%% Inits or resets a sequence to Value
init_sequence(Name, Value) ->
  {atomic, ok} =mnesia:transaction(fun() ->mnesia:write(#sequence{key=Name, index=Value})end),
  ok.
  
%% Returns current value for sequence Name and increments
%% Sequence is created if not exists, and initial value 0 is returned.
sequence(Name) ->
  sequence(Name, 1).

%% increment sequence with Inc
sequence(Name, Inc) ->
     mnesia:dirty_update_counter(sequence, Name, Inc).


     