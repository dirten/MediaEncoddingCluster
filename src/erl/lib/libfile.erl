-module(libfile).
-export([find/3,copy/2, copy_dir/3, touch/1, symlink/2]).


find(_Dir,_Filter,_Recursive)->
  ok.

touch(File)->
  {ok, Fd} = file:open(File, [write]),
  file:close(Fd).
copy(Src, Dst)->
%  io:format("Copy from:~p to:~p~n",[Src,Dst]),
  filelib:ensure_dir(Dst),
  file:copy(Src,Dst).

symlink(Src, Dst)->
  file:make_symlink(Src, Dst).

copy_dir(Src, Dst, Filter)->
  Fun=fun(File, Acc)->
		case is_ignored(File, Filter) of
			true -> Acc;
			false -> [File|Acc]
		end
  end,
  FileList=filelib:fold_files(Src,".*", true, Fun,[]),
  [copy(File, filename:join(Dst, File))||File<-FileList],
  ok.

is_ignored(_, []) -> false;
is_ignored(File, [H|T]) ->
	case string:str(File, H) of
		0 -> is_ignored(File, T);
		_ -> true
	end.
