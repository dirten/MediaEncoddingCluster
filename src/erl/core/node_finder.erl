-module(node_finder).

-behaviour(gen_server).
%-compile(export_all).

-export([start/0, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2, handle_info/2, code_change/3, terminate/2, listen/0, send/0]).


start()->
  spawn(?MODULE,init,[[]]).

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

stop()->
  gen_server:call(?MODULE, stop).

init([])->
  io:format("~w start up~n", [?MODULE]),
  process_flag(trap_exit, true),
%  _Pid=spawn_link(?MODULE,listen,[]),
  _Pid2=spawn_link(?MODULE,send,[]),
  %  link(Pid),
%  register(file_scanner_loop, Pid),
  io:format("~w started~n", [?MODULE]),
  {ok, state}.

send()->
  receive after 2000->
    case inet:ifget("eth0", [broadaddr]) of
      {ok, [{broadaddr, Ip}]} ->
                  {ok, S} =  gen_udp:open(5011, [{broadcast, true}]),
                  gen_udp:send(S, Ip, 6000, "test2"),
                  gen_udp:close(S),
                  io:format("Packet sended");
                _ ->
                  io:format("Bad interface name, or\n"
                    "broadcasting not supported\n")
    end,
    send()
  end.


listen()->
  {ok, S} = gen_udp:open(6000),
    loop(S).


loop(S) ->
    receive
      {udp,Port,Ip,_,_} ->
	    io:format("received:~p ~p~n", [node(Port),Ip]),
	    loop(S)
    end.

handle_call({import_file,Thing},_From,_N)->
  io:format("~w handle_call import~w~n", [?MODULE,Thing]),
  {reply, Thing, state};

handle_call({scan,Directory},_From,_N)->
  io:format("~w handle_call scan~w~n", [?MODULE,Directory]),
  {reply, Directory, state}.

handle_cast(_Msg,N)->
  io:format("~w handle_cast~w~n", [?MODULE,N]),
  {noreply, N}.

handle_info(Info,N)->
  io:format("~w handle_info~w~n", [?MODULE,{Info,N}]),
  case Info of
    {{'EXIT',_,killed},state}->
      exit(killed)
    end,
  {noreply, N}.

terminate(Reason,_N)->
%  file_scanner_loop ! stop,
  io:format("~w shutdown ~w~n", [?MODULE, Reason]),
  ok.

code_change(_OldVsn,N,_Extra)->
  io:format("~w CodeChange ~n", [?MODULE]),
  {ok, N}.
