-include("config.hrl").
-module(encoding_client).
-export([start/0, init/0]).

start()->
  spawn(?MODULE,init,[]),
  ok.

init()->
%  register(encodeclient, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, ?ENCODECLIENTEXE}, [{packet, 4}, binary]),
  loop(global:whereis_name(packet_server),Port).

loop(Server, Port)->
  case gen_server:call(Server, {packetgroup}) of
    Any->
%      io:format("~w~n",[element(4,element(5,Any))]),
      Port ! {self(), {command, term_to_binary({encode,Any})}},
      receive
        {Fileport, {data, Data}} ->
          D=binary_to_term(Data),
          io:format("~w~n",[D])
          after 20000->
            io:format("No Data~n",[])
          end,
    Port ! {self(), close},
    receive
    {Port, closed} ->
      io:format("Port exited on close ~n", []),
%      global:unregister_name(packet_sender),
      exit(normal);
    {'EXIT', Port, Reason} ->
 %     global:unregister_name(packet_sender),
      io:format("Port exited  ~w~n", [Reason])
%      exit({normal, Reason})
      end
%      Size=element(5,element(1,element(5,Any))),
%      io:format("~w,~w,~w,~w,~w~n",[element(1,element(1,element(5,Any))),element(2,element(1,element(5,Any))),element(3,element(1,element(5,Any))),element(4,element(1,element(5,Any))),element(5,element(1,element(5,Any)))])
%if Size > 0 ->
%      loop(Server, Port);
%      ok;
%      true->
%        io:format("Nothing to do")
%        receive
 %         after 1000->
  %          loop(Server,Port)
   %     end
%    end
  end.

