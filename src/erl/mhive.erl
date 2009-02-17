-module(mhive).
-behaviour(application).
-export([start/2, stop/1, startapp/0]).
-include("schema.hrl").

startapp()->
  application:start(mhive).

start(_Type, StartArgs)->
  mnesia:create_schema([node()]),
  mnesia:start(),
  libdb:create(),
%  mnesia:create_table(files,[{disc_copies, [node()]},{attributes, record_info(fields, file)}]),
  file_scanner_sup:start_link(StartArgs).
%  mhive_supervisor:start_link(StartArgs).

stop(_State)->
%  application:stop(mnesia),
%  mnesia:stop(),
  ok.
