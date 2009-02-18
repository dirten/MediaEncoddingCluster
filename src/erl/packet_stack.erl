-module(packet_stack).


-export([packetstream/2]).


packetstream(Filename, Offset)->
  case get(streamdata) of
    undefined->
      put(streamdata,[]);
    _->ok
        
  end,
  C=length(get(streamdata)),
  if C<350 ->
      case gen_server:call({global,packet_sender}, {packetstream,Filename,-1,Offset,400  })of
        hivetimeout->
          hivetimeout;
        Any->
          [Required|_]=[[X||X<-Any,element(1,X)<2]],
          put(streamdata,get(streamdata)++Required)
      end;
    true->ok
  end,


  %      Size=mysum(Any),
  %      io:format("~wM",[mysum(Size)/1024/1024]),
  %      [VideoData|_]=[[X||X<-Any,element(1,X)==0]],
  %      [AudioData|_]=[[X||X<-Any,element(1,X)==1]],
  %      put(video,get(video)++VideoData),
  %      put(audio,get(audio)++AudioData),
  %      {VideoData, AudioData}
  %      io:format("~w",[VideoData])
  %  if length(get(streamdata)) > 0 ->
  Count=length(process(get(streamdata),1)),
  %    io:format("Videoooooooooooooooo Packet Count ~w~n",[length(process(get(streamdata),0))]),
  %    io:format("Audioooooooooooooooo Packet Count ~w~n",[length(process(get(streamdata),1))]),
  %    io:format("ALLLoooooooooooooooo Packet Count ~w~n",[length(get(streamdata))]),
  if Count> 300->
      Data=process(get(streamdata),1),
      put(streamdata,get(streamdata)--Data),
      %      io:format("Audioooooooooooooooo Packet",[]),
      Data;
    %      packet_group(Data,1);
    true->
      Data=process(get(streamdata),0),
      %      io:format("Videooooooooooooooooo PacketGroup ~w",[length(Data)]),
      packet_group(Data,0)
  end.

%  packetstream()
%end 3759782495


process([], _Stream)->[];
process(List, Stream)->
  {Data,_}=lists:partition(fun(A) -> element(1,A)==Stream end, List),
  Data.
packet_group([],_C)->[];
packet_group(Data, _C)->
  % io:format("~w~n~n~n",[H]),
  PG=[X||X<-Data,filter(X)],
  put(counter,0),
  put(state,keyframeend),
  Result=Data--PG,
  put(streamdata,get(streamdata)--PG),
  %  put(counter,0),
  if length(Result)> 2->
      [[A1,A2,A3|_]|_]=[[X||X<-Result]],
      lists:flatten([PG, A1, A2, A3]);
    true->lists:flatten([PG])
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
      if
        element(2,El)==1->
          put(state,keyframeend),
          %          put(counter,0),
          false;
        true->
          put(counter,C+1),
          true
      end;
    %    C>11->false;
    true->false
  end.
