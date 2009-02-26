-module(web_folder_edit).

-include ("wf.inc").
-include ("schema_watchfolder.hrl").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).


main() ->
    {ok,Val}=application:get_env(wwwroot),
    Temp=string:concat(Val,"/onecolumn.html"),
    #template { file=Temp, bindings=[
	{'Group', media},
	{'Item', folder}
]}.
title() -> "Watch Folders Edit".

get_data(Id)->
  Transform=fun(Data)->
                if
                  is_integer(Data) ==true ->integer_to_list(Data);
                  is_tuple(Data) ==true ->tuple_to_list(Data);
                  is_atom(Data) ==true ->atom_to_list(Data);
                  true->Data
                end
            end,
  F = fun() ->
          Q = qlc:q([{
            Transform(element(1,E)),
            Transform(element(2,E)),
            Transform(element(3,E)),
            Transform(element(4,E)),
            Transform(element(5,E)),
            Transform(element(6,E))
                     } || E <- mnesia:table(watchfolder), element(2,E)=:=Id]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
  E.


body()->
  [Id|_]=wf:q(id),
  io:format("PostId:~w",[Id]),
  G=list_to_integer(Id),
  if
    G > 0->
      [Data|_]=get_data(list_to_integer(Id));
    true->
      Data={"","","","","","",""}
  end,

%  [Data|_]=get_data(list_to_integer(Id)),
%  io:format("PostData:~w",[Data]),
    ProfileData=#table { class=tiny, rows=[
    #tablerow { cells=[
      #tablecell { body=#label { text="Folder Id" } },
      #tablecell { body=#value { text=element(2,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Input Folder" } },
      #tablecell { body=#textbox { text=element(3,Data), id=pName} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Output Folder" } },
      #tablecell { body=#textbox { text=element(4,Data), id=pExt} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile" } },
      #tablecell { body=#textbox { text=element(5,Data), id=pFormat} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="File Extension Filter" } },
      #tablecell { body=#textbox {  text=element(6,Data), id=pFormat} }
    ]}
  ]},
    [#h3 { text=title() },
      #hr{},
    ProfileData,
    #hr{},
    #button { class=tiny,id=saveProfile,text="Save Profile", postback=save },
    #button { class=tiny,id=cancel,text="Cancel", postback=cancel }
].

event(save) ->
  ok;
event(cancel) ->
    wf:redirect("/web/folder").
