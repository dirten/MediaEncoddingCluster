-module(libfile).
-export([find/3,copy/2, copy_dir/2]).


find(_Dir,_Filter,_Recursive)->
  ok.

copy(_Src, _Dst)->
  ok.

copy_dir(Src, Dst)->
  file:copy(Src,Dst).

