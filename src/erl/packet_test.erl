-module(packet_test).


-export([packetstream/0, mysum/1]).


packetstream()->
  case gen_server:call({global,packet_sender}, {packetstream,"/media/video/ChocolateFactory.ts",-1,-1,1000  })of
    hivetimeout->
      hivetimeout;
    Any->
      case get(streamdata) of
        undefined->
          put(streamdata,[]);
        _->ok
        end,
        [Required|_]=[[X||X<-Any,element(1,X)<2]],
      put(streamdata,get(streamdata)++Required),
%      Size=mysum(Any),
%      io:format("~wM",[mysum(Size)/1024/1024]),
%      [VideoData|_]=[[X||X<-Any,element(1,X)==0]],
%      [AudioData|_]=[[X||X<-Any,element(1,X)==1]],
%      put(video,get(video)++VideoData),
%      put(audio,get(audio)++AudioData),
%      {VideoData, AudioData}
%      io:format("~w",[VideoData])
%  if length(get(streamdata)) > 0 ->
  {Data, Data2}=process(get(streamdata),1),
  packetstream()
%  end
end.

process([], _Stream)->{nodata,nodata};
process(List, Stream)->
  lists:partition(fun(A) -> is_tuple(A),element(1,A)==Stream end, List).


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

