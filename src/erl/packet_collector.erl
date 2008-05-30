-module(packet_collector).
-export([start/0, loop/0, init/0]).
-include("schema.hrl").


start()->
  Pid=spawn(?MODULE,loop,[]),
  register(?MODULE, Pid),
  Pid.

init()->
  process_flag(trap_exit, true),
  io:format("~p starting\n",[?MODULE]),
  loop().

write({A,B,C})->
%    io:format("received ~w ~w ~w ~n",[A,B,C]),
	mnesia:dirty_write(packet,#packet{id=db:sequence(packet),pts=B, data=C}).

%    Write = fun(Keys) -> [mnesia:write(Keys)], ok end,
%    mnesia:activity(sync_dirty, Write, [#packet{id=db:sequence(packet), data=C}], mnesia_frag).

loop()->
  receive
    {Pid, Result} ->
%      io:format("received ~w ~w~n",[Result, Pid]),
      write(Result);
    X ->
      io:format("receivedX ~w~n",[X]),
      write(X)
  end,
  loop().
