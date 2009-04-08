-module(web_profile_edit).

-include ("wf.inc").
-include ("schema_profile.hrl").
-include_lib("stdlib/include/qlc.hrl").
-compile(export_all).


main() ->
    {ok,Val}=application:get_env(wwwroot),
    Temp=string:concat(Val,"/onecolumn.html"),
    #template { file=Temp, bindings=[
	{'Group', media},
	{'Item', profile}
]}.
title() -> "Profile Edit".

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
            Transform(element(15,E)),
            Transform(element(16,E))
                     } || E <- mnesia:table(profile), element(2,E)=:=Id]),
          qlc:e(Q)
      end,
  {atomic,E}=mnesia:transaction(F),
  %  io:format("Data:~w",[E]),
  E.

body()->


  [Id|_]=wf:q(id),
  G=list_to_integer(Id),
  if
    G > 0->
      [Data|_]=get_data(G);
    G<0->
%      Data={"","-1","","","","","","","","","","","","",""}
      Data=#profile{}
  end,
  case gen_server:call({global,hive_system_info}, {codeclist})of
    AnyCodec->
      VideoCodecOptions=[#option{text=element(1,X), value=integer_to_list(element(2,X)),selected=element(6,Data)==integer_to_list(element(2,X))}||X<-AnyCodec,element(3,X)=:=0,element(4,X)=:=1],
      AudioCodecOptions=[#option{text=element(1,X), value=integer_to_list(element(2,X)),selected=element(11,Data)==integer_to_list(element(2,X))}||X<-AnyCodec,element(3,X)=:=1,element(4,X)=:=1]
%      io:format("Any:~w",[AnyCodec])
    end,
  case gen_server:call({global,hive_system_info}, {formatlist})of
    Any->
      FormatOptions=[#option{text=element(2,X), value=element(1,X), selected=element(4,Data)==element(1,X)}||X<-Any]
%      io:format("Any:~w",[Options])
    end,
%  io:format("Data from ~w: ~w",[Id,Data]),
  ProfileData=#table { class=tiny, rows=[
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Id" } },
      #tablecell { body=#value { text=element(2,Data)} },
      #tablecell { body=#hidden { text=element(2,Data), id=pId} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Name" } },
      #tablecell { body=#textbox { text=element(3,Data), id=pName} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile File Extension" } },
      #tablecell { body=#textbox { text=element(5,Data), id=pExt} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile File Format" } },
      #tablecell { body=#dropdown { options=FormatOptions, id=pFormat} }
    ]}
  ]},
  VideoData=#table { class=tiny, rows=[
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Video Codec" } },
      #tablecell { body=#dropdown { options=VideoCodecOptions, id=pVcodec} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Video Bitrate" } },
      #tablecell { body=#textbox { text=element(7,Data), id=pVbitrate} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Video Framerate" } },
      #tablecell { body=#textbox { text=element(8,Data),id=pVframerate} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Video Width" } },
      #tablecell { body=#textbox { text=element(9,Data),id=pVwidth} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Video Height" } },
      #tablecell { body=#textbox { text=element(10,Data),id=pVheight} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Group of Picture" } },
      #tablecell { body=#textbox { text=element(16,Data),id=pGop} }
    ]}
  ]},
  AudioData=#table { class=tiny, rows=[
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Audio Codec" } },
      #tablecell { body=#dropdown { options=AudioCodecOptions,id=pAcodec} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Audio Channels" } },
      #tablecell { body=#textbox { text=element(12,Data),id=pAchannels} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Audio Bitrate" } },
      #tablecell { body=#textbox { text=element(13,Data),id=pAbitrate} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="Profile Audio Samplerate" } },
      #tablecell { body=#textbox { text=element(14,Data),id=pAsamplerate} }
    ]},
    #tablerow { cells=[
      #tablecell { body=#label { text="2 Pass Encoding" } },
      #tablecell { body=#textbox { text=element(15,Data),id=pMultiPass} }
    ]}
  ]},

  Result=[
    #h3 { text=title() },
    #hr{},
    #table { class=tiny, rows=[
      #tablerow { cells=[
        #tablecell { valign=top,body=ProfileData },
        #tablecell { body=VideoData },
        #tablecell { valign=top,body=AudioData }
      ]}
    ]},
    #hr{},
    #button { class=tiny,id=saveProfile,text="Save Profile", postback=save },
    #button { class=tiny,id=cancel,text="Cancel", postback=cancel }
  ],
  Result.


event(save) ->
  [Pid|_] = wf:q(pId),
  if Pid == "-1"->
    NewPid=libdb:sequence(profile);
    true->
      NewPid=list_to_integer(Pid)
        end,
  [Pname] = wf:q(pName),
  [Pext] = wf:q(pExt),
  [Pformat] = wf:q(pFormat),
  [PVcodec] = wf:q(pVcodec),
  [PVbitrate] = wf:q(pVbitrate),
  [PVframerate] = wf:q(pVframerate),
  [PVwidth] = wf:q(pVwidth),
  [PVheight] = wf:q(pVheight),
  [PAcodec] = wf:q(pAcodec),
  [PAchannels] = wf:q(pAchannels),
  [PAbitrate] = wf:q(pAbitrate),
  [PAsamplerate] = wf:q(pAsamplerate),
  [PMultiPass] = wf:q(pMultiPass),
  [PGop] = wf:q(pGop),
  
Profile=#profile{
    id=NewPid,
    name=Pname,
    ext=Pext,
    vformat=Pformat,
    vcodec=list_to_integer(PVcodec),
    vbitrate=list_to_integer(PVbitrate),
    vframerate=list_to_integer(PVframerate),
    vwidth=list_to_integer(PVwidth),
    vheight=list_to_integer(PVheight),
    acodec=list_to_integer(PAcodec),
    achannels=list_to_integer(PAchannels),
    abitrate=list_to_integer(PAbitrate),
    asamplerate=list_to_integer(PAsamplerate),
    multipass=list_to_integer(PMultiPass),
    gop=list_to_integer(PGop)
    },
   {atomic, ok} =mnesia:transaction(fun() ->mnesia:write(Profile)end),
%  io:format("Message:saveed for id ~w",[Pid]),
  wf:flash("Profile Data saved."),
	ok;
event(cancel) ->
  wf:redirect("/web/profile"),
  io:format("Message:cancel",[]),
	ok;
event(_) ->
  io:format("Message:leer",[]),
  ok.