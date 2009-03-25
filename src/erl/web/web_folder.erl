-module(web_folder).

-include ("wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).
-import(libutil,[to_string/1]).
%-import(libweb,[alternate_color/2]).


main() ->
    {ok,Val}=application:get_env(wwwroot),
    Temp=string:concat(Val,"/onecolumn.html"),
    #template { file=Temp, bindings=[
	{'Group', media},
	{'Item', folder}
]}.
title() -> "Watch Folders".


get_data()->
  F = fun() ->
          Q = qlc:q([[
            to_string(element(3,E)),
            to_string(element(4,E)),
            to_string(element(5,E)),
            to_string(element(6,E)),
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
    #button { class=tiny,id=newProfile,text="Add a new Watchfolder", postback={data, -1} },
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
			#bind { id=tableBinding, data=Data, map=Map,transform=fun libweb:alternate_color/2, body=#tablerow { id=top,cells=[
				#tablecell { id=infolderLabel },
				#tablecell { id=outfolderLabel },
				#tablecell { id=profileLabel },
				#tablecell { id=filterLabel },
				#tablecell { body=#button { id=myButton, text="Edit Folder" } }
			]}}
		]}
	],
	wf:render(Column2).

event({data, Data}) ->
%	Message = "Clicked On Data: " ++ wf:to_list(Data),
%wf:wire(#alert { text=Message }),
    wf:redirect(wf:f("/web/folder/edit/?id=~w",[Data]));
event(_) ->
  io:format("Message:leer",[]),
ok.

