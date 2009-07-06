%%%----------------------------------------------------------------------
%%% File    : libcode.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Internal Helper
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
-module(libcode).
-include("version.hrl").
%-include("config.hrl").
-compile(export_all).

get_mhive_version()->
    ?VERSION.

get_mhivesys_exe()->
    BinName=
        case os:type() of
            {win32,nt} ->
            "mhivesys.exe";
            {unix, _}->
            "mhivesys"
        end,
    case code:priv_dir(mhive) of
        Path when is_list(Path) ->filename:join([Path,BinName]);
        {error, bad_name}->"/usr/bin/valgrind --log-file=/tmp/mhivesys  --tool=memcheck --leak-check=full --show-reachable=yes "++filename:join(["bin",BinName])
    %    {error, bad_name}->filename:join(["bin",BinName])
    end.
    
get_privdir()->
    case code:priv_dir(mhive) of
        Path when is_list(Path) ->Path;
        {error, bad_name}->"."
    end.

epmd_started()->
    case erl_epmd:open() of
        {error,econnrefused}->false;
        _->true
    end.

epmd_start()->
    [ErtsPath|_]=filelib:wildcard(code:root_dir()++"/erts*"),
    case os:find_executable("epmd", ErtsPath++"/bin") of
        false->{error,"could not find epmd"};
        EpmdPath->
            os:cmd(EpmdPath++" -daemon"),
            ok
    end.
