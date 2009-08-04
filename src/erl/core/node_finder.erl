-module(node_finder).

-behaviour(gen_server).
%-compile(export_all).

-export([start/0, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2, handle_info/2, code_change/3, terminate/2, listen/0, listen/1, send/0, get_bcast_addrs/0]).


start()->
    spawn(?MODULE,init,[[]]).

start_link()->
    gen_server:start_link({local,?MODULE},?MODULE,[],[]).

stop()->
    gen_server:call(?MODULE, stop).

init([])->
    io:format("~w start up~n", [?MODULE]),
    process_flag(trap_exit, true),
    _Pid=spawn_link(?MODULE,listen,[]),
    _Pid2=spawn_link(?MODULE,send,[]),
    io:format("~w started~n", [?MODULE]),
    {ok, state}.

get_bcast_addrs()->
    {ok,List}=inet:getif(),
    lists:foldl(fun(El, Acc)->[{element(1,element(1, El)),element(2,element(1, El)),element(3,element(1, El)),255}]++Acc end,[], List).

send()->
    receive after 2000->
            BCast=fun(Ip)->
                    receive after 500->
                            {ok, S} =  gen_udp:open(5011, [{broadcast, true}]),
                            gen_udp:send(S, Ip, 6000, libutil:toString(node())),
                            gen_udp:close(S)
                            end
                  end,
            %sending to each ip address an udp broadcast message to discover
            lists:foreach(BCast,get_bcast_addrs()),
            node_finder:send()
            end.
send3()->
    receive after 2000->
            case inet:ifget("vmnet8", [broadaddr]) of
                {ok, [{broadaddr, Ip}]} ->
                    {ok, S} =  gen_udp:open(5011, [{broadcast, true}]),
                    gen_udp:send(S, Ip, 6000, "test2"),
                    gen_udp:close(S),
                    io:format("Packet sended to ip ~p",[Ip]);
                _ ->
                    io:format("Bad interface name, or\n"
                        "broadcasting not supported\n")
            end,
            node_finder:send()
            end.
send2()->
    receive after 2000->
            SendOpts = [ { ip, {0,0,0,0 } },
                        { multicast_ttl, 3000 },
                        { multicast_loop, true },
                        {broadcast, true}],

            {ok, S} =  gen_udp:open(5011, SendOpts),
            io:format("SocketOpts:~p",[inet:getopts(S,[ip])]),
            gen_udp:send(S, {224,0,0,1}, 6000, "testdata"),
            gen_udp:close(S),
            io:format("Packet sended ~n"),
            node_finder:send()
            end.


listen()->
    {ok, S} = gen_udp:open(6000),
    node_finder:listen(S).


listen(S) ->
    receive
        {udp,Port,Ip,_PortNumber,NodeName} ->

            Host=libnet:get_host_name(NodeName),
            inet_db:add_host(Ip,[Host]),
            case lists:member(list_to_atom(NodeName),nodes()++[node()]) of
                false->
                    net_adm:ping(list_to_atom(NodeName));
                true->allready_connected
            end,
            node_finder:listen(S);
        Any->
            io:format("~p",[Any])
    end.

handle_call({import_file,Thing},_From,_N)->
    io:format("~w handle_call import~w~n", [?MODULE,Thing]),
    {reply, Thing, state};

handle_call({scan,Directory},_From,_N)->
    io:format("~w handle_call scan~w~n", [?MODULE,Directory]),
    {reply, Directory, state};
handle_call(stop,_From,_N)->
    io:format("~w handle_call stop~n", [?MODULE]),
    {reply, stop, state};
handle_call(Any,_From,_N)->
    io:format("~w handle_call ~w ~n", [?MODULE, Any]),
    {reply, stop, state}.

handle_cast(_Msg,N)->
    io:format("~w handle_cast~w~n", [?MODULE,N]),
    {noreply, N}.

handle_info(Info,N)->
    io:format("~w handle_info~w~n", [?MODULE,{Info,N}]),
    {noreply, N}.

terminate(shutdown,_State)->
  %  file_scanner_loop ! stop,
%    io:format("~w shutdown ~n", [?MODULE]),
    ok.

code_change(_OldVsn,N,_Extra)->
    io:format("~w CodeChange ~n", [?MODULE]),
    {ok, N}.
