-module(node_watcher).

-export([start_link/0, init/1, handle_call/3, handle_cast/2, handle_info/2, terminate/2, code_change/3, listen/0]).
-behaviour(gen_server).


start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

init([])->
  io:format("~s Started~n",[?MODULE]),
  %  register(?MODULE, self()),
  process_flag(trap_exit, true),
  net_kernel:monitor_nodes(true),
  _Pid=spawn_link(?MODULE,listen,[]),
  {ok, state}.

listen()->
  {ok, S} = gen_udp:open(6000),
  loop(S).


loop(S) ->
  receive
    {udp,Port,Ip,_,_} ->
	    io:format("received:~p ~p~n", [node(Port),Ip]),
	    loop(S)
  end.


handle_call(All,_From,_N)->
  io:format("~w handle_call scan~w~n", [?MODULE,All]),
  {reply, All, state}.

handle_cast(Info,N)->
  io:format("~w handle_cast~w~n", [?MODULE,{Info,N}]),
  {noreply, N}.

handle_info({nodeup, Node},N)->
  error_logger:info_msg("NodeUp ~p",[Node]),
  {ok, Bin}=file:read_file(".hosts.erlang"),
  Data=binary_to_list(Bin),
  List=string:tokens(Data,"\n"),
  Host="'"++string:substr(atom_to_list(Node),string:rchr(atom_to_list(Node), $@)+1)++"'.",
  %  io:format("Host:~p, List:~p",[Host, List]),
  case lists:member(Host,List) of
    false->
      %      io:format("Adding new Node:~p",[Node]),
      Out=lists:append(List,[Host]),
      %      io:format("new Host List:~p",[string:join(Out,"\n")++"\n"]),
      file:write_file(".hosts.erlang",string:join(Out,"\n")++"\n"),
      append;
    true->
      %      io:format("Not new Node:~p",[Node]),
      do_nothing
  end,
  {noreply, N};
handle_info({nodedown, Node},N)->
  error_logger:info_msg("NodeDown ~p",[Node]),
  {noreply, N}.

terminate(Reason,_N)->
  io:format("~w shutdown ~w~n", [?MODULE, Reason]),
  ok.

code_change(_OldVsn,N,_Extra)->
  io:format("~w CodeChange ~n", [?MODULE]),
  {ok, N}.
