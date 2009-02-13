-module(test).

-export([dirtyimport/0,import/1,insert/0, read/0, read2/0, read3/0, read4/0, read5/1, create/0, drop/0,sequence/1, load/0, clear/0]).
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
