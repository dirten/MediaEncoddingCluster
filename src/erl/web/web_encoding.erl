-module(web_encoding).

-include ("wf.inc").
-include("schema_job.hrl").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).
-import(libutil,[to_string/1]).

main() ->
    {ok,Val}=application:get_env(wwwroot),
    Temp=string:concat(Val,"/onecolumn.html"),
    #template { file=Temp, bindings=[
	{'Group', media},
	{'Item', encoding}
]}.
title() -> "Encodings".

get_data()->
  NowToString=fun(Now)->
    case Now of
      undefined->
        no;
      _->
        {{Year,Month,Day},{Hour, Min, Sec}}=calendar:now_to_local_time(Now),
        integer_to_list(Year)++"-"++integer_to_list(Month)++"-"++integer_to_list(Day)++" "++integer_to_list(Hour)++":"++integer_to_list(Min)++":"++integer_to_list(Sec)
      end
  end,
  
  F = fun() ->
          Q = qlc:q([[
            to_string(element(3,E)),
            to_string(element(4,E)),
            to_string(element(5,E)),
            to_string(NowToString(element(6,E))),            %file.start_time                 %file.duration
            to_string(round(((list_to_integer(E#job.last_ts)-list_to_integer(element(12,F)))/list_to_integer(element(8,F)))*100)),
%            Transform(E#job.last_ts),
            {data,element(2,E)}] || E <- qlc:keysort(2,mnesia:table(job)),F<-mnesia:table(file),E#job.infile==element(2,F)]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
%  io:format("Data:~w",[E]),
  E.
  get_map() -> [
    infileLabel@text,
    outfileLabel@text,
    beginLabel@text,
    completeLabel@text,
    currentLabel@text,
    myButton@postback].


body() ->
	Data = get_data(),
	Map = get_map(),
	Column2 = [
		#h3 { text=title()},
    #hr{},
		#table { class=tiny, rows=[
			#tablerow { cells=[
				#tableheader { text="Infile" },
				#tableheader { text="Outfile" },
				#tableheader { text="Begin" },
				#tableheader { text="Complete" },
				#tableheader { text="% Done" },
				#tableheader { }
			]},
%			#tablerow { cells=[
%				#tablecell {body=#hr{}, colspan=4}
%			]},
			#bind { id=tableBinding, data=Data, map=Map,transform=fun alternate_color/2, body=#tablerow { id=top,cells=[
				#tablecell { id=infileLabel },
				#tablecell { id=outfileLabel },
				#tablecell { id=beginLabel },
				#tablecell { id=completeLabel },
				#tablecell { id=currentLabel }
%				#tablecell { body=#button { id=myButton, text="Details" } }
			]}}
		]}
	],
	wf:render(Column2).
event({data, Data}) ->
	Message = "Clicked On Data: " ++ wf:to_list(Data),
wf:wire(#alert { text=Message });
event(_) ->
  io:format("Message:leer",[]),
ok.


%%% ALTERNATE BACKGROUND COLORS %%%
alternate_color(DataRow, Acc) when Acc == []; Acc==odd ->
	{DataRow, even, {top@style, "background-color: #eee;"}};

alternate_color(DataRow, Acc) when Acc == even ->
	{DataRow, odd, {top@style, "background-color: #ddd;"}}.
