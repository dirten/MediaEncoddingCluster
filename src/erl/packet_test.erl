-module(packet_test).


-export([packetstream/1, mysum/1]).


packetstream(Filename)->
  case get(streamdata) of
    undefined->
      put(streamdata,[]);
    _->ok
        
  end,
  C=length(get(streamdata)),
  if C<50 ->
      case gen_server:call({global,packet_sender}, {packetstream,Filename,-1,-1,500  })of
        hivetimeout->
          hivetimeout;
        Any->
          [Required|_]=[[X||X<-Any,element(1,X)<1]],
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
  Data=process(get(streamdata),0),
  packet_group(Data,0).
%  packetstream()
%end


process([], _Stream)->[];
process(List, Stream)->
  {Data,_}=lists:partition(fun(A) -> is_tuple(A),element(1,A)==Stream end, List),
  Data.
packet_group([],C)->[];
packet_group([H|T], C)->
  % io:format("~w~n~n~n",[H]),
  [PG|_]=[[X||X<-[H|T],filter(X)]],
  put(counter,0),
  put(state,keyframeend),
  put(streamdata,get(streamdata)--PG),
  %  put(counter,0),
  [[A1,A2,A3|_]|_]=[[X||X<-get(streamdata), element(1,X)==0]],
  lists:flatten([PG,A1,A2,A3]).


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
%  false.
%[Data|_]=[[lists:nth(element(1,X)+1,Acc)||X<-PacketList]],
%  Data.
%process()
getunique([], Acc)->
  [];
getunique([H|T], Acc)->
  %  Acc:set(element(1,H), H),
  %  [Acc:get(element(1,H))]++getunique(T).
  [element(1,H)]++getunique(T).

getunique(Data)->
  getunique(Data, array:new(10)).

mysum([])->0;
mysum([H|T])->H+mysum(T).

