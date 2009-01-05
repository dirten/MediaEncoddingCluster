-module(myodbc).
-export([start/0, load/0, init/1, info/0, read/0]).


start()->
    odbc:start(),
    {ok, Ref} = odbc:connect("DSN=local;UID=root;PWD=", []),
    put(con,Ref),
    spawn(?MODULE, init, ["./erltest"]).
    
    
load()->
  call_port({foo,1}).
  
  
info()->
  odbc:describe_table(get(con), "packets").

read()->
  odbc:sql_query(get(con), "SELECT * FROM packets where id< 10").
  
write({_A,_B,_C,_D,_E,_F,_G,_H,_I,_J}) ->
  Ref=get(con),
  odbc:param_query(Ref,"INSERT INTO packets (data) VALUES(?)",[{{sql_varchar,255},["bla"]}]),
% odbc:param_query(Ref,"INSERT INTO packets (data) VALUES(?)",[{sql_longvarbinary,[J]}]),
  ok.


call_port(Msg) ->
    complex ! {call, self(), Msg},
    receive
        {complex, Result} ->
        	write(Result),
            call_port({foo,1})
    	after 10000 ->
    		ok
    end.


init(ExtPrg) ->
%	mnesia:start(),
%	dets:open_file(test_packets, [{type, bag}]),
    register(complex, self()),
    process_flag(trap_exit, true),
    Port = open_port({spawn, ExtPrg}, [{packet, 2}, binary]),
    loop(Port).
%  ok.

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