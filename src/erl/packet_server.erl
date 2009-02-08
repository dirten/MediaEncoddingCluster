-module(packet_server).
-behavior(gen_server).
-include("config.hrl").
-include_lib("stdlib/include/qlc.hrl").
-include("schema.hrl").


-export([init/1,handle_call/3,handle_cast/2, code_change/3, terminate/2, start_link/0, get_job/0]).

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

init([])->
  process_flag(trap_exit, true),
  global:register_name(?MODULE, self()),
  io:format("~s started~n", [?MODULE]),
  {ok, running}.

get_job()->
  JobFun = fun() ->
               Q = qlc:q([{Jobs#job.id, filename:join(Files#file.path,Files#file.filename)} || Jobs <- mnesia:table(job),Files <- mnesia:table(file),Jobs#job.complete_time == undefined,Jobs#job.infile==Files#file.id ]),
               C = qlc:cursor(Q),
               qlc:next_answers(C, 1)
           end,
  {atomic,[Job|_]}=mnesia:transaction(JobFun),
  %  io:format("~w~n",[Job]),
  JobDetailFun = fun() ->
                     Q = qlc:q([Detail || Detail <- mnesia:table(jobdetail), Detail#jobdetail.jobid == element(1,Job) ]),
                     qlc:e(Q)
                 end,
  {atomic,JobDetails}=mnesia:transaction(JobDetailFun),

  Streams = fun(Id)->
                DataFun = fun()->
                              qlc:e(qlc:q([Stream || Stream <- mnesia:table(stream), Stream#stream.id == Id ]))
                          end,
                {atomic,[Data|_]}=mnesia:transaction(DataFun),
                %               io:format("~w~n",[Data]),
                Data
            end,
  
      Decoder=[Streams(X)||X<-[SID#jobdetail.instream||SID<-JobDetails]],
       Encoder=[Streams(X)||X<-[SID#jobdetail.outstream||SID<-JobDetails]]
  ,
  {element(2,Job),element(1,Job),Decoder,Encoder}.


handle_call(Call,_From,_N)->
  io:format("handle_call ~w ~n", [Call]),
  {Filename, JobId, Decoder, Encoder}=get_job(),
  case gen_server:call(global:whereis_name(packet_sender), {packetgroup,Filename,1,0,2})of
    Data ->
      io:format("Stream ~w",[element(1,element(1,Data))]),
      [D|_]=[X||X<-Decoder,element(4,X)==element(1,element(1,Data))],
      [E|_]=[X||X<-Encoder,element(4,X)==element(1,element(1,Data))],
      io:format("Decoder ~w",[E]),
      {reply, {Filename, JobId, D, E, Data}, state}
  end.

handle_cast(_Msg,N)->
  io:format("handle_cast(Msg,N)~n", []),
  {noreply, N}.

%handle_info(Info,N)->
%  io:format("handle_info(~w,N)~n", [Info]),
%  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~p stopping~n",[?MODULE]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.
