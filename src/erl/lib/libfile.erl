-module(libfile).
-export([find/3,find/4,copy/2, copy_dir/3, touch/1, symlink/2, write/2]).
-include_lib("kernel/include/file.hrl").



find(Name,Filter,Rec)->
  find(Name,Filter,Rec,[]).

find(Name,Filter,Rec, _Acc)->
  Fun=fun(File, Acc)->
          case string:len(Filter) of
            0->[File|Acc];
            _->
              case string:str(File, Filter) of
                0-> Acc;
                _-> [File|Acc]
              end
          end
      end,
  filelib:fold_files(Name,".*", Rec, Fun,[]).

touch(File)->
  {ok, Fd} = file:open(File, [write]),
  file:close(Fd).

copy(Src, Dst)->
  filelib:ensure_dir(Dst),
  io:format("CopyFile from:~p to ~p",[Src, Dst]),
  {ok,Bytes}=file:copy(Src,Dst),
  io:format("BytesCopyed:~p~n",[Bytes]),
  {ok, FileInfo} = file:read_file_info(Src),
  file:write_file_info(Dst, FileInfo).

symlink(Src, Dst)->
  file:make_symlink(Src, Dst).


write(FName, Conts) ->
  {ok, Fd} = file:open(FName, [write]),
  file:write(Fd, Conts),
  file:close(Fd).

copy_dir(Src, Dst, Filter)->
  Fun=fun(File, Acc)->
          case is_ignored(File, Filter) of
            true -> Acc;
            false -> [File|Acc]
          end
      end,
  FileList=filelib:fold_files(Src,".*", true, Fun,[]),
  io:format("CopyDir:~p",[FileList]),
 
  {ok,Cwd}=file:get_cwd(),
   io:format("CopyTo:~p~n",[Dst]),
  [copy(File, filename:join([Dst, File]))||File<-FileList],
  ok.

is_ignored(_, []) -> false;
is_ignored(File, [H|T]) ->
	case string:str(File, H) of
		0 -> is_ignored(File, T);
		_ -> true
	end.
