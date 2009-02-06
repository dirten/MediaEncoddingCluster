-include("config.hrl").
-module(encoding_client).
-export([start/0, init/0]).

start()->
  spawn(?MODULE,init,[]),
  ok.

init()->
  register(encodeclient, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, ?ENCODECLIENTEXE}, [{packet, 4}, binary]),
  loop(global:whereis_name(packet_server),Port).

loop(Server, Port)->
  case gen_server:call(Server, {packetgroup}) of
    Any->io:format("Received  ~w~n", [Any])
  end,
%  Server ! {call, self(),bla},
  receive
    {packet_server, Result} ->
%       io:format("{packet_sender, Result}  ~w~n", []),
      Port ! { self(),{encod,term_to_binary(Result)}},
      receive
        {Port,{data,Data}} ->
%      {encodeddata, Data} ->
        io:format("{Port, {data, Data}}  ~w~n", [binary_to_term(Data)])

          
%          loop(Server, Port)
%      after 5000 ->
 %     unregister(encodeclient),
  %        exit(port_terminated)
      end;
      Any->io:format("Received  ~w~n", [Any]);


{'EXIT', Port, Reason} ->
      unregister(encodeclient),
      io:format("Port exited  ~w~n", [Reason]),
      exit({port_terminated, Reason})
      after 5000 ->
      unregister(encodeclient),
      io:format("Port exited  ~n", []),
          exit(port_terminated)

end.

