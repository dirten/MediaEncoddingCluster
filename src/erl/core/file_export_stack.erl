%%%----------------------------------------------------------------------
%%% File    : file_export_stack.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Handles the Audio/Video Packets from the Encoded Packets
%%%           and brings them into the right order to write them to Outfile
%%% Created : 4 Jun 2009 by Jan Hölscher <jan.hoelscher@esblab.com>
%%%
%%%
%%% MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
%%%
%%% This program is free software; you can redistribute it and/or
%%% modify it under the terms of the GNU General Public License as
%%% published by the Free Software Foundation; either version 2 of the
%%% License, or (at your option) any later version.
%%%
%%% This program is distributed in the hope that it will be useful,
%%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%%% General Public License for more details.
%%%
%%% You should have received a copy of the GNU General Public License
%%% along with this program; if not, write to the Free Software
%%% Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
%%% 02111-1307 USA
%%%
%%%----------------------------------------------------------------------
-module(file_export_stack).

-compile(export_all).

file_loader(FileId)->
  case file:read_file(filename:join(["data", integer_to_list(FileId)])) of
    {ok,D}->
      Data=binary_to_term(D),
%      ets:insert(packetlist,Data),
      DTmp=lists:flatten([lists:map(fun file_export_stack:map_ts/1,Data)|[{0,0,list_to_integer(element(3,lists:last(Data)))+1,"0",load_trigger,0,0,<<0>>}]]),
%      io:format("~p",[DTmp]),
      DTmp;
    {error,enoent}->
      io:format("File ~p Not Found",[FileId]),
      [{0,0,999999,"0",load_trigger,0,0,<<0>>}]
  end.

prepare(Result)->
  case ets:info(packetlist) of
    undefined->
      ets:new(packetlist,[named_table, duplicate_bag]);
    _->
      ets:delete_all_objects(packetlist)
  end,
  [file_loader(element(2,X))||X<-Result],
  %  [Data||Data<-lists:keysort(2,lists:map(fun(D)->{element(1,D),list_to_integer(element(3,D))} end,ets:tab2list(packetlist)))].
  lists:nthtail(20,[Data||Data<-lists:keysort(3,ets:tab2list(packetlist))]).

table(FileList) ->
  {FirstTwoFiles,Rest}=lists:split(2, FileList),
  Tmp=lists:flatten([file_loader(element(2,X))||X<-FirstTwoFiles]),
  %  [Data||Data<-lists:keysort(2,lists:map(fun(D)->{element(1,D),list_to_integer(element(3,D))} end,ets:tab2list(packetlist)))].
  %  PacketList=[D||D<-lists:keysort(3,ets:tab2list(packetlist))],
  io:format("Result Loaded"),
  List=lists:keysort(3,Tmp),
  io:format("Result sorted"),

  TF =
    fun() ->
        io:format("first tf"),
        qlc_next(List, Rest)
    end,
  qlc:table(TF, []).

qlc_next([],[])->
  [];
qlc_next(List, FileList)when length(List)>0 andalso length(FileList)>0->
  ChunkSize=
    if
      length(List)>=20 -> 20;
      true->length(List)
    end,
  {Val,T}=lists:split(ChunkSize,List),
  {Packets,PacketList,LastFiles}=
    case lists:keyfind(load_trigger,5,Val) of
      false->{Val,T,FileList};
      Any->
        io:format("loading chunk:~p~n",[Any]),
        [ToLoad|Rest]=FileList,
        {lists:delete(Any, Val),lists:keysort(3,lists:flatten([T|file_loader(element(2,ToLoad))])),Rest}
    end,
  %  [Val|T]=List,
  %  io:format("Value List:~p~n",[Val]),
  [Packets|qlc_next(PacketList, LastFiles)];
qlc_next(_,_)->
  [].
%{0,1,"3759840833","3759840833",1,3600,50563,  <<0,0,1,176,1,0,0,1,181,137,19,0,0,1,0,0,0,1,32,0,...>>}
map_ts(A)->
  {element(1,A),element(2,A),libutil:toInteger(element(3,A)),element(4,A),element(5,A),element(6,A),element(7,A),element(8,A)}.

packet_viewer(FileId)->
  case file:read_file(filename:join(["data", integer_to_list(FileId)])) of
    {ok,Data}->
      binary_to_term(Data);
    {error,enoent}->
      io:format("File ~p Not Found",[FileId]),
      nodata
  end.

next()->
  ok.
