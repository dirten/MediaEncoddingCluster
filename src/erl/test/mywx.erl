%%
%% %CopyrightBegin%
%%
%% Copyright Ericsson AB 2009. All Rights Reserved.
%%
%% The contents of this file are subject to the Erlang Public License,
%% Version 1.1, (the "License"); you may not use this file except in
%% compliance with the License. You should have received a copy of the
%% Erlang Public License along with this software. If not, it can be
%% retrieved online at http://www.erlang.org/.
%%
%% Software distributed under the License is distributed on an "AS IS"
%% basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
%% the License for the specific language governing rights and limitations
%% under the License.
%%
%% %CopyrightEnd%
%%
%%%-------------------------------------------------------------------
%%% File    : hello.erl
%%% Author  : Matthew Harrison <harryhuk at users.sourceforge.net>
%%% Description : _really_ minimal example of a wxerlang app
%%%
%%% Created :  18 Sep 2008 by  Matthew Harrison <harryhuk at users.sourceforge.net>
%%%-------------------------------------------------------------------
-module(mywx).

-include_lib("wx/include/wx.hrl").

-export([start/0]).
-compile(export_all).

start() ->
    Wx = wx:new(),
    Frame = wx:batch(fun() -> create_window(Wx) end),
    wxWindow:show(Frame),
    loop(Frame),
    wx:destroy(),
    ok.

create_window(Wx) ->
    Frame = wxFrame:new(Wx,
			-1, % window id
			"Hello World", % window title
			[{size, {600,400}}]),


    wxFrame:createStatusBar(Frame,[]),

    %% if we don't handle this ourselves, wxwidgets will close the window
    %% when the user clicks the frame's close button, but the event loop still runs
    wxFrame:connect(Frame, close_window),

    ok = wxFrame:setStatusText(Frame, "Hello World!",[]),
    Frame.

loop(Frame) ->
    receive
   	#wx{event=#wxClose{}} ->
   	    io:format("~p Closing window ~n",[self()]),
   	    ok = wxFrame:setStatusText(Frame, "Closing...",[]),
	    wxWindow:destroy(Frame),
	    ok;
	Msg ->
	    io:format("Got ~p ~n", [Msg]),
	    loop(Frame)
    end.

