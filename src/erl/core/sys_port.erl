%%%----------------------------------------------------------------------
%%% File    : mhive_webserver.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Start and Stop the internal System Information Port
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

-include("../include/config.hrl").
-module(sys_port).
-export([start/1, start_link/0,stop/0, init/1, handle_call/3, handle_cast/2, handle_info/2, code_change/3, terminate/2, get_codec_list/0, get_format_list/0]).
-behaviour(gen_server).



get_codec_list()->
  gen_server:call({global,hive_system_info}, {codeclist}).

get_format_list()->
  gen_server:call({global,hive_system_info}, {formatlist}).

start(Dir)->
  spawn(?MODULE,init,[Dir]),
  ok.

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

stop()->
  global:whereis_name(hive_system_info) ! stop,
  ok.

init([])->
  BinPath=libcode:get_mhivesys_exe(),
  io:format("Starting ~s ~s ~n", [?MODULE, BinPath]),
  global:register_name(hive_system_info, self()),
  process_flag(trap_exit, true),
  Port = open_port({spawn, BinPath}, [{packet, 4}, binary]),
  link(Port),
  register(mhive_sys_port,Port),
  io:format("~s started~n", [?MODULE]),
  {ok, state}.

handle_call({Command},_From,_N)->
  mhive_sys_port ! {self(), {command, term_to_binary({Command})}},
  receive
    {_Fileport, {data, Data}} ->
      D=binary_to_term(Data),
      {reply, D, state}
  after 4000 ->
      io:format("System Port Timeout~n",[]),
      {reply, sysport_timeout, state}
  end.



handle_cast(_Msg,N)->
  io:format("handle_cast(Msg,N)~n", []),
  {noreply, N}.

handle_info(Info,N)->
  case Info of
    {'EXIT', _Fileport, Reason2} ->
      io:format("Port exited  ~w~n", [Reason2]),
      exit({normal, Reason2});
    _->
      io:format("handle_info(,N)~w~n", [Info])
  end,
  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~p terminate~n",[?MODULE]),
  ok.

code_change(_OldVsn,N,_Extra)->{ok, N}.

