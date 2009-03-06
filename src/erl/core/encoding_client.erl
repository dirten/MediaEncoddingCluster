-include("../include/config.hrl").
-module(encoding_client).
-export([start/0, init/0]).

start()->
  spawn(?MODULE,init,[]),
  ok.

init()->
%  register(encodeclient, self()),
  {ok,SysPortCommand}=application:get_env(sysportexe),
  process_flag(trap_exit, true),
  Port = open_port({spawn, SysPortCommand}, [{packet, 4}, binary]),
  loop(Port).

loop( Port)->
  case catch gen_server:call({global,packet_server}, {packetgroup}) of
    {hivetimeout}->
      io:format("hivetimeout waiting 5 secs~n",[]),
      receive after 5000->loop(Port)end;
    {nomorepackets}->
      io:format("nomorepackets waiting 5 secs~n",[]),
      receive after 5000->loop(Port)end;
    {nodata}->
      io:format("nomorepackets waiting 5 secs~n",[]),
      receive after 5000->loop(Port)end;
    {nojob}->
%      io:format("nojob waiting 5 secs~n",[]),
      receive after 5000->loop(Port)end;
    Any->
%  {Filename, Procid, D, E, Data}->
%      io:format("~w~n",[Any]),
      Port ! {self(), {command, term_to_binary({encode,Any})}},
%      Data=erlang:port_info(Port),
%      io:format("~w",[Data]),
      receive
        {_Fileport, {data, Data}} ->
          D=binary_to_term(Data),
        {_, Procid, _, _, _}=Any,
        gen_server:cast({global,packet_server}, {encoded,Procid,D}),
%        {global,packet_server} ! {encoded,D},
        io:format("Data sended to packet server ~n", []),
%          io:format("~w~n",[D])
          loop(Port);
        {Port, closed} ->
          io:format("Port exited on close ~n", []),
%         global:unregister_name(packet_sender),
          exit(normal);
        {'EXIT', Port, Reason2} ->
 %        global:unregister_name(packet_sender),
          io:format("EncodingClient exited  ~w with data ~w ~n", [Reason2, Any]),
          receive after 5000->init()end
%         exit({normal, Reason2})
          after 10000->
            io:format("No Data~n",[]),
            loop(Port)
          end,
    Port ! {self(), close},
    receive
    {Port, closed} ->
      io:format("Port exited on close ~n", []),
%      global:unregister_name(packet_sender),
      exit(normal);
    {'EXIT', Port, Reason} ->
 %     global:unregister_name(packet_sender),
      io:format("GenCall to ~w Failed  ~w~n", [Port,Reason])
%      exit({normal, Reason})
      end
%    {timeout,{_Reason}}->
%      io:format("DataTimeOut ~n", []),
%      loop( Port)
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

