%%%----------------------------------------------------------------------
%%% File    : setup.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Setting up the MHIVE
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
-module(setup).
-compile(export_all).
-include("schema.hrl").
-include("version.hrl").

%% @spec()->ok
setup()->
%  setup_db(),
    io:format("Configure Cluster Environment~n"),
    Mode=setup_mode(),
    Auto=setup_autostart(),

    case os:type() of
        {win32,nt} ->
            setup:setup_win32(list_to_atom(Mode),list_to_atom(Auto));
        {unix, _}->
            setup_linux(list_to_atom(Mode),list_to_atom(Auto))
    end.

setup_mode()->
    read_data("please define the Cluster Instance Mode {server | client | both}: ",[server,client,both]).

setup_autostart()->
    read_data("should Instance Autostart at System Boot {yes | no}: ",[yes,no]).

setup_webserver()->
    read_data("Webserver Port : ",[]).


setup_linux(Mode,Auto)->
    setup_db(),
    setup_config(mode,Mode),
    setup_auto_start(Auto),
    ok.

setup_win32(Mode, AutoStart)->
    [ErtsPath|_]=filelib:wildcard(code:root_dir()++"/erts*"),
    setup_db(),
    setup_config(mode,Mode),
    case AutoStart of
        yes->
            Node=libnet:local_name(),
            os:cmd(lists:concat([ErtsPath,"/bin/erlsrv remove MHiveService "])),
            os:cmd(lists:concat([ErtsPath,"/bin/erlsrv add MHiveService -w ",code:root_dir()," -name ",Node," -d new -args \"-setcookie default -config releases/"++libutil:toString(?VERSION)++"/sys -boot releases/"++libutil:toString(?VERSION)++"/start\""]));
        _->
            ok
    end.

setup_auto_start(AutoStart)->
    case AutoStart of
        yes->
            {ok,Base}=file:get_cwd(),
            _Res=libfile:symlink(filename:join(Base,"bin/mectl"),"/etc/rc.d/rc5.d/S75mectl"),
            _Res=libfile:symlink(filename:join(Base,"bin/mectl"),"/etc/rc.d/rc5.d/K03mectl");
        _->
            ok
    end,
    ok.

setup_db()->
    io:format("Stopping Database~n"),
    mnesia:stop(),
    Node=libnet:local_name(),
    io:format("Using Local Name ~p~n",[Node]),
    io:format("starting kernel"),
    libnet:start_network(),
    mnesia:create_schema([Node]),
    mnesia:start(),
    mnesia:wait_for_tables([config],1000),
    libdb:create(),
    mnesia:load_textfile(filename:join([libcode:get_privdir(),"default.data"])).


setup_config(Key, Val)->
    libdb:write(#config{key=Key, val=Val}).
%  {atomic, ok} =mnesia:transaction(fun() ->mnesia:write(#config{key=Key, val=Val})end).
%    mnesia:dirty_write(#config{key=Key, val=Val}).


read_data(Text,Values)->
    {ok, [Data|_]}=io:fread(Text,"~s"),
    if length(Values)>0->
            case lists:member(list_to_atom(string:to_lower(Data)),Values) of
                true->
                    Data;
                false->
                    print_failure(Data,[yes,no]),
                    read_data(Text, Values)
            end;
        true->Data
    end.

print_failure(Value, Valid)->
    io:format("~nWrong value given ~p, valid values are ~p~n",[Value, Valid]).

