-module(packet_stack).


-export([packetstream/2]).


packetstream(Filename, Offset)->
  case get(streamdata) of
    undefined->
      put(streamdata,[]);
    _->ok        
  end,
  LastFilename=get(lastfilename),
  C=length(get(streamdata)),
  if
    LastFilename =:=Filename orelse LastFilename=:=undefined->
      if C<350 ->
          case gen_server:call({global,packet_sender}, {packetstream,Filename,-1,Offset,500  })of
            hivetimeout->
              io:format("hivetimeout",[]),
              hivetimeout;
            Any->
%              io:format("Data:~p",[Any]),
              [Required|_]=[[X||X<-Any,element(1,X)<2]],
              put(streamdata,get(streamdata)++Required)
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

  Count=length(process(get(streamdata),1)),
  if Count> 300->
      Data=process(get(streamdata),1),
      put(streamdata,get(streamdata)--Data),
      Data;
    true->
      Data=process(get(streamdata),0),
      packet_group(Data,0)
  end.


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
