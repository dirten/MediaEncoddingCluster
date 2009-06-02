%%%----------------------------------------------------------------------
%%% File    : mhive_.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Main Start and Stop Module
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
-module(mhive).
-behaviour(application).
-export([start/2, stop/1, startapp/0, stopapp/0, configure/0]).
-include("schema.hrl").

%-=====================================================================-
%-                                Public                               -
%-=====================================================================-

%% @spec startapp () -> ok
%% @doc starting up the main Application called mhive.
%% @end

startapp()->
  Node=libnet:local_name(),
  Self=libnet:local_name("starter"),
  net_kernel:start([Self]),
  S=rpc:call(Node,application,start,[sasl]),
  io:format("Startup ~p",[S]),
  M=rpc:call(Node,application,start,[mhive]),
  io:format("Startup ~p",[M]).

%% @spec stopapp () -> ok
%% @doc stopping up the main Application called mhive.
%% @end
stopapp()->
  Node=libnet:local_name(),
  Self=libnet:local_name("stopper"),
  io:format("Using Local Name ~p~n",[Node]),
  net_kernel:start([Self]),
  R=rpc:call(Node,init,stop,[]),
  io:format("Shutdown ~p",[R]).

%% @spec configure() -> ok
%% @doc configuration of the mhive Application,
%% configuration option are the mode and the autostart.
%% @end
configure()->
  setup:setup().
%  init:stop().

%% @spec start() -> ok
%% @doc this function will be called from application:start(mhive) function
%% @end
start(_Type, StartArgs)->
  Node=libnet:local_name(),
  io:format("Self node name = ~p~n",[Node]),
  net_kernel:start([Node]),
  %  net_adm:world(),
  %  application:set_env(mhive,wwwroot,filename:join(libcode:get_privdir(),"wwwroot")),
  %% TODO libcode wieder einbinden
  %%application:set_env(mhive,wwwroot,"wwwroot"),

  case config:get(mode) of
    server->
      mnesia:start(),
      mnesia:wait_for_tables([config, scheduler],5000),
      mhive_supervisor:start_link(StartArgs);
    client->
      client_supervisor:start_link(StartArgs);
    both->
      mnesia:start(),
      mnesia:wait_for_tables([config, scheduler],5000),
      mhive_supervisor:start_link(StartArgs),
      client_supervisor:start_link(StartArgs);
    _->system_not_configured
  end.

%% @spec stop() -> ok
%% @doc this function will be called from application:stop(mhive) function
%% @end

stop(_State)->
  ok.
