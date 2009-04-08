-include("../include/config.hrl").
-module(encoding_client).
-compile(export_all).
%-behaviour(gen_server).

start()->
  spawn(?MODULE,init,[temp]),
  ok.

start_link()->
  proc_lib:start_link(?MODULE,init,[self()]).



init(Parent)->
  BinPath=libcode:get_mhivesys_exe(),
  io:format("~s Started with ~p~n",[?MODULE, BinPath]),
  %  loop().
  %  application:set_env(sysportexe,"bin/mhivesys"),
  %  io:format("Client:~p",[application:get_env(sysportexe)]),
  %  {ok,SysPortCommand}=application:get_env(sysportexe),
  process_flag(trap_exit, true),
  Port = open_port({spawn, BinPath}, [{packet, 4}, binary]),
  %  unlink(Port),
  proc_lib:init_ack(Parent, {ok, self()}),
  %  register(encodeclient, Port),
  %  proc_lib:init_ack(Parent, {ok, self()}),
  loop(Port).

loop( Port)->
  case catch gen_server:call({global,packet_server}, {packetgroup},infinity) of
    {hivetimeout}->
      io:format("hivetimeout waiting 5 secs~n",[]),
      receive after 5000->encoding_client:loop(Port)end;
    {nomorepackets}->
      io:format("nomorepackets waiting 5 secs~n",[]),
      receive after 5000->encoding_client:loop(Port)end;
    {nodata}->
      io:format("nomorepackets waiting 5 secs~n",[]),
      receive after 5000->encoding_client:loop(Port)end;
    {nojob}->
      %      io:format("nojob waiting 5 secs~n",[]),
      receive after 5000->encoding_client:loop(Port)end;
    %Any->
    {Filename, Procid, Pass,Dec, Enc, Data}->
%            io:format("~p~n",[{Filename, Procid, Pass,Dec, Enc}]),
      Port ! {self(), {command, term_to_binary({encode,{Filename, Procid, Pass, Dec, Enc, Data}})}},
      %      Data=erlang:port_info(Port),
      %      io:format("~w",[Data]),
      receive
        {_Fileport, {data, InData}} ->
          D=binary_to_term(InData),
          gen_server:cast({global,packet_server}, {encoded,Procid,Pass,D}),
          %        {global,packet_server} ! {encoded,D},
%          io:format("Data sended to packet server ~n", []),
          %          io:format("~w~n",[D])
          encoding_client:loop(Port);
        {Port, closed} ->
          io:format("Port exited on close ~n", []),
          %         global:unregister_name(packet_sender),
          exit(normal);
        {'EXIT', Port, Reason2} ->
          %        global:unregister_name(packet_sender),
          io:format("EncodingClient exited  ~w with data ~p ~n", [Reason2,Procid]),
          %          io:format("EncodingClient exited  ~w ~n", [Reason2]),
          exit({normal, Reason2}),
          receive after 4000->init([])end
          %         exit({normal, Reason2})
      after 100000->
          io:format("No Data from port~n",[]),
          encoding_client:loop(Port)
      end
  end.

system_continue(_Parent, _Deb, _Chs) ->
  io:format("System Continue ~s",[?MODULE]).
%  loop().

system_terminate(Reason, _Parent, _Deb, _Chs) ->
  io:format("Terminate~s,~w",[?MODULE, Reason]),
  exit(Reason).

