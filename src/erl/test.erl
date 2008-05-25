-module(test).
-export([test/0, init/0, test_run/0, free/0]).

-record(tpacket, {id,data}).


create_data()->
	for(1,50000,fun(I)->I+I end).

init()->
		mnesia:create_table(tpacket,[{disc_only_copies, [node()]},{attributes, record_info(fields, tpacket)}]).

free()->
		mnesia:delete_table(tpacket).

test_run()->
	for(1,5000,fun(A)->test() end).
test()->
	mnesia:dirty_write(tpacket,#tpacket{id=db:sequence(test),data=create_data()}).

for(Max,Max,F)->
	[F(Max)];
for(I,Max,F)->
	[F(I)|for(I+1,Max,F)].