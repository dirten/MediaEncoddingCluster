-module(test).

-export([grid/0, gridimport/0,export/1,dirtyimport/0,import/1,insert/0, read/0, read2/0, read3/0, read4/0, read5/1, create/0, drop/0,sequence/1, load/0, clear/0]).
-include("schema.hrl").
%-include("C:\\Programme\\erl5.6.5\\lib\\stdlib-1.15.5\\include/qlc.hrl").
%-include("/usr/lib/erlang/lib/stdlib-1.15.1/include/qlc.hrl").
-include_lib("stdlib/include/qlc.hrl").
-record(sequence, {key, index}).

create()->
  mnesia:create_table(file,[{disc_copies, [node()]},{attributes, record_info(fields, file)}]),
  mnesia:create_table(watchfolder,[{disc_copies, [node()]},{attributes, record_info(fields, watchfolder)}]),
  mnesia:create_table(stream,[{disc_copies, [node()]},{attributes, record_info(fields, stream)}]),
  mnesia:create_table(profile,[{disc_copies, [node()]},{attributes, record_info(fields, profile)}]),
  mnesia:create_table(job,[{disc_copies, [node()]},{attributes, record_info(fields, job)}]),
  mnesia:create_table(jobdetail,[{disc_copies, [node()]},{attributes, record_info(fields, jobdetail)}]),
  mnesia:create_table(sequence, [{type, set}, {disc_copies,[node()]}, {attributes, record_info(fields, sequence)}]).


drop()->
  mnesia:delete_table(file),
  mnesia:delete_table(watchfolder),
  mnesia:delete_table(stream),
  mnesia:delete_table(profile),
  mnesia:delete_table(job),
  mnesia:delete_table(jobdetail),
  mnesia:delete_table(sequence).

load()->
  mnesia:load_textfile("test.data").

clear()->
  drop(),
  create(),
  load().
