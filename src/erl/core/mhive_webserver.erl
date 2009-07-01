%%%----------------------------------------------------------------------
%%% File    : mhive_webserver.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Start and Stop the internal Webserver for the JSON API
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
-module(mhive_webserver).
-export([start_link/0,terminate/2, init/1,code_change/3,handle_call/3,handle_cast/2,handle_info/2]).
-behaviour(gen_server).

start_link()->
  gen_server:start_link({local,?MODULE},?MODULE,[],[]).

init([])->
  process_flag(trap_exit, true),
  inets:start(),
  _Old=[{file,config:get(wwwroot)++"/conf/8080.conf"}],
  filelib:ensure_dir(getConfig(error_log)),
  Conf=[
    {port,getConfig(port)},
    {server_name,getConfig(server_name)},
    {server_root,getConfig(server_root)},
    {document_root,getConfig(document_root)},
    {modules,[mod_alias, mod_auth, mod_esi, mod_actions, mod_cgi, mod_dir, mod_get, mod_head, mod_log, mod_disk_log]},
    {error_log,getConfig(error_log)},
    {transfer_log,getConfig(transfer_log)},
    {security_log,getConfig(security_log)},
    {erl_script_alias, {"/api", [json]}},
    {directory_index, ["index.hml", "mec.html"]}
       ],
  inets:start(httpd,Conf),
  io:format("~p started~n", [?MODULE]),
  {ok, state}.


handle_call(Any,_From,_N)->
  {reply, Any, state}.

handle_cast(_Msg,N)->
  io:format("~w handle_cast~w~n", [?MODULE,N]),
  {noreply, N}.

handle_info(Info,N)->
  io:format("~p handle_info~w~n", [?MODULE,{Info,N}]),
  {noreply, N}.

terminate(_Reason,_N)->
  io:format("~w shutdown ~n", [?MODULE]),
  inets:stop(),
  ok.

code_change(_OldVsn,N,_Extra)->
  io:format("~w CodeChange ~n", [?MODULE]),
  {ok, N}.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Internal function getWebConfig
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


getConfig(port)->
  8080;
getConfig(server_root)->
  libcode:get_privdir()++"/wwwroot";
getConfig(document_root)->
  libcode:get_privdir()++"/wwwroot/htdocs";
getConfig(server_name)->
  "mhive.net";
getConfig(error_log)->
  libcode:get_privdir()++"/logs/error_log";
getConfig(transfer_log)->
  libcode:get_privdir()++"/logs/access_log";
getConfig(security_log)->
  libcode:get_privdir()++"/logs/security_log";
getConfig([])->
  "".
