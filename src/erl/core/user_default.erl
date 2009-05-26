%%%----------------------------------------------------------------------
%%% File    : user_default.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Start and Stop the Application and Console tools
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

-module(user_default).
-import(io, [format/1, format/2]).
-compile(export_all).

help()->
  format("***  internal Commands\n"),
  format("help()            --> print this help Message\n"),
  format("loglist()         --> Lists all Log Messages from Logging Service\n"),
  format("loglist(N)        --> Lists last <N> Log Messages from Logging Service\n"),
  format("logshow(N)        --> Show Log Messages with #  <N>\n"),
  format("connect(Host)     --> Connecting this Node to Host\n"),
  format("startserver()     --> Starting the Server on this Node\n"),
  format("stopserver()      --> Stopping the Server on this Node\n"),
  format("startclient(Host) --> Starting the Client on this Node and connect to Host\n"),
  format("stopclient()      --> Stopping the Client on this Node\n"),
  true.

reload()->
    application:stop(mhive),
  application:unload(mhive),
  devel:build("0.0.4.2"),
  application:start(mhive).

restart()->
  application:stop(mhive),
  application:unload(mhive),
  devel:build("0.0.4.2"),
  libdb:clear(),
  application:start(mhive).
  
logload()->
  rb:start().

loglist()->
  rb:list().

logshow()->
  rb:show().

logshow(Nr)->
  rb:show(Nr).

connect(Host)->
  connect(Host, "node").

connect(Host, NodeName)->
  case inet:getaddr(Host, inet) of
    {ok, Addr}->
      format("Connecting to Host Address ~w~n",[Addr]),
      {A1,A2,A3,A4}=Addr,
      Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
      Node=list_to_atom(NodeName++"@"++Ip),
      case net_adm:ping(Node)of
        pong->
          format("Visible Nodes ~p~n",[[node()|nodes()]]),
          {ok,Node};
        pang->
          format("No Services ~p found on Host with Address ~p ~p~n",[NodeName,Host,Node]),
          {ok,Data}=net_adm:names(Ip),
          Services=[element(1,X)||X<-Data],
          format("Available Services on Host Address ~p~n",[Services]),
          error
      end;
    {error, nxdomain}->
      format("Can not resolve Host Address for ~p~n",[Host]),
      error
  end.

startserver()->
  {ok,Data}=inet:getaddr(net_adm:localhost(), inet),
  {A1,A2,A3,A4}=Data,
  Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
  Node=list_to_atom("node@"++Ip),
  _Pid=spawn(Node,application,start,[mhive]).
%  application:start(mhive).

stopserver()->
  {ok,Data}=inet:getaddr(net_adm:localhost(), inet),
  {A1,A2,A3,A4}=Data,
  Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
  Node=list_to_atom("node@"++Ip),
  _Pid=spawn(Node,application,stop,[mhive]).

startclient(Host)->
  startclient(Host,"node").

startclient(Server, NodeName)->
  case connect(Server, NodeName) of
    {ok,_N}->
      {ok,Data}=inet:getaddr(net_adm:localhost(), inet),
      {A1,A2,A3,A4}=Data,
      Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
      Node=list_to_atom("node@"++Ip),
      _Pid=spawn(Node,application,start,[mhive_client]),
      %      rpc:call(Node, application,start,[mhive_client]),
      %      unlink(Pid),
      ok;
    error->
      error
  end,
  ok.
startclient()->
  {ok,Data}=inet:getaddr(net_adm:localhost(), inet),
  {A1,A2,A3,A4}=Data,
  Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
  Node=list_to_atom("node@"++Ip),
  Pid=spawn(Node,application,start,[mhive_client]),
  %      rpc:call(Node, application,start,[mhive_client]),
  %      unlink(Pid),
  Pid.
stopclient()->
  {ok,Data}=inet:getaddr(net_adm:localhost(), inet),
  {A1,A2,A3,A4}=Data,
  Ip=integer_to_list(A1)++"."++integer_to_list(A2)++"."++integer_to_list(A3)++"."++integer_to_list(A4),
  Node=list_to_atom("node@"++Ip),
  _Pid=spawn(Node,application,stop,[mhive_client]).

