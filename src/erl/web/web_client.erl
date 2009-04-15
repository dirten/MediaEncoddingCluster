-module(web_client).

-include ("wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).


main() ->
    {ok,Val}=application:get_env(wwwroot),
    Temp=string:concat(Val,"/onecolumn.html"),
    #template { file=Temp, bindings=[
	{'Group', client},
	{'Item', client}
]}.
title() -> "Client Configuration".


get_data()->
  E=[[atom_to_list(X)]||X<-[node()|nodes()]],
  E.

get_map() -> [nodeLabel@text].




body()->
	Data = get_data(),
	Map = get_map(),
  ProfileData=#table { class=tiny, rows=[
    #tablerow { cells=[
      #tablecell { body=#label { text="Self" } },
      #tablecell { body=#value { text=""} },
      #tablecell { body=#label { text=atom_to_list(node())} }
    ]},
%    #tablerow { cells=[
%      #tablecell { body=#label { text="Server" } },
%      #tablecell { body=#value { text=""} },
%      #tablecell { body=#label { text=atom_to_list(node(global:whereis_name(packet_server)))} }
%    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Connect to Server" } },
      #tablecell { body=#value { text=""} },
      #tablecell { body=#textbox { text="", id=pHost} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Node Name (optional)" } },
      #tablecell { body=#value { text=""} },
      #tablecell { body=#textbox { text="node", id=pNodeName} },
      #tablecell { body=#label { text="default is 'node'"} }
    ]}
  ]},
  NodeTable=#table { class=tiny, rows=[
			#tablerow { cells=[
				#tableheader { text="Visible Nodes" }
			]},
%			#tablerow { cells=[
%				#tablecell {body=#hr{}, colspan=4}
%			]},
			#bind { id=tableBinding, data=Data, map=Map,transform=fun alternate_color/2, body=#tablerow { id=top,cells=[
				#tablecell { id=nodeLabel }
			]}}
		]},
  Result=[
    #h3 { text=title() },
    #hr{},
    #table { class=tiny, rows=[
      #tablerow { cells=[
        #tablecell { valign=top,body=ProfileData },
        #tablecell { valign=top,body=NodeTable }
      ]}
    ]},
    #hr{},
    #button { class=tiny,id=saveProfile,text="Connect", postback=save },
    #button { class=tiny,id=cancel,text="Cancel", postback=cancel }
  ],
  Result.

event(save) ->
  [H]=wf:q(pHost),
  [N]=wf:q(pNodeName),
  if
    length(H)>0 ->
      case connect(H,N) of
        {ok,_Node}->  wf:flash("sucessfully Connected to Server ");
        error->wf:flash("Connection to Server failed")
      end;
    true->  wf:flash("please give a Server Name")
  end
%  wf:insert_bottom(nodeLabel, #panel { body=node(), actions=#show { effect=pulsate, options=[{times, 1}] }})
.

connect(Host, NodeName)->
  case inet:getaddr(Host, inet) of
    {ok, Addr}->
      io:format("Connecting to Host Address ~w~n",[Addr]),
      {A1,A2,A3,A4}=Addr,
      Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
      Node=list_to_atom(NodeName++"@"++Ip),
      case net_adm:ping(Node)of
        pong->
          io:format("Visible Nodes ~p~n",[[node()|nodes()]]),
          {ok,Node};
        pang->
          wf:flash("no pssible Service found"),

          io:format("No Services ~p found on Host with Address ~p ~p~n",[NodeName,Host,Node]),
%          {ok,Data}=net_adm:names(Ip),
%          Services=[element(1,X)||X<-Data],
%          io:format("Available Services on Host Address ~p~n",[Services]),
          error
       end;
    {error, nxdomain}->
      wf:flash("Can not resolve Host Address"),
      io:format("Can not resolve Host Address for ~p~n",[Host]),
    error
    end.
%%% ALTERNATE BACKGROUND COLORS %%%
alternate_color(DataRow, Acc) when Acc == []; Acc==odd ->
	{DataRow, even, {top@style, "background-color: #eee;"}};

alternate_color(DataRow, Acc) when Acc == even ->
	{DataRow, odd, {top@style, "background-color: #ddd;"}}.
