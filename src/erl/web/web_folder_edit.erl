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
  F=fun()->
    qlc:e(qlc:q([Profile||Profile<-mnesia:table(profile)]))
  end,
  {atomic,Profiles}=mnesia:transaction(F),
  ProfileOptions=[#option{text=element(3,X), value=integer_to_list(element(2,X)), selected=element(5,Data)=:=integer_to_list(element(2,X))}||X<-Profiles],

%  [Data|_]=get_data(list_to_integer(Id)),
%  io:format("PostData:~w",[Data]),
    ProfileData=#table { class=tiny, rows=[
    #tablerow { cells=[
      #tablecell { body=#label { text="Folder Id" } },
      #tablecell { body=#textbox { text=element(2,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Input Folder" } },
      #tablecell { body=#textbox { text=element(3,Data), id=pInfolder} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Output Folder" } },
      #tablecell { body=#textbox { text=element(4,Data), id=pOutfolder} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile" } },
%      #tablecell { body=#textbox { text=element(5,Data), id=pProfile} }
      #tablecell { body=#dropdown { options=ProfileOptions, id=pProfile} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="File Extension Filter" } },
      #tablecell { body=#textbox {  text=element(6,Data), id=pFilter} }
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
  [Pid|_] = wf:q(pId),
  if Pid == "-1"->
    NewPid=libdb:sequence(watchfolder);
    true->
      NewPid=list_to_integer(Pid)
        end,

  [In|_]=wf:q(pInfolder),
  [Out|_]=wf:q(pOutfolder),
  [Pro|_]=wf:q(pProfile),
  [Fil|_]=wf:q(pFilter),
  Folder=#watchfolder{
    id=NewPid,
    infolder=In,
    outfolder=Out,
    profile=Pro,
    filter=Fil
  },
   {atomic, ok} =mnesia:transaction(fun() ->mnesia:write(Folder)end),
%  io:format("Message:saveed for id ~w",[Pid]),
  wf:flash("Watch Folder Data saved."),
ok;
event(cancel) ->
    wf:redirect("/web/folder").
