-module(web_media_detail).

-include ("wf.inc").
%-include ("schema_watchfolder.hrl").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).

main() ->
    {ok,Val}=application:get_env(wwwroot),
    Temp=string:concat(Val,"/onecolumn.html"),
    #template { file=Temp, bindings=[
	{'Group', media},
	{'Item', media}
]}.

title() -> "Media Detail".

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
            Transform(element(6,E)),
            Transform(element(7,E)),
            Transform(element(8,E)),
            Transform(element(9,E)),
            Transform(element(10,E)),
            Transform(element(11,E))
                     } || E <- mnesia:table(file), element(2,E)=:=Id]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
  E.

body()->
  [Id|_]=wf:q(id),
%  io:format("PostId:~w",[Id]),
  G=list_to_integer(Id),
  if
    G > 0->
      [Data|_]=get_data(list_to_integer(Id));
    true->
      Data={"","","","","","",""}
  end,
ProfileData=#table { class=tiny, rows=[
    #tablerow { cells=[
      #tablecell { body=#label { text="File Id: " } },
      #tablecell { body=#label { text=element(2,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="File Path: " } },
      #tablecell { body=#label { text=element(4,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="File Name: " } },
      #tablecell { body=#label { text=element(3,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="File Size: " } },
      #tablecell { body=#label { text=element(5,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="File Format: " } },
      #tablecell { body=#label { text=element(6,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Stream Count: " } },
      #tablecell { body=#label { text=element(7,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Duration: " } },
      #tablecell { body=#label { text=element(8,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Bitrate: " } },
      #tablecell { body=#label { text=element(9,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Type: " } },
      #tablecell { body=#label { text=element(10,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Parent: " } },
      #tablecell { body=#label { text=element(11,Data), id=pId} }
    ]}
  ]},[
  #h3 { text=title() },
      #hr{},
    ProfileData,
    #hr{}].

