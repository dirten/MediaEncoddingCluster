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
    {ok,Data}->
      ets:insert(packetlist,binary_to_term(Data)),
      ok;
    {error,enoent}->
      io:format("File ~p Not Found",[FileId]),
      nodata
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

table(_T)->
  ok.

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
