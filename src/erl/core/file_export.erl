-module(file_export).

-include("config.hrl").

-export([init/1,start_link/0, loop/0]).
-export([system_continue/3, system_terminate/4]).

	-include("schema.hrl").
-include_lib("stdlib/include/qlc.hrl").

start_link()->
  proc_lib:start_link(?MODULE,init,[self()]).

init(Parent)->
  proc_lib:init_ack(Parent, {ok, self()}),
  io:format("FileExport Started~n",[]),
  loop().

%get_completed_jobs()->
%  F = fun() ->
%          Q = qlc:q([{PU#job.id,filename:join(F#file.path, F#file.filename)} || PU <- qlc:keysort(2,mnesia:table(job)),F<-mnesia:table(file), PU#job.complete_time > undefined, F#file.id==PU#job.outfile]),
%          C = qlc:e(Q),
%          C
%      end,
%  {atomic,E}=mnesia:transaction(F),
%  E.

loop()->
  receive
  after 15000->
      export(select),
      loop()
  end.


system_continue(_Parent, _Deb, _Chs) ->
  io:format("~s",[?MODULE]),
  loop().

system_terminate(Reason, _Parent, _Deb, _Chs) ->
  io:format("~s,~w",[?MODULE, Reason]),
  exit(Reason).


export(FileNo) when is_integer(FileNo)->
  {atomic,E}=mnesia:transaction(fun()->qlc:e(qlc:q([{PU#job.id,filename:join(F#file.path, F#file.filename)} || PU <- qlc:keysort(2,mnesia:table(job)),F<-mnesia:table(file), PU#job.complete_time > undefined, F#file.id==PU#job.outfile,F#file.id==FileNo]))end),
  [FileName|_]=E,
  %% check if File exist
  case filelib:is_regular(element(2,FileName)) of
    false->
      %%check if directory is writable
      case filelib:ensure_dir(element(2,FileName)) of
        ok->
          F2=[X||X<-E,filelib:ensure_dir(element(2,FileName))=:=ok],
          Fun2=fun(JobId)->
                   fun()->
                               qlc:e(
                                 qlc:q(
                                 [Detail#jobdetail.outstream || Detail <- mnesia:table(jobdetail), Detail#jobdetail.jobid==JobId]
                                      )
                                    )
                           end
               end,
          Bla=[element(2,mnesia:transaction(Fun2(element(1,D))))|| D<-F2],
          %  {atomic,E2}=mnesia:transaction(Fun2(2)),
          Fun=fun(StreamId)->
                  fun()->
                              qlc:e(
                                qlc:q(
                                [P || P <- qlc:keysort(5,mnesia:table(process_unit)), P#process_unit.targetstream==StreamId,P#process_unit.receivesize>0]
                                     )
                                   )
                          end
              end,
          Result=lists:flatten([[element(2,mnesia:transaction(Fun(T1)))||T1<-D] ||D<-Bla]),

          FileWriter = fun(FileId, FilePort)->
                           case file:read_file(filename:join(["data", integer_to_list(FileId)])) of
                             {ok,Data}->
                               %        io:format("Write Data"),
                               FilePort ! {self(), {command, term_to_binary({writepacket,binary_to_term(Data)})}},
                               ok;
                             %        file:write(MergePid, Data);
                             %        binary_to_term(Data);
                             {error,enoent}->
                               nodata
                           end
                       end,
          %    B=[element(2,X)||X<-Result],
          %  [File|_]=T,
          %  io:format("open File ~s",[element(2,File)]),
          Port = open_port({spawn, ?SYSPORTEXE}, [{packet, 4}, binary]),
          %  Port=[],
          Port ! {self(), {command, term_to_binary({createfile,list_to_atom(element(2,FileName))})}},
          {atomic,St}=mnesia:transaction(fun()->qlc:e(qlc:q([S || S <- qlc:keysort(2,mnesia:table(stream)), S#stream.fileid==FileNo]))end),
          [Port!{self(), {command, term_to_binary({addstream,S, element(4,S)})}}||S<-St],
          Port ! {self(), {command, term_to_binary({initfile})}},

          %    {ok,Pid}=file:open("Merge.data", write),%dets:open_file(filename:join(["tmp", integer_to_list(ProcId)]),[]),
          %   io:write(Pid, Data),

          [FileWriter(element(2,X), Port)||X<-Result],
          Port ! {self(), {command, term_to_binary({closefile})}},
          Port ! {self(), close},

          %    file:close(Pid),
          % {atomic,E3}=mnesia:transaction(Fun(13)),
          file_created;
        _->directory_not_writable
      end;
    true->
      fileexist
  end;


export(select)->
  {atomic,E}=mnesia:transaction(fun()->qlc:e(qlc:q([PU#job.outfile|| PU <- qlc:keysort(2,mnesia:table(job)), PU#job.complete_time > undefined]))end),
  [export(F)||F<-E].

