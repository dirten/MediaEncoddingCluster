%%%----------------------------------------------------------------------
%%% File    : libnet.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Network and Distributed Helper
%%% Created : 18 Feb 2009 by Jan Hölscher <jan.hoelscher@esblab.com>
%%%
%%%
%%% MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
%%%
%%% This program is free software; you can redistribute it and/or
%%% modify it under the terms of the GNU General Public License as
%%% published by the Free Software Foundation; either version 2 of the
%%% License, or (at your option) any later version.
%%%
%%% This program is distributed in the hope that it will be useful,
%%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%%% General Public License for more details.
%%%
%%% You should have received a copy of the GNU General Public License
%%% along with this program; if not, write to the Free Software
%%% Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
%%% 02111-1307 USA
%%%
%%%----------------------------------------------------------------------
-module(libnet).
-compile(export_all).

stop_network()->
  net_kernel:stop().

start_network()->
  case libcode:epmd_started() of
    false->
      case libcode:epmd_start() of
        ok->error_logger:info_msg("epmd started");
        {error, Msg}->error_logger:error_report(Msg)
      end;
    _->ok
  end,
  Node=libnet:local_name(),
  case net_kernel:start([Node,shortnames]) of
    {ok, _Pid}->io:format("Network Kernel started~n");
    {error,{already_started,Pid}}->io:format("Network allready started on PID ~w~n",[Pid])
  end,
  inet_db:set_lookup([native, file]).

get_host_name(Name)->
  Start = string:rchr(Name, $@),
  lists:sublist(Name, Start+1,length(Name)).
get_node_name(Name)->
  Start = string:rchr(Name, $@),
  lists:sublist(Name, Start-1).


local_name()->
  local_name("node").

local_name(Name)->
  {ok,HostName}=inet:gethostname(),
  list_to_atom(Name++"@"++HostName).
  
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

          io:format("No Services ~p found on Host with Address ~p ~p~n",[NodeName,Host,Node]),
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
