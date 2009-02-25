-module(file_export).

-export([init/1,start_link/0, loop/0]).

-export([system_continue/3, system_terminate/4]).
-include("schema.hrl").
-include_lib("stdlib/include/qlc.hrl").

start_link()->
  proc_lib:start_link(?MODULE,init,[self()]).

init(Parent)->
  proc_lib:init_ack(Parent, {ok, self()}),
  loop().

get_completed_jobs()->
  F = fun() ->
          Q = qlc:q([{PU#job.id,filename:join(F#file.path, F#file.filename)} || PU <- qlc:keysort(2,mnesia:table(job)),F<-mnesia:table(file), PU#job.complete_time > undefined, F#file.id==PU#job.outfile]),
          C = qlc:e(Q),
          C
      end,
  {atomic,E}=mnesia:transaction(F),
  E.

loop()->
  receive
  after 5000->loop()
  end.
  

system_continue(_Parent, _Deb, _Chs) ->
  loop().

system_terminate(Reason, _Parent, _Deb, _Chs) ->
  exit(Reason).


