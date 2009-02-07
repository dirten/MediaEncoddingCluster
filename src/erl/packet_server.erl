-module(packet_server).
-behavior(gen_server).
-include("config.hrl").
-include_lib("stdlib/include/qlc.hrl").
-include("schema.hrl").


-export([init/1,handle_call/3,handle_cast/2, code_change/3, terminate/2, start_link/0,loop/1, get_job/0]).

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

init([])->
  %  register(fileport,Port),
  process_flag(trap_exit, true),
  global:register_name(?MODULE, self()),
  %  Q=queue:new(),
  %  spawn_link(?MODULE,loop,[Q]),
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


loop(Q) ->
  io:format(" Q Loop~n",[]),
  receive
  after 10000->
      io:format(" Reading packets into Q~n",[]),
      N=queue:len(Q),
      if N < 3 ->
          Term=gen_server:call(global:whereis_name(packet_sender), {packetgroup,"",0,0,100}),
          io:format("~w",[Term]),
          lists:foreach(fun(X)->queue:in(X,Q) end,[Term])
      end,
      loop(Q)
  end,
  ok.


handle_call(Call,_From,_N)->
  io:format("handle_call ~w ~n", [Call]),
  {Filename, JobId, Decoder, Encoder}=get_job(),
  case gen_server:call(global:whereis_name(packet_sender), {packetgroup,Filename,0,-1,120})of
    Data ->
%      io:format("~w~n",[Data]),
      {reply, {Filename, JobId, Decoder, Encoder, Data}, state}
  end.
%{reply, {Filename, JobId, Decoder, Encoder}, state}.

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
