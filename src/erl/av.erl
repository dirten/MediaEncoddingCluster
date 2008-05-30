-module(av).
-export([start/0, stop/0, init/1]).
-export([foo/1, bar/1, read/0]).
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

print({A,B,C}) ->
	io:format("~w~n",[A]).

read()->
  db:sequence(test).
  
write({A,B,C}) ->
    Write = fun(Keys) -> [mnesia:write(Keys)], ok end,
    mnesia:activity(sync_dirty, Write, [#packet{id=db:sequence(packet), data=C}], mnesia_frag).
%%    I=read(),
%%    I=db:sequence(test),
%	mnesia:dirty_write(packet,#packet{pts=A, data=C}).
%%	Fun=fun()->
%%		print({A,B,C}),
%%		mnesia:write(#packet{id=I,pts=A,data_size=B,data=term_to_binary(C)})
%%		end,
%%	mnesia:async_dirty(Fun).
%%	mnesia:transaction(Fun).
%%	mnesia:ets(Fun).
	
call_port(Msg) ->
    complex ! {call, self(), Msg},
    receive
        {complex, Result} ->
%%        	print(Result),
        	write(Result),
            call_port({foo,1});
        {'EXIT', Port, Reason} ->
            exit(port_terminated)
    	after 50000 ->
    		exit(port_terminated)
    end.

init(ExtPrg) ->
	mnesia:start(),
    register(complex, self()),
    process_flag(trap_exit, true),
    Port = open_port({spawn, ExtPrg}, [{packet, 2}, binary]),
    loop(Port).

loop(Port) ->
    receive
        {call, Caller, Msg} ->
            Port ! {self(), {command, term_to_binary(Msg)}},
            receive
                {Port, {data, Data}} ->
                    Caller ! {complex, binary_to_term(Data)}
            end,
            loop(Port);
        stop ->
            Port ! {self(), close},
            receive
                {Port, closed} ->
                    exit(normal)
            end;
        {'EXIT', Port, Reason} ->
            exit(port_terminated)
    end.