init_sequence(Name, Value) ->
  {atomic, ok} =mnesia:transaction(fun() ->mnesia:write(#sequence{key=Name, index=Value})end),
  ok.
      
sequence(Name, Inc) ->
  mnesia:dirty_update_counter(sequence, Name, Inc).

sequence(Name) ->
  sequence(Name, 1).


insert()->
  R=#file{id=2, filename=test.avi, path='/tmp', size=100, containertype=avi,streamcount=2,duration=12000,bitrate=512000,type=0,parent=0},
  mnesia:dirty_write(R).

read()->
  File = #file{filename = '$1', path='$2',_='_'},
  E=mnesia:dirty_select(file,[{File,[],['$2','$1']}]),
  E.

read2()->
  F = fun() ->
          Q = qlc:q([[E#file.path,E#file.filename] || E <- mnesia:table(file)]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
  E.
read3()->
  F = fun() ->
          Q = qlc:q([[Files,Jobs,Details] || Jobs <- mnesia:table(job),Files <- mnesia:table(file),Details <- mnesia:table(jobdetail),Jobs#job.id==Details#jobdetail.jobid,Jobs#job.infile==Files#file.id , Jobs#job.complete_time == undefined ],{join, nested_loop}),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
  E.
read4()->
  F = fun() ->
          Q = qlc:q([[Files,Jobs] || Jobs <- mnesia:table(job),Files <- mnesia:table(file),Jobs#job.infile==Files#file.id , Jobs#job.complete_time == undefined ]),
          C = qlc:cursor(Q),
          qlc:next_answers(C, 1)
      end,
  {atomic,[E|_]}=mnesia:transaction(F),
  E.
read5(JobId)->
  F = fun() ->
          Q = qlc:q([[Detail,Stream] || Detail <- mnesia:table(jobdetail),Stream <- mnesia:table(stream),Detail#jobdetail.instream==Stream#stream.id, Detail#jobdetail.jobid == JobId ]),
          C = qlc:cursor(Q),
          qlc:next_answers(C, 2)
      end,
  {atomic,E}=mnesia:transaction(F),
  E.
import(Count)->
  case gen_server:call({global,packet_sender}, {packetstream,"/media/TREKSTOR/videos/Der Blutige Pfad Gottes - German (DVD-Quali).avi",-1,-1,1000  })of
    %  case gen_server:call({global,packet_sender}, {packetstream,"/media/TREKSTOR/videos/ChocolateFactory.ts",-1,-1,1000  })of
    hivetimeout->
      hivetimeout;
    Any->
      %-record(packet,{id,pts, dts,data, data_size}).
      Result=mnesia:transaction(
        fun()->
            [
              mnesia:write(#packet{
              id=libdb:sequence(packet),
              pts=element(3,X),
              data=element(8,X)})||X<-Any
            ]
        end),
      io:format("Data Write ~w",[Result]),
      NewCount= if Count > 10 ->
                    mnesia:dump_log(),
                    0;
                  true->Count+1
                end,
      case Result of
        {atomic,[]}->ok;
        _->
          import(NewCount)
      end
  end.
dirtyimport()->
  case gen_server:call({global,packet_sender}, {packetstream,"/media/TREKSTOR/videos/Der Blutige Pfad Gottes - German (DVD-Quali).avi",-1,-1,1000  })of
    hivetimeout->
      hivetimeout;
    Any->
      %-record(packet,{id,pts, dts,data, data_size}).

      %Result=[mnesia:dirty_write(#packet{
      %              id=libdb:sequence(packet),
      %              pts=element(3,X),
      %              data=element(8,X)})||X<-Any
      %            ],
      Result=mnesia:dirty_write(#packet{
        id=libdb:sequence(packet),
        data=Any}),
      io:format("Data Write ~w",[Result]),
      if length(Any)
        >0->dirtyimport();
        true->
          ok
      end
  end.
gridimport()->
  {ok, Pid}=gridfile:open("grid","testfile"),

  case gen_server:call({global,packet_sender}, {packetstream,"/media/TREKSTOR/videos/Der Blutige Pfad Gottes - German (DVD-Quali).avi",-1,-1,100  })of
    hivetimeout->
      hivetimeout;
    Any->
      %-record(packet,{id,pts, dts,data, data_size}).

      %Result=[mnesia:dirty_write(#packet{
      %              id=libdb:sequence(packet),
      %              pts=element(3,X),
      %              data=element(8,X)})||X<-Any
      %            ],
      gridfile:insert(Pid,{Any, {bla,3}})
  end.


export(cursor)->
  F = fun() ->
          Q = qlc:q([[PU] || PU <- qlc:keysort(5,mnesia:table(process_unit),{order, descending}), element(4,PU)==45]),
          C = qlc:cursor(Q),
          qlc:next_answers(C, 1)
      end,
  {atomic,E}=mnesia:transaction(F),
  E;
export(FileNo) when is_integer(FileNo)->
  {atomic,E}=mnesia:transaction(fun()->qlc:e(qlc:q([{PU#job.id,filename:join(F#file.path, F#file.filename)} || PU <- qlc:keysort(2,mnesia:table(job)),F<-mnesia:table(file), PU#job.complete_time > undefined, F#file.id==PU#job.outfile,F#file.id==FileNo]))end),
    io:format("open File ~p",[E]),
%  T=[X||X<-E,not filelib:is_regular(element(2,X))],
  [FileName|_]=E,
  case filelib:is_regular(element(2,FileName)) of
    false->
  F2=[X||X<-E,filelib:ensure_dir(element(2,FileName))==ok],
  Fun2=fun(JobId)->
    DataFun=fun()->
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
    DataFun=fun()->
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
    Port = open_port({spawn, "/home/jhoelscher/MediaEncodingCluster/build/src/erl/mhivewriter"}, [{packet, 4}, binary]),
%  Port=[],
  Port ! {self(), {command, term_to_binary({createfile,list_to_atom(element(2,FileName))})}},
  {atomic,St}=mnesia:transaction(fun()->qlc:e(qlc:q([S || S <- qlc:keysort(2,mnesia:table(stream)), S#stream.fileid==FileNo]))end),
  Re=[Port!{self(), {command, term_to_binary({addstream,S, element(4,S)})}}||S<-St],
    Port ! {self(), {command, term_to_binary({initfile})}},

%    {ok,Pid}=file:open("Merge.data", write),%dets:open_file(filename:join(["tmp", integer_to_list(ProcId)]),[]),
 %   io:write(Pid, Data),

    B=[FileWriter(element(2,X), Port)||X<-Result],
    Port ! {self(), {command, term_to_binary({closefile})}},
    Port ! {self(), close},

%    file:close(Pid),
% {atomic,E3}=mnesia:transaction(Fun(13)),
  file_created;
  true->
    fileexist
  end;

export(select)->
  {atomic,E}=mnesia:transaction(fun()->qlc:e(qlc:q([PU#job.outfile|| PU <- qlc:keysort(2,mnesia:table(job)), PU#job.complete_time > undefined]))end),
  [export(F)||F<-E].
%ok.
grid()->
  %  {ok, Pid}=gridfile:new("grid","testfile",[{dimensions, 2}]),
  {ok, Pid}=gridfile:open("grid","testfile"),
  gridfile:insert(Pid,{myobject2, {bla,3}}).

config(Key)->
  config:get(Key).


arguments(Args)->
  io:format("PArgs:~p",[Args]).




