-module(web_folder).

-include ("wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).


main() ->
    {ok,Val}=application:get_env(wwwroot),
    Temp=string:concat(Val,"/onecolumn.html"),
    #template { file=Temp, bindings=[
	{'Group', media},
	{'Item', folder}
]}.
title() -> "Import Folders".


get_data()->
  Transform=fun(Data)->
                if
                  is_integer(Data) ==true ->integer_to_list(Data);
                  is_tuple(Data) ==true ->tuple_to_list(Data);
                  is_atom(Data) ==true ->atom_to_list(Data);
                  true->Data
                end
            end,
  F = fun() ->
          Q = qlc:q([[
            Transform(element(3,E)),
            Transform(element(4,E)),
            Transform(element(5,E)),
            Transform(element(6,E)),
            {data,element(2,E)}] || E <- mnesia:table(watchfolder)]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
%  io:format("Data:~w",[E]),
  E.
  get_map() -> [
    infolderLabel@text,
    outfolderLabel@text,
    profileLabel@text,
    filterLabel@text,
    myButton@postback].


body() ->
	Data = get_data(),
	Map = get_map(),
	Column2 = [
		#h3 { text=title()},
    #hr{},
		#table { class=tiny, rows=[
			#tablerow { cells=[
				#tableheader { text="Infolder" },
				#tableheader { text="Outfolder" },
				#tableheader { text="Profile" },
				#tableheader { text="File Filter" },
				#tableheader { }
			]},
%			#tablerow { cells=[
%				#tablecell {body=#hr{}, colspan=4}
%			]},
			#bind { id=tableBinding, data=Data, map=Map,transform=fun alternate_color/2, body=#tablerow { id=top,cells=[
				#tablecell { id=infolderLabel },
				#tablecell { id=outfolderLabel },
				#tablecell { id=profileLabel },
				#tablecell { id=filterLabel },
				#tablecell { body=#button { id=myButton, text="Details" } }
			]}}
		]}
	],
	wf:render(Column2).
event({data, Data}) ->
	Message = "Clicked On Data: " ++ wf:to_list(Data),
wf:wire(#alert { text=Message }),
    wf:redirect(wf:f("/web/folder/edit/?id=~w",[Data]));
event(_) ->
  io:format("Message:leer",[]),
ok.


%%% ALTERNATE BACKGROUND COLORS %%%
alternate_color(DataRow, Acc) when Acc == []; Acc==odd ->
	{DataRow, even, {top@style, "background-color: #eee;"}};

alternate_color(DataRow, Acc) when Acc == even ->
	{DataRow, odd, {top@style, "background-color: #ddd;"}}.
