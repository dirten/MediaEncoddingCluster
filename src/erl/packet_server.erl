-module(packet_server).
-behavior(gen_server).
-include("config.hrl").
-include_lib("stdlib/include/qlc.hrl").
-include("schema.hrl").


-export([init/1,handle_call/3,handle_cast/2, code_change/3, terminate/2, start_link/0,loop/1]).

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

init([])->
%  register(fileport,Port),
  process_flag(trap_exit, true),
  global:register_name(?MODULE, self()),
  %  Q=queue:new(),
%  spawn_link(?MODULE,loop,[Q]),
  io:format("~s started~n", [?MODULE]),
  {ok, running}.

get_job()->
    F = fun() ->
          Q = qlc:q([filename:join([E#file.path,E#file.filename]) || E <- mnesia:table(file)]),
          qlc:e(Q)
      end,
  {atomic,_E}=mnesia:transaction(F),
  ok.

loop(Q) ->
  io:format(" Q Loop~n",[]),
  receive
    after 10000->
      io:format(" Reading packets into Q~n",[]),
      N=queue:len(Q),
      if N < 3 ->
        Term=gen_server:call(global:whereis_name(packet_sender), {packetgroup,"",0,0,100}),
        io:format("~w",[Term]),
        lists:foreach(fun(X)->queue:in(X,Q) end,[Term])
        end,
      loop(Q)
    end,
    ok.


handle_call(Call,_From,_N)->
  io:format("handle_call ~w ~n", [Call]),
  {reply, {packet_server,{bla}}, state}.

handle_cast(_Msg,N)->
  io:format("handle_cast(Msg,N)~n", []),
  {noreply, N}.

%handle_info(Info,N)->
%  io:format("handle_info(~w,N)~n", [Info]),
%  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~p stopping~n",[?MODULE]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.
