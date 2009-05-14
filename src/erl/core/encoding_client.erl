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
  process_flag(trap_exit, true),
  Port = open_port({spawn, BinPath}, [{packet, 4}, binary]),
  proc_lib:init_ack(Parent, {ok, self()}),
  loop(Port).

loop( Port)->
  try gen_server:call({global,packet_server}, {packetgroup},infinity) of
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
      %io:format("nojob waiting 5 secs~n",[]),
      receive after 5000->encoding_client:loop(Port)end;
    {Filename, Procid, Pass,Dec, Enc, Data}->
      Port ! {self(), {command, term_to_binary({encode,{Filename, Procid, Pass, Dec, Enc, Data}})}},
      receive
        {_Fileport, {data, InData}} ->
          D=binary_to_term(InData),
          gen_server:cast({global,packet_server}, {encoded,Procid,Pass,D}),
          encoding_client:loop(Port);
        {Port, closed} ->
          io:format("Port exited on close ~n", []),
          exit(normal);
        {'EXIT', Port, Reason2} ->
          io:format("EncodingClient exited  ~w with data ~p ~n", [Reason2,Procid]),
          exit({normal, Reason2}),
          receive after 4000->init([])end
      after 100000->
          io:format("No Data from port~n",[]),
          encoding_client:loop(Port)
      end
  catch
    M:{noproc,
       {gen_server,call,
        [{global,packet_server},
         {packetgroup},
         infinity]}}->
      io:format("No connection to Packet Server ~p~n",[M]),
      receive after 10000->
                  encoding_client:loop(Port)
                  
              end;
    M:F->io:format("Exception aufgetreten ~p:~p~n",[M,F])

  end.

system_continue(_Parent, _Deb, _Chs) ->
  io:format("System Continue ~s",[?MODULE]).

system_terminate(Reason, _Parent, _Deb, _Chs) ->
  io:format("Terminate~s,~w",[?MODULE, Reason]),
  exit(Reason).

