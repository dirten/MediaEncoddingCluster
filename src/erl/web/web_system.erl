-module (web_system).
-include ("nitrogen/include/wf.inc").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).

-import(libweb,[alternate_color/2]).

main() ->
  {ok,Val}=application:get_env(wwwroot),
  Temp=string:concat(Val,"/onecolumn.html"),
  #template { file=Temp, bindings=[
    {'Group', media},
    {'Item', system}
                                  ]}.
title() -> "System Overview".
%headline() -> "".
%right() -> "ok".%linecount:render().


get_data()->
  Fun=fun(Data)->
    case Data of
      client->
        "Client";
      server->
        "Server";
      both->
        "Server / Client";
      _->
        "Not running"
      end

  end,
  E=[[atom_to_list(X),Fun(rpc:call(X,config,get,[mode]))]||X<-[node()|nodes()]],
  E.

get_map() -> [nodeLabel@text,typeLabel@text].




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
      #tablecell { body=#label { text="Connect to Node" } },
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
				#tableheader { text="Visible Nodes" },
				#tableheader { text="Mode" }
			]},
%			#tablerow { cells=[
%				#tablecell {body=#hr{}, colspan=4}
%			]},
			#bind { id=tableBinding, data=Data, map=Map,transform=fun libweb:alternate_color/2, body=#tablerow { id=top,cells=[
				#tablecell { id=nodeLabel },
				#tablecell { id=typeLabel }
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
      case libnet:connect(H,N) of
        {ok,_Node}->  wf:flash("sucessfully Connected to Server ");
        {error,Msg}->wf:flash(Msg)
      end;
    true->  wf:flash("please give a Server Name")
  end.

