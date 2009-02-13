-module(packet_server).
-behavior(gen_server).
-include("config.hrl").
-include_lib("stdlib/include/qlc.hrl").
-include("schema.hrl").


-export([init/1,handle_call/3,handle_call_2/3,handle_cast/2, code_change/3, terminate/2, start_link/0, get_job/0]).

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

handle_call(Call,From,_N)->
  io:format("handle_call ~w ~n", [Call]),
  case get_job() of
    {Filename, JobId, Decoder, Encoder}->
      Data=packet_test:packetstream(Filename),
      C=length(Data),
      if
        C==0->
          Result=mnesia:transaction(
            fun()->
                case mnesia:read({job, JobId}) of
                  []->io:format("Data not found ~n",[]),
                    mnesia:abort(no_process_unit_found);
                  [Job|_]->
                    mnesia:write(Job#job{complete_time=now()})
                end
            end
                                   ),
          {reply, {nodata}, state};
        true->
          DS=[element(7,X)||X<-Data],
          BytesSend=lists:sum(DS),
          io:format("BytesPacked ~w~n",[BytesSend]),
          [FirstPacket|_]=Data,
          %      io:format("Data: ~w",[element(1,FirstPacket)]),
          [D|_]=[X||X<-Decoder,element(4,X)==element(1,FirstPacket)],
          [E|_]=[X||X<-Encoder,element(4,X)==element(1,FirstPacket)],
          %          io:format("DecoderData: ~w",[D]),
          ProcU=#process_unit{
            id=libdb:sequence(process_unit),
            sourcestream=D#stream.id,
            targetstream=E#stream.id,
            sendtime=now(),
            sendnode=From,
            startts=element(3,FirstPacket),
            framecount=length(Data),
            sendsize=BytesSend},
          mnesia:transaction(fun()->mnesia:write(ProcU)end),
          {reply, {Filename, ProcU#process_unit.id, D, E, Data}, state}
          %          []->{reply, {nojob}, state}
      end
  end.

handle_call_2(Call,From,_N)->
  io:format("handle_call ~w ~n", [Call]),
  case get_job() of
    {Filename, JobId, Decoder, Encoder}->
      case gen_server:call({global,packet_sender}, {packetgroup,Filename,0,-1,-1  })of
        hivetimeout->
          {reply, {hivetimeout}, state};
        {nomorepackets}->
          {reply, {nomorepackets}, state};
        Data ->
          %                io:format("Data ~w",[Data]),
          %      io:format("Stream ~w",[element(1,element(1,Data))]),
          [DS|_]=[element(7,X)||X<-Data],
          BytesSend=lists:sum(DS),
          io:format("BytesPacked ~w",[BytesSend]),
          [D|_]=[X||X<-Decoder,element(4,X)==element(1,element(1,Data))],
          [E|_]=[X||X<-Encoder,element(4,X)==element(1,element(1,Data))],
          %      io:format("Data ~w",[Data]),
          ProcU=#process_unit{
            id=libdb:sequence(process_unit),
            sourcestream=D#stream.id,
            targetstream=D#stream.id,
            sendtime=now(),
            sendnode=From,
            sendsize=BytesSend},
          mnesia:transaction(fun()->mnesia:write(ProcU)end),
          {reply, {Filename, JobId, D, E, Data}, state};
        Any->
          io:format("~w Any Data ~w",[?MODULE, Any])
      end
  end.

handle_cast(Msg,N)->
  io:format("handle_cast(Msg,N) ~n", []),
  {_,ProcId,Data}=Msg,
  DS=[element(7,X)||X<-Data],
  BytesReceived=lists:sum(DS),
  Result=mnesia:transaction(
    fun()->
        case mnesia:read({process_unit, ProcId}) of
          []->io:format("Data not found ~n",[]),
            mnesia:abort(no_process_unit_found);
          [Unit|_]->
            %          io:format("Data found ~w~n",[Unit]),
            mnesia:write(Unit#process_unit{receivesize=BytesReceived, completetime=now(), data=Data})
        end
    end
                           ),

  io:format("BytesReceived ~w ~w~n",[BytesReceived, Result]),
  {noreply, N}.

%handle_info(Info,N)->
%  io:format("handle_info(~w,N)~n", [Info]),
%  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~p stopping~n",[?MODULE]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.
