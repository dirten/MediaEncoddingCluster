%%%----------------------------------------------------------------------
%%% File    : libdb.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Database Helper
%%% Created : 18 Feb 2009 by Jan Hölscher <jan.hoelscher@esblab.com>
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
-module(libdb).
-export([create/0, sequence/1, clear/0, write/1, read/1, recreate/0, load_presets/0, columns/1]).
-include("schema.hrl").
-include_lib("stdlib/include/qlc.hrl").
-record(sequence, {key, index}).

%%
create()->
  mnesia:create_table(file,[{disc_copies, [node()]},{attributes, record_info(fields, file)}]),
  mnesia:create_table(media,[{disc_copies, [node()]},{attributes, record_info(fields, media)}]),
  mnesia:create_table(watchfolder,[{disc_copies, [node()]},{attributes, record_info(fields, watchfolder)}]),
  mnesia:create_table(stream,[{disc_copies, [node()]},{attributes, record_info(fields, stream)}]),
  mnesia:create_table(profile,[{disc_copies, [node()]},{attributes, record_info(fields, profile)}]),
  mnesia:create_table(job,[{disc_copies, [node()]},{attributes, record_info(fields, job)}]),
  mnesia:create_table(jobdetail,[{disc_copies, [node()]},{attributes, record_info(fields, jobdetail)}]),
  mnesia:create_table(process_unit,[{disc_copies, [node()]},{attributes, record_info(fields, process_unit)}]),
  mnesia:create_table(packet,[{disc_copies, [node()]},{attributes, record_info(fields, packet)}]),
  mnesia:create_table(config,[{disc_copies, [node()]},{attributes, record_info(fields, config)}]),
  mnesia:create_table(sequence, [{type, set}, {disc_copies,[node()]}, {attributes, record_info(fields, sequence)}]),
  mnesia:create_table(scheduler, [{type, set}, {disc_copies,[node()]}, {attributes, record_info(fields, scheduler)}]),
  mnesia:create_table(logging, [{type, set}, {disc_copies,[node()]}, {attributes, record_info(fields, logging)}]),
  mnesia:create_table(releases, [{type, set}, {disc_copies,[node()]}, {attributes, record_info(fields, releases)}]).

drop()->
  mnesia:delete_table(file),
  mnesia:delete_table(media),
  mnesia:delete_table(watchfolder),
  mnesia:delete_table(stream),
  mnesia:delete_table(profile),
  mnesia:delete_table(job),
  mnesia:delete_table(jobdetail),
  mnesia:delete_table(process_unit),
  mnesia:delete_table(packet),
  mnesia:delete_table(config),
  mnesia:delete_table(scheduler),
  mnesia:delete_table(releases),
  mnesia:delete_table(logging),
  mnesia:delete_table(sequence).

load_presets()->
  mnesia:load_textfile("default.data").

recreate()->
  drop(),
create().

clear()->
  drop(),
  create(),
  load_presets().

sequence(Name, Inc) ->
 	     mnesia:dirty_update_counter(sequence, Name, Inc).

sequence(Name) ->
  sequence(Name, 1).

write(Obj)->
  case mnesia:transaction(fun() ->mnesia:write(Obj)end) of
    {atomic, ok} ->
    ok;
    Err->
      io:format("Error in ~p:~p",[?MODULE,Err]),
      {error, Err}
  end.

read(Table)->
  F=fun()->
    qlc:e(qlc:q([Data||Data<-mnesia:table(Table)]))
  end,
  {atomic,Data}=mnesia:transaction(F),
  Data.


columns(TableName)->
  mnesia:table_info(TableName, attributes).
