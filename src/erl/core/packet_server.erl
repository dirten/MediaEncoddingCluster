-module(packet_server).
-behavior(gen_server).
-include("config.hrl").
-include_lib("stdlib/include/qlc.hrl").
-include("./schema.hrl").


-export([init/1,handle_call/3,handle_cast/2, handle_info/2, code_change/3, terminate/2, start_link/0, get_job/0]).

start_link()->
  gen_server:start_link({global,?MODULE},?MODULE,[],[]).

init([])->
  process_flag(trap_exit, true),
  case filelib:ensure_dir(filename:join([config:get(tmp_data_dir,"data"),"test.data.dir" ])) of
    ok->
      ok;
    {error, Reason}->
      io:format("Error: Tmp Data dir not exist ~p",[Reason])
      %      {error,permission_denied_tmp_data}
  end,
  register(?MODULE, self()),
  io:format("~s started with PID=~p~n", [?MODULE, self()]),
%  fprof:trace(start),
  {ok, running}.

get_job()->
  JobFun = fun() ->
               Q = qlc:q([{Jobs#job.id, filename:join(Files#file.path,Files#file.filename), Jobs#job.last_ts, Jobs#job.pass} || Jobs <- mnesia:table(job),Files <- mnesia:table(file),Jobs#job.complete_time == undefined,Jobs#job.infile==Files#file.id ]),
               C = qlc:cursor(Q),
               qlc:next_answers(C, 1)
           end,

  %  {atomic,[Job|_]}=mnesia:transaction(JobFun),
  case mnesia:transaction(JobFun) of
    {atomic,[Job|_]}->
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
      Encoder=[Streams(X)||X<-[SID#jobdetail.outstream||SID<-JobDetails]],
      {element(2,Job),element(1,Job),element(3,Job),element(4,Job),Decoder,Encoder};
    {atomic,[]}->[]

  end.

handle_call(_Call,From,_N)->
  %  io:format("handle_call ~w ~n", [Call]),
  case get_job() of
    []->{reply, {nojob}, state};
    {Filename, JobId, LastTs, Pass, Decoder, Encoder}->

      Data=packet_stack:packetstream(Filename,LastTs, Decoder,Encoder),
      C=length(Data),
      if
        C==0->
          _Result=mnesia:transaction(
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
          %          io:format("BytesPacked ~w~n",[BytesSend]),
          [FirstPacket|_]=Data,
          %          io:format("Data: ~p",[FirstPacket]),
          [D|_]=[X||X<-Decoder,element(4,X)==element(1,FirstPacket)],
          [E|_]=[X||X<-Encoder,element(4,X)==element(1,FirstPacket)],
          %          io:format("DecoderData: ~w",[D]),
          FromTs=#timestamp{num=element(10,D), den=element(11,D)},
          ToTs=#timestamp{num=1, den=1000000},
          FirstTimeStamp=libav:rescale_timestamp(list_to_integer(element(4,FirstPacket)), FromTs, ToTs),
          LastTimeStamp=libav:rescale_timestamp(list_to_integer(element(4,lists:last(Data))), FromTs, ToTs),
          ProcU=#process_unit{
            id=libdb:sequence(process_unit),
            sourcestream=D#stream.id,
            targetstream=E#stream.id,
            sendtime=now(),
            sendnode=From,
            startts=FirstTimeStamp,
            framecount=length(Data),
            sendsize=BytesSend},
          mnesia:transaction(fun()->mnesia:write(ProcU)end),
        
          _Result=mnesia:transaction(
            fun()->
                case mnesia:read({job, JobId}) of
                  []->io:format("Data not found ~n",[]),
                    mnesia:abort(no_job_found);
                  [Job|_]->
                    %          io:format("Data found ~w~n",[Unit]),
                    %            mnesia:write(Unit#process_unit{receivesize=BytesReceived, completetime=now(), data=Data})
                    %                    io:format("In ~p, Out ~p~n",[element(4,FirstPacket), LastTimeStamp]),
                    mnesia:write(Job#job{last_ts=integer_to_list(LastTimeStamp)})
                end
            end),
          %            io:format("Decoder ~p",[D]),
          {reply, {Filename, ProcU#process_unit.id, Pass, D, E, Data}, state}
          %          []->{reply, {nojob}, state}
      end
  end.

handle_cast(Msg,N)->
  %  io:format("handle_cast(Msg,N) ~n", []),
  {_,ProcId,Pass,Data}=Msg,
  % io:format("-~w:~w:~w:~w-",[DS,BinLen,BytesReceived, lists:sum(BinLen)]),
  _Result=mnesia:transaction(
    fun()->
        case mnesia:read({process_unit, ProcId}) of
          []->io:format("Data not found ~n",[]),
            mnesia:abort(no_process_unit_found);
          [Unit|_]->
 %           io:format("Data for Pass ~p ~n",[Pass]),
            if Pass =:= 1->
                mnesia:write(Unit#process_unit{receivesize=0, completetime=now(), log=Data});
              Pass =:= 0 orelse Pass =:= 2 ->
                DS=[element(7,X)||X<-Data],
                BytesReceived=lists:sum(DS),
                mnesia:write(Unit#process_unit{receivesize=BytesReceived, completetime=now()}),
                {ok,Pid}=file:open(filename:join([config:get(tmp_data_dir,"data"), integer_to_list(ProcId)]), write),
                file:write(Pid, term_to_binary(Data)),
                file:close(Pid)
%              io:format("Data written to disk ~n",[])
            end
        end
    end),
  %      Result=mnesia:transaction(
  %      fun()->
  %          case mnesia:read({job, JobId}) of
  %            []->io:format("Data not found ~n",[]),
  %              mnesia:abort(no_job_found);
  %            [Job|_]->
  %          io:format("Data found ~w~n",[Unit]),
  %            mnesia:write(Unit#process_unit{receivesize=BytesReceived, completetime=now(), data=Data})
  %              mnesia:write(Job#job{last_ts=element(3,FirstPacket)})
  %          end
  %      end)
  %  io:format("BytesReceived ~w ~w~n",[BytesReceived, Result]),
  {noreply, N}.

handle_info(_Info,N)->
  io:format("handle_info(Info,N) in Module ~p~n", [?MODULE]),
  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~p stopping~n",[?MODULE]),
%  fprof:trace(stop),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.
