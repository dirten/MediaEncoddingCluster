-module(file_scanner).
-behaviour(gen_server).
-include("schema.hrl").
-include_lib("stdlib/include/qlc.hrl").
%-include("/usr/lib/erlang/lib/stdlib-1.15.1/include/qlc.hrl").
%-include("C:\\Programme\\erl5.6.5\\lib\\stdlib-1.15.5\\include/qlc.hrl").

-export([start/0, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2, code_change/3, terminate/2,loop/0, process_file_list/3, process_file/1, filter/2, diff/2]).

start()->
  spawn(?MODULE,init,[[]]).

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

stop()->
  gen_server:call(?MODULE, stop).

init([])->
  io:format("~w start up~n", [?MODULE]),
  process_flag(trap_exit, true),
  Pid=spawn_link(?MODULE,loop,[]),
%  link(Pid),
  register(loop, Pid),
  io:format("~w started~n", [?MODULE]),
  {ok, state}.

%profile_creator()->
%  ok.

loop()->
  receive
    stop ->
      io:format("~w stop loop~n", [?MODULE]),
      ok
  after 10000->
      %      io:format("~w Loop entry1 ~n", [?MODULE]),
      F = fun() ->
              Q = qlc:q([E || E <- mnesia:table(watchfolder)]),
              qlc:e(Q)
          end,
      {atomic, E}=mnesia:transaction(F),
      Fun=fun(El)->
              FileList=filelib:wildcard(string:concat(El#watchfolder.infolder,"/*")),
              process_file_list(FileList,El#watchfolder.profile,El#watchfolder.outfolder)
          end,
      lists:foreach(Fun,E),
      loop()
  end.


create_job(Fileid,Profileid,OutPath)->
  F = fun() ->
          Q = qlc:q([E|| E <- mnesia:table(file),E#file.id == Fileid]),
          qlc:e(Q)
      end,
  {atomic,FileList}=mnesia:transaction(F),

  F2 = fun() ->
          Q = qlc:q([E|| E <- mnesia:table(profile),E#profile.id == Profileid]),
          qlc:e(Q)
      end,
  {atomic,ProfileList}=mnesia:transaction(F2),
  F3 = fun() ->
          Q = qlc:q([E|| E <- mnesia:table(stream),E#stream.fileid == Fileid]),
          qlc:e(Q)
      end,
  {atomic,Streams}=mnesia:transaction(F3),

  [Profile|_T]=ProfileList,
  [File|_T]=FileList,
%  NewFile=#file{id=test:sequence(file), path=OutPath, streamcount=2},
  NewFile=#file{id=test:sequence(file),filename=string:join([filename:rootname(File#file.filename),Profile#profile.ext],"."), path=filename:join([OutPath|[string:join(string:tokens(Profile#profile.name," "),"_")]]), streamcount=2},
  mnesia:write(NewFile),

  SortedStreams=lists:keysort(4, Streams),
  [VS|Rest]=SortedStreams,
  [AS|_Rest]=Rest,

  NewVideoStream=#stream{id=test:sequence(stream),fileid=NewFile#file.id,streamidx=VS#stream.streamidx},
  NewAudioStream=#stream{id=test:sequence(stream),fileid=NewFile#file.id,streamidx=AS#stream.streamidx},
  mnesia:write(NewVideoStream),
  mnesia:write(NewAudioStream),

  Job=#job{id=test:sequence(job), infile=Fileid, outfile=NewFile#file.id},
  mnesia:write(Job),
  JobVideoDetail=#jobdetail{id=test:sequence(jobdetail), jobid=Job#job.id, instream=VS#stream.id, outstream=NewVideoStream#stream.id},
  JobAudioDetail=#jobdetail{id=test:sequence(jobdetail), jobid=Job#job.id, instream=AS#stream.id, outstream=NewAudioStream#stream.id},
  mnesia:write(JobVideoDetail),
  mnesia:write(JobAudioDetail),
  
io:format("FileJob info ~w ~w ~w~n",[File, Profile, NewVideoStream]).

save_stream_info(FileName,SID, FileId)->
  %  io:format("get stream info from~s~n",[FileName]),
  case gen_server:call(global:whereis_name(packet_sender), {streaminfo,FileName,SID,0,0}) of
    {_Tmp,Index,SType,Codec,Rate,TbNum,TbDen,Width,Height,Channels,Gop,Format} ->
      Stream = #stream{id=test:sequence(stream),fileid=FileId,streamidx=Index,streamtype=SType,codec=Codec,rate=Rate,num=TbNum, den=TbDen, width=Width, height=Height,channels=Channels,gop=Gop,format=Format},
      mnesia:write(Stream),
      io:format("Stream info ~w~n",[Stream]),
      save_stream_info(FileName,SID+1, FileId);
    Any->
      io:format("Stream info ~w~n",[Any])
  end,
  ok.

process_file_list([H|T],Profile, OutPath)->
  F = fun() ->
          Q = qlc:q([filename:join([E#file.path,E#file.filename]) || E <- mnesia:table(file)]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),

%  Files =diff([H|T],E),
  Files =[H|T]--E,

  Fun=fun(X)->
          mnesia:transaction(
            fun()->
                case gen_server:call(global:whereis_name(packet_sender), {fileinfo,X,0,0,0}) of
                  {FileName,FilePath,Size,Type,StreamCount,Duration,BitRate}  ->
                    File = #file{id=test:sequence(file),filename=FileName, path=FilePath, size=Size, containertype=Type,streamcount=StreamCount,duration=Duration,bitrate=BitRate},
                    mnesia:write(File),
                    %                    io:format("get stream info from~s~n",[FileName]),
                    save_stream_info(X,0,File#file.id),
                    io:format("File imported~s~n",[FileName]),
                    create_job(File#file.id, Profile, OutPath),
                    io:format("File Job Created ~s~n",[FileName]);
                  {filenotfound}  ->
                    io:format("File not found ~w~n",[list_to_atom(X)]);
                  {format_invalid}  ->
                    ok;
                  %          io:format("File Wrong format ~w~n",[X]);
                  Any->
                    io:format("AnyFileScanner ~w~n",[binary_to_term(Any)])
                end
            end)
      end,
  lists:foreach(Fun,Files);
process_file_list([],Profile, OutPath)->
      ok.
%io:format("File ~s~n",[Files]);

%  process_file(T),
%  process_file_list(H),
%  ok;

process_file(File)->
  io:format("File ~s~n",[File]),
  ok.

filter(F, [H|T]) ->
  case F(H) of
    true  -> [H|filter(F, T)];
    false -> filter(F, T)
  end;
filter(_F, []) -> [].

diff(L1, L2) ->
  filter(fun(X) -> not lists:member(X, L2) end, L1).

handle_call({import_file,Thing},_From,_N)->
  io:format("~w handle_call import~w~n", [?MODULE,Thing]),
  {reply, Thing, state};

handle_call({scan,Directory},_From,_N)->
  io:format("~w handle_call scan~w~n", [?MODULE,Directory]),
  {reply, Directory, state}.

handle_cast(_Msg,N)->
  io:format("~w handle_cast~w~n", [?MODULE,N]),
  {noreply, N}.

%handle_info(Info,N)->
%  io:format("~w handle_info~w~n", [?MODULE,{Info,N}]),
%  {noreply, N}.

terminate(Reason,_N)->
  loop ! stop,
  io:format("~w shutdown ~w~n", [?MODULE, Reason]),
  ok.

code_change(_OldVsn,N,_Extra)->
  io:format("~w CodeChange ~n", [?MODULE]),
  {ok, N}.

