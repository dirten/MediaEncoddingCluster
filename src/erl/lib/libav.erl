-module(libav).
-include("schema.hrl").
-compile(export_all).



rescale_timestamp(TimeStamp, From, To)->
%  io:format("Input:~p",[TimeStamp]),
  B=From#timestamp.num*To#timestamp.den,
  C=To#timestamp.num*From#timestamp.den,
  R=C/2,
  round((TimeStamp*B+R)/C).

