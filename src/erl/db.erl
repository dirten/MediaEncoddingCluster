-module(db).
-export([init/0, delete/0]).
-include("schema.hrl").

init()->ok.
%	mnesia:create_table(packet,[{disc_copies, [node()]},{attributes, record_info(fields, packet)}]).
	
delete()->ok.
%	mnesia:delete_table(packet).
	

