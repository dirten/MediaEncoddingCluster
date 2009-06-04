-module(file_scanner).
%-behaviour(gen_server).
-include("schema.hrl").
-include_lib("stdlib/include/qlc.hrl").
%-include("/usr/lib/erlang/lib/stdlib-1.15.1/include/qlc.hrl").
%-include("C:\\Programme\\erl5.6.5\\lib\\stdlib-1.15.5\\include/qlc.hrl").

-export([scan/0, process_file_list/3, process_file/1, filter/2, diff/2]).

scan()->
%  io:format("Scanning files"),
  E=libdb:read(watchfolder),
  Fun=fun(El)->
          case El#watchfolder.status of
            "active"->
              Recursive =
                if El#watchfolder.recursive =:="yes"->true;
                  El#watchfolder.recursive =:="no"->false;
                  true->false
                end,
              FileList=libfile:find(El#watchfolder.infolder,El#watchfolder.filter,Recursive),
%              io:format("FileList~p~n",[FileList]),
              process_file_list(FileList,El#watchfolder.profile,El#watchfolder.outfolder);
              _Else->
                do_nothing
          end
      end,
  lists:foreach(Fun,E).

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
  NewFile=#file{
    id=libdb:sequence(file),
    filename=string:join([filename:rootname(File#file.filename),Profile#profile.ext],"."),
    path=filename:join([OutPath|[string:join(string:tokens(Profile#profile.name," "),"_")]]),
    streamcount=File#file.streamcount,
    parent=Fileid
               },
  mnesia:write(NewFile),
  Pass=if
         Profile#profile.multipass =:=2->1;
         true->0
       end,
  Job=#job{id=libdb:sequence(job), infile=Fileid, outfile=NewFile#file.id, last_ts=File#file.start_time, profile=Profile#profile.id, pass=Pass},
  io:format("Job : ~p",[Job]),
  libdb:write(Job),
  %  mnesia:write(Job),

  SortedStreams=lists:keysort(4, Streams),
  [VS|Rest]=SortedStreams,


  %-record(stream,{id,fileid,streamidx,streamtype,codec,codecname,rate,num, den, width, height,channels,gop,format}).
  %-record(profile,{id,name,ext,vformat,vcodec,vbitrate,vframerate,vwidth,vheight,achannels,acodec,abitrate,asamplerate}).
  NewVideoStream=#stream{
    id=libdb:sequence(stream),
    fileid=NewFile#file.id,
    streamidx=VS#stream.streamidx,
    codec=Profile#profile.vcodec,
    bitrate=Profile#profile.vbitrate,
    rate=Profile#profile.vframerate,
    num=1,
    den=Profile#profile.vframerate,
    width=Profile#profile.vwidth,
    height=Profile#profile.vheight,
    gop=Profile#profile.gop,
    format=0},
  libdb:write(NewVideoStream),
  JobVideoDetail=#jobdetail{id=libdb:sequence(jobdetail), jobid=Job#job.id, instream=VS#stream.id, outstream=NewVideoStream#stream.id},
  libdb:write(JobVideoDetail),

  if length(Rest)>0->
      [AS|_Rest]=Rest,
      NewAudioStream=#stream{
        id=libdb:sequence(stream),
        fileid=NewFile#file.id,
        streamidx=AS#stream.streamidx,
        codec=Profile#profile.acodec,
        rate=Profile#profile.asamplerate,
        num=AS#stream.num,
        den=AS#stream.den,
        bitrate=Profile#profile.abitrate,
        %    width=Profile#profile.width,
                             %    height=Profile#profile.height,
                             channels=Profile#profile.achannels,
                               gop=20,
                               format=0},
      libdb:write(NewAudioStream),

      JobAudioDetail=#jobdetail{id=libdb:sequence(jobdetail), jobid=Job#job.id, instream=AS#stream.id, outstream=NewAudioStream#stream.id},
      libdb:write(JobAudioDetail);
    true->
      no
  end.
  
% io:format("FileJob info ~p ~p ~p~n",[File, Profile, NewVideoStream]).

save_stream_info(FileName,SID, FileId)->
  %  io:format("get stream info from~s~n",[FileName]),
  case gen_server:call(global:whereis_name(packet_sender), {streaminfo,FileName,SID,0,0}) of
    %    {stream,518,518,0,0,2,undefined,15000000,25,1,90000,720,576,0,12,0}
    %   {0,0,0,2,8000000,25,1,90000,720,576,0,12,0}
    {_Tmp,Index,SType,Codec,BitRate,Rate,TbNum,TbDen,Width,Height,Channels,Gop,Format, StartTime, Duration} ->
      Stream = #stream{
        id=libdb:sequence(stream),
        fileid=FileId,
        streamidx=Index,
        streamtype=SType,
        codec=Codec,
        bitrate=BitRate,
        rate=Rate,
        num=TbNum,
        den=TbDen,
        width=Width,
        height=Height,
        channels=Channels,
        gop=Gop,
        format=Format,
        start_time=list_to_integer(StartTime),
        duration=list_to_integer(Duration)
                      },
      io:format("Stream info ~w~n",[Stream]),
      libdb:write(Stream),
      save_stream_info(FileName,SID+1, FileId);
    Any->
      io:format("Any Stream info ~w~n",[Any])
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
                  {_FileName,_FilePath,Size,Type,StreamCount,Duration,BitRate, StartTime}  ->
                    io:format("try File import~s~n",[X]),
                    File = #file{
                      id=libdb:sequence(file),
                      filename=filename:basename(X),
                      path=filename:dirname(X),
                      size=Size,
                      containertype=Type,
                      streamcount=StreamCount,
                      duration=Duration,
                      bitrate=BitRate,
                      start_time=StartTime
                                },
                    libdb:write(File),
                    %                    io:format("get stream info from~s~n",[FileName]),
                    save_stream_info(X,0,File#file.id),
                    io:format("File imported~s~n",[filename:basename(X)]),
                    create_job(File#file.id, Profile, OutPath),
                    io:format("File Job Created ~s~n",[filename:basename(X)]);
                  {filenotfound}  ->
                    io:format("File not found ~w~n",[list_to_atom(X)]);
                  {format_invalid}  ->
                    ok;
                  %                           io:format("File Wrong format ~s~n",[X]);
                  Any->
                    io:format("AnyFileScanner ~w~n",[binary_to_term(Any)])
                end
            end)
      end,
  lists:foreach(Fun,Files);
process_file_list([],_Profile, _OutPath)->
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

