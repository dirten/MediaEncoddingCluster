-module (web_samples).
-include ("nitrogen/include/wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).

main() ->
  {ok,Val}=application:get_env(wwwroot),
  Temp=string:concat(Val,"/onecolumn.html"),
  #template { file=Temp, bindings=[
    {'Group', media},
    {'Item', medialist}
                                  ]}.
title() -> "Test List".
%headline() -> "".
%right() -> "ok".%linecount:render().



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
          Q = qlc:q([[Transform(element(4,E)),Transform(element(3,E)),Transform(element(5,E)),{data,element(2,E)}] || E <- mnesia:table(file)]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
  E.

get_map() -> [titleLabel@text, authorLabel@text, descriptionLabel@text, myButton@postback].

body() ->
	Data = get_data(),
	Map = get_map(),
	Column2 = [
		#h3 { text="Media Files" },
		#table { class=tiny, rows=[
			#tablerow { cells=[
				#tableheader { text="Path" },
				#tableheader { text="File" },
				#tableheader { text="SIze" },
				#tableheader { }
			]},
			#bind { id=tableBinding, data=Data, map=Map, body=#tablerow { cells=[
				#tablecell { id=titleLabel },
				#tablecell { id=authorLabel },
				#tablecell { id=descriptionLabel },
				#tablecell { body=#button { id=myButton, text="Details" } }
			]}}
		]}
	],
	wf:render(Column2).

event({data, Data}) ->
	Message = "Clicked On Data: " ++ wf:to_list(Data),
	wf:wire(#alert { text=Message }),
	ok;
event(_) ->
  io:format("Message:leer",[]),
ok.
