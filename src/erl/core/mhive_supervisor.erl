%%%----------------------------------------------------------------------
%%% File    : mhive_supervisor.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Start and Stop the Master Supervisor
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
-module(mhive_supervisor).
-behaviour(supervisor).
-export([start/0,start_in_the_shell/0, start_link/1,init/1]).


start()->
  spawn(fun() ->supervisor:start_link({local,?MODULE},?MODULE, _Arg=[])end).

start_in_the_shell()->
  {ok,Pid}=supervisor:start_link({local,?MODULE},?MODULE, _Arg=[]),
  unlink(Pid).

start_link(Args)->
  supervisor:start_link({local,?MODULE},?MODULE, Args).

init(_Args)->
  {ok,
   {
     {one_for_one,3,10},
     [
     {scheduler,{scheduler,start_link,[]},  permanent,  1000,  worker,  [scheduler]},
     {file_port, {file_port,start_link,[]},  permanent,  1000,  worker,  [file_port]},
     {sys_port, {sys_port,start_link,[]},  permanent,  1000,  worker,  [sys_port]},
     {packet_server, {packet_server,start_link,[]},  permanent,  1000,  worker,  [packet_server]},
     {node_watcher, {node_watcher,start_link,[]},  permanent,  1000,  worker,  [node_watcher]},
     {mhive_webserver,{mhive_webserver,start_link,[]},  permanent,  1000,  worker,  [mhive_webserver]}
     ]
   }
  }.

