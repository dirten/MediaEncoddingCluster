-module(test).

-export([insert/0, read/0, read2/0, create/0, drop/0,sequence/1]).
-include("schema.hrl").
-include("C:\\Programme\\erl5.6.5\\lib\\stdlib-1.15.5\\include/qlc.hrl").
%-include("/usr/lib/erlang/lib/stdlib-1.15.1/include/qlc.hrl").
%-include("stdlib/include/qlc.hrl").
-record(sequence, {key, index}).

create()->
  mnesia:create_table(file,[{disc_copies, [node()]},{attributes, record_info(fields, file)}]),
  mnesia:create_table(watchfolder,[{disc_copies, [node()]},{attributes, record_info(fields, watchfolder)}]),
  mnesia:create_table(stream,[{disc_copies, [node()]},{attributes, record_info(fields, stream)}]),
  mnesia:create_table(profile,[{disc_copies, [node()]},{attributes, record_info(fields, profile)}]),
  mnesia:create_table(sequence, [{type, set}, {disc_copies,[node()]}, {attributes, record_info(fields, sequence)}]).

drop()->
  mnesia:delete_table(file),
  mnesia:delete_table(watchfolder),
  mnesia:delete_table(stream),
  mnesia:delete_table(sequence).

init_sequence(Name, Value) ->
 	  {atomic, ok} =mnesia:transaction(fun() ->mnesia:write(#sequence{key=Name, index=Value})end),
 	  ok.
      
sequence(Name, Inc) ->
 	     mnesia:dirty_update_counter(sequence, Name, Inc).

sequence(Name) ->
  sequence(Name, 1).


insert()->
  R=#file{id=2, filename=test.avi, path='/tmp', size=100, containertype=avi,streamcount=2,duration=12000,bitrate=512000,type=0,parent=0},
  mnesia:dirty_write(R).

read()->
  File = #file{filename = '$1', path='$2',_='_'},
  E=mnesia:dirty_select(file,[{File,[],['$2','$1']}]),
  E.

read2()->
  F = fun() ->
    Q = qlc:q([filename:join([E#file.path,E#file.filename]) || E <- mnesia:table(file)]),
    qlc:e(Q)
  end,
  {atomic,E}=mnesia:transaction(F),
  E.
