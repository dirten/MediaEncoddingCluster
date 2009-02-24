-module(web_profile).
-include ("nitrogen/include/wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).



title()->
  "Profiles".
main() ->
  {ok,Val}=application:get_env(wwwroot),
  Temp=string:concat(Val,"/onecolumn.html"),
  #template { file=Temp, bindings=[
    {'Group', profile},
    {'Item', profile}
                                  ]}.
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
            Transform(element(5,E)),
            Transform(element(4,E)),
            Transform(element(6,E)),
            Transform(element(7,E)),
            Transform(element(8,E)),
            Transform(element(9,E)),
            Transform(element(10,E)),
            Transform(element(12,E)),
            Transform(element(11,E)),
            Transform(element(13,E)),
            Transform(element(14,E)),
            {data,element(2,E)}
                     ] || E <- mnesia:table(profile)]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
  %  io:format("Data:~w",[E]),
  E.

get_map() -> [
  nameLabel@text,
  formatLabel@text,
  extLabel@text,
  vcodecLabel@text,
  vbitrateLabel@text,
  vframerateLabel@text,
  vwidthLabel@text,
  vheightLabel@text,
  acodecLabel@text,
  achannelsLabel@text,
  abitrateLabel@text,
  asamplerateLabel@text,
  myButton@postback].

body() ->
	Data = get_data(),
	Map = get_map(),
	Column2 = [
		#h3 { text=title() },
    #hr{},
		#table { class=tiny, rows=[
    #tablerow { cells=[
    #tableheader { text="Name" },
    #tableheader { text="Format" },
    #tableheader { text="Ext" },
    #tableheader { text="Video Codec" },
    #tableheader { text="Video Bitrate" },
    #tableheader { text="Video Framerate" },
    #tableheader { text="Video Width" },
    #tableheader { text="Video Height" },
    #tableheader { text="Audio Codec" },
    #tableheader { text="Audio Channels" },
    #tableheader { text="Audio Bitrate" },
    #tableheader { text="Audio Samplerate" },
    #tableheader { }
                      ]},
                        #bind { id=tableBinding, data=Data,transform=fun alternate_color/2, map=Map, body=#tablerow { id=top, cells=[
                        #tablecell { id=nameLabel },
                        #tablecell { id=formatLabel },
                        #tablecell { id=extLabel },
                        #tablecell { id=vcodecLabel },
                        #tablecell { id=vbitrateLabel },
                        #tablecell { id=vframerateLabel },
                        #tablecell { id=vwidthLabel },
                        #tablecell { id=vheightLabel },
                        #tablecell { id=acodecLabel },
                        #tablecell { id=achannelsLabel },
                        #tablecell { id=abitrateLabel },
                        #tablecell { id=asamplerateLabel },
                        #tablecell { body=#button { id=myButton, text="Details" } }
                                                                                                                                    ]}}
                              ]}
            ],
	wf:render(Column2).


%event(go) ->ok.

%	wf:flash("Hello there"),
%	wf:update(test, "This is a test.");
event({data, Data}) ->
	Message = "Clicked On Data: " ++ wf:to_list(Data),
	wf:wire(#alert { text=Message }),
	ok;
event(_) ->
  io:format("Message:leer",[]),
  ok.

%%% ALTERNATE BACKGROUND COLORS %%%
alternate_color(DataRow, Acc) when Acc == []; Acc==odd ->
	{DataRow, even, {top@style, "background-color: #eee;"}};

alternate_color(DataRow, Acc) when Acc == even ->
	{DataRow, odd, {top@style, "background-color: #ddd;"}}.

