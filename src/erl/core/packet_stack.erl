-module(packet_stack).
-include("./schema.hrl").
-include_lib("stdlib/include/qlc.hrl").


-export([packetstream/4]).

-record(streams, {index,packetcount,packets=[]}).
%-record(processgroup,{id, packets}).

-record(state,{status=none, count=0}).

%% returns [Packet] | []
packetstream(Filename, Offset, DecoderList,_EncoderList)->
  case ets:info(prozessgrouplist) of
    undefined->
      ets:new(prozessgrouplist,[named_table, ordered_set]);
    _->ok
  end,
  case ets:info(rest) of
    undefined->
      ets:new(rest,[named_table, ordered_set]);
    _->ok
  end,
  LastFilename=get(lastfilename),
  C=ets:info(prozessgrouplist, size),
  if
    LastFilename =:=Filename orelse LastFilename=:=undefined->
      if C<5 ->
          io:format("Loading Packets~n",[]),
          case gen_server:call({global,packet_sender}, {packetstream,Filename,-1,Offset,500  },infinity)of
            hivetimeout->
              io:format("hivetimeout",[]),
              hivetimeout;
            {no_more_packets}->
              io:format("No more Packets from File ~p",[Filename]);
            [eof]->
              io:format("No more Packets from File ~p",[Filename]),

              [ets:insert(prozessgrouplist,{libdb:sequence(prozessgrouplist),Data})||Data<-qlc:e(qlc:q([D||{_Id,D}<-ets:table(rest)])), length(Data)>0],
              ets:delete_all_objects(rest);
              %            ets:insert(prozessgrouplist,{libdb:sequence(prozessgrouplist),D})
            %{StreamIndex, KeyFrame, Pts, Dts, Flags, Duration, Size, Data}
            Any->
              %% currently there will be only the firtst 2 streams processed
              [Required|_]=[[X||X<-Any,element(1,X)<2]],
              NewStreams=process_packet_list(Required,[#streams{index=1},#streams{index=2},#streams{index=3},#streams{index=4},#streams{index=5},#streams{index=6},#streams{index=7},#streams{index=8}]),
              [Decoder|_]=[X||X<-DecoderList,X#stream.streamidx==0],
              %              [Encoder|_]=[X||X<-EncoderList,X#stream.streamidx==0],
              build_process_groups(NewStreams, Decoder)
          end;
        true->do_nothing
      end;
    true->
      if C ==0->
          put(lastfilename, Filename);
        true->
          do_nothing
      end
  end,
  Key=ets:first(prozessgrouplist),
  case ets:lookup(prozessgrouplist, Key) of
    [Tmp|_]->
      ets:delete(prozessgrouplist,Key),
      element(2,Tmp);
    _->
      []
  end.

build_process_groups([], _Dec)->ok;
build_process_groups([Stream|T], Dec)->
  PG=case ets:lookup(rest, Stream#streams.index) of
       [Tmp|_]->
         ets:delete(rest,Stream#streams.index),
         element(2, Tmp);
       _->[]
     end,
  if Stream#streams.index =:= 1->
      Rest=build_video_groups(Stream,Dec,PG);
    true->
      Rest=build_audio_groups(Stream, PG)
  end,
  ets:insert(rest,{Stream#streams.index,Rest}),
  build_process_groups(T, Dec).

build_audio_groups(Stream, PG)->
  Packets=Stream#streams.packets,
  process_audio_packet(Packets,200,PG).

build_video_groups(Stream, Dec, PG)->
  Packets=Stream#streams.packets,
  process_video_packet(Packets,#state{},PG, Dec).



process_video_packet([],_State,PG,_Dec)->PG;
process_video_packet([Packet|T],State,PG, Dec)->
  case State#state.status of
    none->
      %% When Packet is KeyFrame
      if element(2,Packet)==1-> 
          NewState=State#state{status=group, count=1},
          NewPG=lists:append(PG,[Packet]);
        true->
          NewState=State,%#state{status=group, count=1},
          NewPG=lists:append([PG, [Packet]])
      end;
    group->
      if element(2,Packet)==1->
          NewState=State#state{status=none, count=0},
          if length(T)> 2 andalso Dec#stream.codec=:=2->
              [A1, A2|_]=T,
              DbPG=lists:append([PG, [Packet,A1, A2]]);
            true->
              DbPG=PG
          end,
          ets:insert(prozessgrouplist,{libdb:sequence(prozessgrouplist),DbPG}),
          NewPG=lists:append([[], [Packet]]);
        true->
          NewState=State,
          NewPG=lists:append(PG,[Packet])
      end
  end,
  process_video_packet(T,NewState,NewPG, Dec).


process_audio_packet([],_Count,PG)->PG;
process_audio_packet([Packet|T],Count,PG)->
  if length(PG)<Count->
      NewPG=lists:append(PG,[Packet]),
      process_audio_packet(T,Count,NewPG);
    true->
      NewPG=lists:append(PG,[Packet]),
      ets:insert(prozessgrouplist,{libdb:sequence(prozessgrouplist),NewPG}),
      process_audio_packet(T,Count,[])
  end.

process_packet_list([],List)->
  List;
process_packet_list([Packet|Tail], List) when is_tuple(Packet)->
  Index=element(1,Packet)+1,
  Stream=lists:nth(Index,List),
  NewStream=Stream#streams{packets=lists:append(Stream#streams.packets,[Packet])},
  NewList=lists:keyreplace(NewStream#streams.index,#streams.index,List,NewStream),
  process_packet_list(Tail, NewList);
process_packet_list([Packet|Tail], List)->
  io:format("Packet is not a Tuple:~p~n",[Packet]),
  process_packet_list(Tail,List).

