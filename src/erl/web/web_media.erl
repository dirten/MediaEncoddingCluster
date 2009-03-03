-module (web_media).
-include ("wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).

main() ->
  {ok,Val}=application:get_env(wwwroot),
  Temp=string:concat(Val,"/onecolumn.html"),
  #template { file=Temp, bindings=[
    {'Group', media},
    {'Item', media}
                                  ]}.
title() -> "Media List".

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
          Q = qlc:q([
            [
            Transform(element(2,E)),
            Transform(element(4,E)),
            Transform(element(3,E)),
            Transform(element(5,E)),
            {data,element(2,E)}
            ] || E <- qlc:keysort(2,mnesia:table(file),{order, ascending})]),
          qlc:e(Q)
          %          C = qlc:cursor(Q),
          %          qlc:next_answers(C, 1)
      end,
  {atomic,E}=mnesia:transaction(F),
  %  io:format("Data:~w",[E]),
  E.

get_map() -> [idLabel@text,titleLabel@text, authorLabel@text, descriptionLabel@text, myButton@postback].


body() ->
	Data = get_data(),
	Map = get_map(),
	Column2 = [
		#h3 { text="Media Files" },
    #hr{},
    #table { class=tiny, rows=[
    #tablerow { cells=[
    #tableheader { text="#" },
    #tableheader { text="Path" },
    #tableheader { text="File" },
    #tableheader { text="Size | Bytes" },
    #tableheader { }
                      ]},
                        #bind { id=tableBinding, data=Data, map=Map,transform=fun alternate_color/2, body=#tablerow { id=top,cells=[
                        #tablecell { id=idLabel},
                        #tablecell { id=titleLabel},
                        #tablecell { id=authorLabel },
                        #tablecell { id=descriptionLabel },
                        #tablecell { body=#button { id=myButton, text="Details" } }
                                                                                                                                   ]}}
                              ]}
            ],
	wf:render(Column2).
%  #rounded_panel { color=black, body=[
%	#h3 { text="Title" },
%	Column2
%]}.


%event(go) ->ok.

%	wf:flash("Hello there"),
%	wf:update(test, "This is a test.");
event({data, Data}) ->
  wf:redirect(wf:f("/web/media/detail/?id=~w",[Data]));
event(Data) ->
  io:format("Message:~w",[Data]),
  ok.


%%% ALTERNATE BACKGROUND COLORS %%%
alternate_color(DataRow, Acc) when Acc == []; Acc==odd ->
  %  io:format("DataRow:~s",[DataRow]),
	{DataRow, even, [{top@style, "background-color: #eee;"}]};

alternate_color(DataRow, Acc) when Acc == even ->
	{DataRow, odd, {top@style, "background-color: #ddd;"}}.