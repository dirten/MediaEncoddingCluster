-module(libfile).
-export([find/3,find/4,copy/2, copy_dir/3, touch/1, symlink/2, files/3]).
-include_lib("kernel/include/file.hrl").


files(Top, Re, Action) ->
    case file:list_dir(Top) of
	{ok, Files} ->
	    files(Top, Files, Re, Action, []);
	{error, Reason}  ->
	    {error, {Top, Reason}}
    end.

files(Top, [F|Tail], Re, Action, Ack) ->
    F2 = Top ++ "/" ++ F,
    case file:read_file_info(F2) of
	{ok, FileInfo} when FileInfo#file_info.type == directory ->
	    case files(F2, Re, Action) of
		{error, Reason} ->
		    {error, Reason};
		List ->
		    files(Top, Tail, Re, Action, List ++ Ack)
	    end;
	{error, Reason} ->
	    {error, {F2, Reason}};
	{ok, FileInfo} when FileInfo#file_info.type == regular ->
	    case catch regexp:match(F, Re) of
		{match, _,_} ->
		    files(Top, Tail, Re, Action, [Action(F2) | Ack]);
		nomatch ->
		    files(Top, Tail, Re, Action, Ack);
		{error, Reason} ->
		    {error, {F2, {regexp, Reason}}}
	    end;
	Other ->
	    files(Top, Tail, Re, Action, Ack)
    end;

files(_Top, [], _Re, _Action, Ack) ->
    Ack.


find(Name,Filter,Rec)->
  find(Name,Filter,Rec,[]).

find(Name,Filter,Rec, Acc)->
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
  filelib:fold_files(Name,".*", true, Fun,[]).

find()->
  ok.
touch(File)->
  {ok, Fd} = file:open(File, [write]),
  file:close(Fd).

copy(Src, Dst)->
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
