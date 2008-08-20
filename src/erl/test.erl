-module(test).
-export([test/0, init/0, test_run/0, free/0]).

-record(tpacket, {id,data}).


create_data()->
	for(1,500,fun(I)->I+I end).

init()->
		mnesia:create_table(tpacket,[{disc_copies, [node()]},{attributes, record_info(fields, tpacket)}]),
        mnesia:change_table_frag(tpacket, {activate, []}),
		Info = fun(Item) -> mnesia:table_info(tpacket, Item) end,
		mnesia:change_table_frag(tpacket, {add_frag,mnesia:activity(sync_dirty, Info, [frag_dist], mnesia_frag)}),
		mnesia:change_table_frag(tpacket, {add_frag,mnesia:activity(sync_dirty, Info, [frag_dist], mnesia_frag)}),
		mnesia:change_table_frag(tpacket, {add_frag,mnesia:activity(sync_dirty, Info, [frag_dist], mnesia_frag)}),
		mnesia:change_table_frag(tpacket, {add_frag,mnesia:activity(sync_dirty, Info, [frag_dist], mnesia_frag)}),
		mnesia:change_table_frag(tpacket, {add_frag,mnesia:activity(sync_dirty, Info, [frag_dist], mnesia_frag)}),
		mnesia:change_table_frag(tpacket, {add_frag,mnesia:activity(sync_dirty, Info, [frag_dist], mnesia_frag)}),
		mnesia:change_table_frag(tpacket, {add_frag,mnesia:activity(sync_dirty, Info, [frag_dist], mnesia_frag)}),
		mnesia:change_table_frag(tpacket, {add_frag,mnesia:activity(sync_dirty, Info, [frag_dist], mnesia_frag)}).
%%		mnesia:change_table_frag(Tab, {activate, []}).

free()->
		mnesia:delete_table(tpacket).

test_run()->
	for(1,100000,fun(A)->test() end).

test()->
    D=create_data(),
    Write = fun(Keys) -> [mnesia:write(Keys)], ok end,
    mnesia:activity(sync_dirty, Write, [#tpacket{id=db:sequence(test), data=D}], mnesia_frag).
%%	mnesia:dirty_write(tpacket,#tpacket{id=db:sequence(test),data=term_to_binary(create_data())}).

test_trans()->
	Fun=fun()->
		mnesia:write(#tpacket{id=db:sequence(test),data=term_to_binary(create_data())})
	end,
	mnesia:transaction(Fun).

for(Max,Max,F)->
	[F(Max)];
for(I,Max,F)->
	[F(I)|for(I+1,Max,F)].
	
	