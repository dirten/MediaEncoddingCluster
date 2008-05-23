-module(complex2).
-export([start/0, stop/0, init/1]).
-export([foo/1, bar/1]).
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

write({A,B,C}) ->
	Fun=fun()->
		Packet = #packet{
			pts=A,
			data_size=B,
			data=C
		},
%%		print({A,B,C}),
		mnesia:write(Packet)
		end,
%%	mnesia:async_dirty(Fun).
	mnesia:transaction(Fun).
	
call_port(Msg) ->
    complex ! {call, self(), Msg},
    receive
        {complex, Result} ->
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


