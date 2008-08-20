-module(av).
-export([start/0, stop/0, init/1]).
-export([foo/1, bar/1, read/0]).
-export([print/1]).
-import(db).
-include("schema.hrl").

start() ->
    spawn(?MODULE, init, ["./erltest"]).

stop() ->
    complex ! stop.

foo(X) ->
    call_port({foo, X}).
bar(Y) ->
    call_port({bar, Y}).

print(A) ->
	io:format("~w~n",[A]).

read()->
  db:sequence(test).

%-record(packet, {id,stream_id,pts,dts, stream_index, key_frame, frame_group,flags, duration, pos, data}).

write({A,B,C,D,E,F,G,H,I,J}) ->
%    dets:insert(test_packets, {db:sequence(packet),J}).
    Write = fun(Keys) -> [mnesia:write(Keys)], ok end,
    mnesia:activity(sync_dirty, Write, [#packet{id=db:sequence(packet), stream_id=A, pts=B, dts=C, stream_index=D, key_frame=E, frame_group=F,flags=G, duration=H, pos=I}], mnesia_frag).
%    mnesia:activity(sync_dirty, Write, [#packet{id=db:sequence(packet), stream_id=A, pts=B, dts=C, stream_index=D, key_frame=E, frame_group=F,flags=G, duration=H, pos=I, data=J}], mnesia_frag).
%%    I=read(),
%%    I=db:sequence(test),
%	mnesia:dirty_write(packet,#packet{pts=A, data=C}).
%	Fun=fun()->
%		mnesia:write(#packet{id=db:sequence(packet), stream_id=0, pts=A, dts=B, stream_index=0, key_frame=0, frame_group=0,flags=0, duration=0, pos=0})
%		end,
%%	mnesia:async_dirty(Fun).
%	mnesia:transaction(Fun).
%%	mnesia:ets(Fun).
	
call_port(Msg) ->
    complex ! {call, self(), Msg},
    receive
        {complex, Result} ->
%%        	print(Result),
        	write(Result),
            call_port({foo,1})
    	after 10000 ->
    		ok
    end.

init(ExtPrg) ->
	mnesia:start(),
%	dets:open_file(test_packets, [{type, bag}]),
    register(complex, self()),
    process_flag(trap_exit, true),
    Port = open_port({spawn, ExtPrg}, [{packet, 2}, binary]),
    loop(Port).

loop(Port) ->
%    erlang:display("waiting for data~n"),
    receive
        {call, Caller, Msg} ->
            Port ! {self(), {command, term_to_binary(Msg)}},
%            erlang:display("data here~n"),
            receive
                {Port, {data, Data}} ->
                    Caller ! {complex, binary_to_term(Data)}
                after 5000 -> ok
%    		      erlang:display("data not arrived")
            end,
            loop(Port);
        stop ->
            erlang:display("Stopping now~n"),
            Port ! {self(), close},
            receive
                {Port, closed} ->
                    exit(normal)
            end;
        {'EXIT', Port, Reason} ->
            erlang:display("Stopping terminated now"),
            erlang:display(Reason),
            dets:close(test_packets),
            exit(port_terminated);
        _ ->
          erlang:display("unknown")
          
    end.
%      io:format("Stopping now~n")


