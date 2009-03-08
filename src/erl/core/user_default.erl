-module(user_default).

-compile(export_all).

logload()->
  rb:start().

loglist()->
  rb:list().

logshow(Nr)->
  rb:show(Nr).


