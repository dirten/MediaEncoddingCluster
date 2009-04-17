-module (web_samples).
-include ("nitrogen/include/wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).

main2()->
  #template{}.


main() ->
  {ok,Val}=application:get_env(wwwroot),
  Temp=string:concat("wwwroot","/empty.html"),
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
	wf:render(Column2),
%  io:format("Data:~p",[Data]),

  Bla=[
   #list{id="tree1", class="tree",body=#listitem{text="MediaFiles", class="folder f-close root", body=#list{body=[#listitem{id="link1",class="doc",body=#link{text=lists:nth(2,D)}}||D<-Data]}}}
  ],
  Bla2=wf:render(Bla),
  io:format("BlaBlah:~p",[Bla2]),
    ["<ul id='tree1' class='tree'><li class='folder f-close root'>MediaFiles<ul><li id='link1' class='doc'><span><a>ChocolateFactory.ts</a></span></li><li id='link2' class='doc'><span><a>ChocolateFactory.avi</a></span></li></ul></li></ul>"].

event({data, Data}) ->
	Message = "Clicked On Data: " ++ wf:to_list(Data),
	wf:wire(#alert { text=Message }),
	ok;
event(_) ->
  io:format("Message:leer",[]),
ok.
