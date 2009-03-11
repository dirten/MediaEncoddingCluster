-module(libdb).
-export([create/0, sequence/1, clear/0]).
-include("schema.hrl").
-include_lib("stdlib/include/qlc.hrl").
-record(sequence, {key, index}).

%%

create()->
  mnesia:create_table(file,[{disc_copies, [node()]},{attributes, record_info(fields, file)}]),
  mnesia:create_table(watchfolder,[{disc_copies, [node()]},{attributes, record_info(fields, watchfolder)}]),
  mnesia:create_table(stream,[{disc_copies, [node()]},{attributes, record_info(fields, stream)}]),
  mnesia:create_table(profile,[{disc_copies, [node()]},{attributes, record_info(fields, profile)}]),
  mnesia:create_table(job,[{disc_copies, [node()]},{attributes, record_info(fields, job)}]),
  mnesia:create_table(jobdetail,[{disc_copies, [node()]},{attributes, record_info(fields, jobdetail)}]),
  mnesia:create_table(process_unit,[{disc_only_copies, [node()]},{attributes, record_info(fields, process_unit)}]),
  mnesia:create_table(packet,[{disc_only_copies, [node()]},{attributes, record_info(fields, packet)}]),
  mnesia:create_table(config,[{disc_only_copies, [node()]},{attributes, record_info(fields, config)}]),
  mnesia:create_table(sequence, [{type, set}, {disc_copies,[node()]}, {attributes, record_info(fields, sequence)}]).

drop()->
  mnesia:delete_table(file),
  mnesia:delete_table(watchfolder),
  mnesia:delete_table(stream),
  mnesia:delete_table(profile),
  mnesia:delete_table(job),
  mnesia:delete_table(jobdetail),
  mnesia:delete_table(process_unit),
  mnesia:delete_table(packet),
  mnesia:delete_table(config),
  mnesia:delete_table(sequence).

load()->
  mnesia:load_textfile("test.data").

clear()->
  drop(),
  create(),
  load().

sequence(Name, Inc) ->
 	     mnesia:dirty_update_counter(sequence, Name, Inc).

sequence(Name) ->
  sequence(Name, 1).