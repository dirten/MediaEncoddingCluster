-module(libnet).
-compile(export_all).


local_name()->
  local_name("node").
local_name(Name)->
   {ok,Data}=inet:getaddr(net_adm:localhost(), inet),
  {A1,A2,A3,A4}=Data,
  Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
  Node=list_to_atom(Name++"@"++Ip),
  Node.
  
connect(Host, NodeName)->
  case inet:getaddr(Host, inet) of
    {ok, Addr}->
      io:format("Connecting to Host Address ~w~n",[Addr]),
      {A1,A2,A3,A4}=Addr,
      Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
      Node=list_to_atom(NodeName++"@"++Ip),
      case net_adm:ping(Node)of
        pong->
          io:format("Visible Nodes ~p~n",[[node()|nodes()]]),
          {ok,Node};
        pang->

%          io:format("No Services ~p found on Host with Address ~p ~p~n",[NodeName,Host,Node]),
%          {ok,Data}=net_adm:names(Ip),
%          Services=[element(1,X)||X<-Data],
%          io:format("Available Services on Host Address ~p~n",[Services]),
          {error,"no pssible Service found"}
       end;
    {error, nxdomain}->
%      wf:flash("Can not resolve Host Address"),
%      io:format("Can not resolve Host Address for ~p~n",[Host]),
    {error,"Can not resolve Host Address"}
    end.
