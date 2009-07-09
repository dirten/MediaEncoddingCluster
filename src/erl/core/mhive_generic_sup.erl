%%%----------------------------------------------------------------------
%%% File    : mhive_generic_sup.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Generich Supervisor to start with server and client
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
-module(mhive_generic_sup).
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
         {node_finder,{node_finder,start_link,[]},  permanent,  1000,  worker,  [node_finder]}
         ]
     }
    }.

