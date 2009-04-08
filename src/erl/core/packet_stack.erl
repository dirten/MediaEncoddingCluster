-module(packet_stack).
-include("./schema.hrl").
-include_lib("stdlib/include/qlc.hrl").


-export([packetstream/4]).

-record(streams, {index,packetcount,packets=[]}).
-record(processgroup,{id, packets}).

-record(state,{status=none, count=0}).

%% returns [Packet] | []
packetstream(Filename, Offset, DecoderList,EncoderList)->
  case get(streamdata) of
    undefined->
      put(streamdata,[]);
    _->ok
  end,
  case get(processgroups) of
    undefined->
      put(processgroups,[]);
    _->ok
  end,
  case ets:info(prozessgrouplist) of
    undefined->
      ets:new(prozessgrouplist,[named_table, ordered_set]);
    _->ok
  end,
  case get(streams) of
    undefined->
      put(streams,[#streams{index=1},#streams{index=2},#streams{index=3},#streams{index=4},#streams{index=5},#streams{index=6},#streams{index=7},#streams{index=8}]);
    _->ok
  end,
  LastFilename=get(lastfilename),
  C=ets:info(prozessgrouplist, size),
  [Decoder|_]=[X||X<-DecoderList,X#stream.streamidx==0],
  [Encoder|_]=[X||X<-EncoderList,X#stream.streamidx==0],

  if
    LastFilename =:=Filename orelse LastFilename=:=undefined->
      if C<5 ->
          io:format("Loading Packets~n",[]),
          case gen_server:call({global,packet_sender}, {packetstream,Filename,-1,Offset,1000  },infinity)of
            hivetimeout->
              io:format("hivetimeout",[]),
              hivetimeout;
            {no_more_packets}->
              io:format("No more Packets from File ~p",[Filename]);
            %{StreamIndex, KeyFrame, Pts, Dts, Flags, Duration, Size, Data}
            Any->
              %% currently there will be only the firtst 2 streams processed
              %              Streams=get(streams),
              [Required|_]=[[X||X<-Any,element(1,X)<2]],
              NewStreams=process_packet_list(Required,[#streams{index=1},#streams{index=2},#streams{index=3},#streams{index=4},#streams{index=5},#streams{index=6},#streams{index=7},#streams{index=8}]),
              build_process_groups(NewStreams, Decoder)
              %              put(streams,NewStreams),
              %
              %              put(streamdata,get(streamdata)++Required)
          end;
        true->ok
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
      io:format("Delete from ets Key:~p",[Key]),
      ets:delete(prozessgrouplist,Key),
      element(2,Tmp);
    _->[]
  end.
  %[Tmp|_]=qlc:next_answers(qlc:cursor(qlc:q([DataTmp||DataTmp<-ets:table(prozessgrouplist)])),1),
  %  Count=length(process(get(streamdata),1)),
  %  if Count> 300->
  %      Data=process(get(streamdata),1),
  %      put(streamdata,get(streamdata)--Data),
  %      Data;
  %    true->
  %      Data=process(get(streamdata),0),
  %      case packet_group(Data,0, Decoder, Encoder) of
  %        not_enough_video_data->
  %          AudioData=process(get(streamdata),1),
  %          put(streamdata,get(streamdata)--AudioData),
  %          AudioData;
  %        VideoData->
  %          VideoData
  %      end
  %  end.

  build_process_groups([], _Dec)->ok;
  build_process_groups([Stream|T], Dec)->
    if Stream#streams.index =:= 1->
        build_video_groups(Stream,Dec);
      true->
        build_audio_groups(Stream)
    end,
    build_process_groups(T, Dec).

build_audio_groups(Stream)->
  Packets=Stream#streams.packets,
  process_audio_packet(Packets,100,[]).

build_video_groups(Stream, Dec)->
  Packets=Stream#streams.packets,
  process_video_packet(Packets,#state{},[], Dec).



process_video_packet([],_State,PG,_Dec)->PG;
process_video_packet([Packet|T],State,PG, Dec)->
  case State#state.status of
    none->
      %% When Packet is KeyFrame
      if element(2,Packet)==1-> 
          NewState=State#state{status=group, count=1},
          NewPG=lists:append(PG,[Packet]);
        %% do nothing while find the first Keyframe
        true->
          NewState=State,%#state{status=group, count=1},
          NewPG=lists:append([[], [Packet]])
      end;
    group->
      if element(2,Packet)==1->
          NewState=State#state{status=none, count=0},
          if length(T)> 2 andalso Dec#stream.codec=:=2->
              io:format("Length:~p Decoder:~p",[length(T), Dec#stream.codec]),
              [A1, A2|_]=T,
              %io:format("A1:~p,A2:~p,A3:~p",[A1,A2,A3]),
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
  io:format("No More Packets~n"),
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


process([], _Stream)->[];
process(List, Stream)->
  {Data,_}=lists:partition(fun(A) -> element(1,A)==Stream end, List),
  Data.

packet_group([],_C, _Dec, _Enc)->[];
packet_group(Data, _C, Dec, _Enc)->
  % io:format("~w~n~n~n",[H]),
  PG=[X||X<-Data,filter(X)],
  put(counter,0),
  case get(state)of
    keyframeend->
      Result=Data--PG,
      put(streamdata,get(streamdata)--PG),
      %  put(counter,0),
      if length(Result)> 2 andalso Dec#stream.codec=:=2->
          [[A1,A2,A3|_]|_]=[[X||X<-Result]],
          lists:flatten([PG, A1, A2, A3]);
        true->lists:flatten([PG])
      end;
    _->
      put(state,keyframeend),
      not_enough_video_data
  end.
%  PG.


filter(El)->
  C=get(counter),
  S=get(state),
  if
    S==keyframeend,C==0->
      if
        element(2,El)==1->
          put(state,keyframestart),
          put(counter,C+1),
          true;
        true->true
      end;
    S==keyframestart,C>0->
      %    Counter=get(counter),
      if
        element(2,El)==1->
          put(state,keyframeend),
          false;
        true->
          put(counter,C+1),
          true
      end;
    %    C>11->false;
    true->false
  end.
