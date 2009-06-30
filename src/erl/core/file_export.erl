-module(file_export).

-include("config.hrl").

-export([run/0, export/1]).
-include("schema.hrl").
-include_lib("stdlib/include/qlc.hrl").


run()->
    {atomic,E}=mnesia:transaction(fun()->qlc:e(qlc:q([PU#job.outfile|| PU <- qlc:keysort(2,mnesia:table(job)), PU#job.complete_time > undefined,PU#job.pass=:=0 orelse PU#job.pass=:=2]))end),
    [export(F)||F<-E].

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
                    Fun2=
                        fun(JobId)->
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
                    Fun=
                        fun(StreamId)->
                            fun()->
                                    qlc:e(
                                        qlc:q(
                                        [P || P <- qlc:keysort(#process_unit.startts,mnesia:table(process_unit)), P#process_unit.targetstream==StreamId,P#process_unit.receivesize>0]
                                             )
                                         )
                            end
                        end,
                    Result=lists:keysort(5,lists:flatten([[element(2,mnesia:transaction(Fun(T1)))||T1<-D] ||D<-Bla])),

                    FileWriter =
                        fun(FileId, FilePort)->
                            case file:read_file(filename:join(["data", integer_to_list(FileId)])) of
                                {ok,Data}->
                                    FilePort ! {self(), {command, term_to_binary({writepacketlist,binary_to_term(Data)})}},
                                    ok;
                                {error,enoent}->
                                    io:format("File ~p Not Found",[FileId]),
                                    nodata
                            end
                        end,
                    PacketWriter =
                        fun(Packet, FilePort)->
                            FilePort ! {self(), {command, term_to_binary({writepacket,Packet})}},
                            ok
                        end,
                    Mapper=fun(A)->
                            {element(1,A),element(2,A),libutil:toString(element(3,A)),element(4,A),element(5,A),element(6,A),element(7,A),element(8,A),element(9,A),element(10,A)}
                           end,
                    PacketListWriter =
                        fun(PacketList, FilePort)->
%                  io:format("write PacketList:~p",[PacketList]),
                            NewPacketList=lists:map(Mapper,PacketList),
%                  io:format("write PacketList:~p",[NewPacketList]),
                            FilePort ! {self(), {command, term_to_binary({writepacketlist,NewPacketList})}},
                            FilePort
                        end,
                    BinPath=libcode:get_mhivesys_exe(),
%          fprof:trace(start, "my_fprof.trace"),
                    Port = open_port({spawn, BinPath}, [{packet, 4}, binary]),
                    link(Port),
                    Port ! {self(), {command, term_to_binary({createfile,list_to_atom(element(2,FileName))})}},
                    {atomic,St}=mnesia:transaction(fun()->qlc:e(qlc:q([S || S <- qlc:keysort(2,mnesia:table(stream)), S#stream.fileid==FileNo]))end),
                    [Port!{self(), {command, term_to_binary({addstream,S, element(4,S)})}}||S<-St],
                    Port ! {self(), {command, term_to_binary({initfile})}},
          %          ResultSorted=file_export_stack:prepare(Result),
          %          qlc:e(qlc:q([PacketWriter(D, Port)||D<-file_export_stack:table(Result)])),

          %          qlc:fold(PacketListWriter, Port,qlc:q([D||D<-file_export_stack:table(Result)])),


                    %qlc:e(qlc:q([PacketListWriter(D, Port)||D<-file_export_stack:table(Result)])),
                    Cur=qlc:cursor(qlc:q([D||D<-file_export_stack:table(Result)])),
                    iterate(Cur,PacketListWriter, Port),


          %          [PacketWriter(X, Port)||X<-file_export_stack:table(Result)],
                    Port ! {self(), {command, term_to_binary({closefile})}},
                    Port ! {self(), close},
 %         fprof:trace(stop),
                    file_created;
                _->directory_not_writable
            end;
        true->
            fileexist;
        _->error
    end.


iterate(Cursor, Writer, Port)->
    case qlc:next_answers(Cursor, 1) of
        []->io:format("AllDataWrite"),all_data_writen;
        [Data]->
%            io:format("Write data"),
            Writer(Data, Port),
            iterate(Cursor, Writer, Port)
    end.


