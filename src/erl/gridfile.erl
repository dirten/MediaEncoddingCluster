%%%
%%% The contents of this file are subject to the Erlang Public License,
%%% Version 1.0, (the "License"); you may not use this file except in
%%% compliance with the License. You may obtain a copy of the License at
%%% http://www.erlang.org/license/EPL1_0.txt
%%%
%%% Software distributed under the License is distributed on an "AS IS"
%%% basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
%%% the License for the specific language governing rights and limitations
%%% under the License.
%%%
%%% The Original Code is gridfile-1.0.
%%%
%%% The Initial Developer of the Original Code is Ericsson Telecom
%%% AB. Portions created by Ericsson are Copyright (C), 1998, Ericsson
%%% Telecom AB. All Rights Reserved.
%%%
%%% Contributor(s): ______________________________________.
 
%%%----------------------------------------------------------------------
%%% #0.    BASIC INFORMATION
%%%----------------------------------------------------------------------
%%% File:       gridfile.erl
%%% Author       : Ulf Wiger <ulf.wiger@ericsson.com>
%%% Description  : Adaptable, symmetric multikey file structure
%%% 
%%% Modules used : io,gen_server,ets,lists,file,filename
%%% 
%%%----------------------------------------------------------------------
-module(gridfile).
-vsn('1.0').
-date('99-01-21').
-author('ulf.wiger@ericsson.com').
-behaviour(gen_server).

-export([open/2,
	 new/3,
	 close/1]).
-export([read/2, write/2, delete/2, 
	 lookup/2, insert/2,
	 next/3, previous/3,
	 tab2list/1,
	 match/2,
	 match_object/2,
	 range_match/2]).

-export([init/1,
	 handle_call/3,
	 handle_info/2,
	 handle_cast/2,
	 terminate/2]).

-record(state, {dimensions = 3,
		keypos = 2,
		grid = tab(grid),
		buckets = tab(buckets),
		scales = [],
		next_scale_no = 1,
		split_threshold = 10,
		merge_threshold = 2,
		next_bucket = {[], 1},  %% {FreeBuckets, NewBucketNo}
		split_direction = 1,
		parent,
		dir,
		name}).

-ifdef(debug).
-define(dbg(Fmt,Args), io:format(Fmt, Args)).
-else.
-define(dbg(Fmt,Args), no_debug).
-endif.


%%% ===========================================================
%%% This module is based on 
%%% "The Grid File: An Adaptable, Symmetric Multikey File Structure"
%%% by L, Nievergelt, H. Hinterberger
%%% Institut für Informatik, ETH
%%% and K.C. Sevcik, University of Toronto
%%%
%%% Found in "Readings in Database Systems", 2nd Ed, pp 108-24
%%% edited by Michael Stonebraker.
%%% (c) 1994 Morgan Kaufmann Publishers, San Mateo, California
%%%	-	-	-	-	-	-	-
%%% The basic idea is to form a multi-dimensional grid where each cell
%%% points to a specific bucket. In my implementation, the grid scales
%%% are represented as balanced binary trees, and the buckets are represented
%%% as separate files, into which objects are written using file:write_file/2.
%%%
%%% Rough performance measurements (1000 objects, 3 dimensions) indicate:
%%% - insert: ca 3-5 ms/object (dets: ca 2 ms)
%%% - lookup: ca 1.3 ms/object (dets: ca 450 us)
%%% - delete: ca 2 ms/object
%%% - match:  ca 50-60 ms matching 100 objects from 1000 (whole table scanned)
%%% - range_match: ca 17 ms for a search matching 95 objects.
%%%
%%% The above measurements where taken on an UltraSPARC 1 running
%%% BEAM on Solaris.
%%% ===========================================================


%%% ===========================================================
%%% ===================== INTERFACE ===========================
%%% ===========================================================


%% -----------------------------------------------
%% open(Dir, GridFileName) -> {ok, GridFile}.
%%
%%   Dir ::= string()
%%   GridFileName ::= string()
%%   GridFile ::= pid()
%%
%% This function will open an existing gridfile.
%% Gridfiles are currently represented on disk as
%%
%%   Dir/GridFileName.bin
%%   Dir/GridFileName.buckets/XXX.bin
%%
%% -----------------------------------------------
open(Dir, GridFile) ->
    gen_server:start_link(?MODULE, {self(), {open,Dir,GridFile}}, []).

%% -----------------------------------------------
%% new(Dir, GridFile, Options) -> {ok, GridFile}.
%%
%%   Dir ::= string()
%%   GridFileName ::= string()
%%   GridFile ::= pid()
%%   Options ::= [{atom(), term()}]
%%
%% This function will open an existing gridfile.
%% Gridfiles are currently represented on disk as
%%
%%   Dir/GridFileName.bin
%%   Dir/GridFileName.buckets/XXX.bin
%%
%% Valid options are:
%%   {dimensions, Dim : integer()}
%%	The key is expected to be a tuple of size Dim
%%   {keypos, integer()}
%%	Key position
%%   {split_threshold, SplitThr : integer()}
%%	A bucket split is attempted if the bucket holds more than 
%%	SplitThr objects
%%   {merge_threshold, MergeThr : integer()}
%%	A bucket merge is attempted if the bucket holds fewer than
%%	MergeThr objects.
%%
%% -----------------------------------------------
new(Dir, GridFile, Options) ->
    gen_server:start_link(?MODULE, {self(), {new, Dir, GridFile,Options}}, 
			  []).


%% -----------------------------------------------
%% read(GridFile, Key) -> [] | [Obj]
%% lookup(GridFile, Key) -> [] | [Obj]
%%
%% Analogous to ets:lookup(Tab, Key)
%% -----------------------------------------------
read(GF,Key) -> lookup(GF,Key).
lookup(GF, Key) -> req(GF, {read, Key}).


%% -----------------------------------------------
%% insert(GridFile, Obj) -> ok
%% write(GridFile, Obj) -> ok
%%
%% Analogous to ets:insert(Tab, Obj)
%% -----------------------------------------------
write(GF, Obj) -> req(GF, {write, Obj}).
insert(GF,Obj) -> write(GF, Obj).


%% -----------------------------------------------
%% delete(GridFile, Key) -> ok
%%
%% Analogous to ets:delete(Tab, Key)
%% -----------------------------------------------
delete(GF, Key) -> req(GF, {delete, Key}).


%% -----------------------------------------------
%% next(GridFile, Dimension, Key) -> [] | [Obj]
%%
%% Returns the next object in the given dimension
%% -----------------------------------------------
next(GF, Dimension, Key) -> req(GF, {next, Dimension, Key}).


%% -----------------------------------------------
%% previous(GridFile, Dimension, Key) -> [] | [Obj]
%%
%% Returns the previous object in the given dimension
%% -----------------------------------------------
previous(GF, Pos, Key) -> req(GF, {prev, Pos, Key}).


%% -----------------------------------------------
%% tab2list(GridFile) -> [Obj]
%%
%% Analogous to ets:tab2list(Tab)
%% -----------------------------------------------
tab2list(GF) -> req(GF, tab2list).


%% -----------------------------------------------
%% match(GridFile, Pattern) -> [Obj]
%%
%% Analogous to ets:match(Tab, Pattern)
%% -----------------------------------------------
match(GF, Pat) -> req(GF, {match, Pat}).

%% -----------------------------------------------
%% match_object(GridFile, Pattern) -> [Obj]
%%
%% Analogous to ets:match_object(Tab, Pattern)
%% -----------------------------------------------
match_object(GF, Pat) -> req(GF, {match_object, Pat}).


%% -----------------------------------------------
%% range_match(GridFile, Pattern) -> [Obj]
%%
%% Performs a match operation on a range of key values
%% Pattern is similar to that used in ets:match_object(Tab,Pattern),
%% but with the following elements allowed for the key position
%%
%%   '_'
%%	matches all key components in the given dimension
%%   {exact, Value}
%%	matches Value in the given dimension
%%   {range,Min,Max}
%%	matches all key components where Min =< Value >= Max
%%
%% If '_' is given in the key position, range_match/2 behaves as
%% match_object/2.
%% -----------------------------------------------
range_match(GF, Pat) -> req(GF, {range_match, Pat}).


%% -----------------------------------------------
%% close(GridFile) -> ok.
%%
%% closes the gridfile.
%% -----------------------------------------------
close(GF) -> req(GF, close).


%%% ===========================================================
%%% ================== IMPLEMENTATION =========================
%%% ===========================================================


init({Parent, Action}) ->
    process_flag(trap_exit, true),
    S = open_gridfile(Action, #state{parent = Parent}),
    {ok, S}.

handle_call(close, From, S) ->
    Res = close_gridfile(S),
    {stop, normal, ok, S};
handle_call(Query, From, S) ->
    {Res, S1} = catch_op(Query, S),
    {reply, Res, S1}.


handle_info({'EXIT', Pid, Reason}, S) when Pid == S#state.parent ->
    close_gridfile(S),
    {stop, normal, S};
handle_info(_Msg, S) ->
    {noreply, S}.

handle_cast(Msg, S) ->
    {noreply, S}.


terminate(normal, S) -> ok;
terminate(Reason, S) ->
    close_gridfile(S).


catch_op(Query, S) ->
    case catch handle_op(Query, S) of
	{'EXIT', {terminal, Reason}} ->
	    %% terminal exit - must close file
	    exit(Reason);
	{'EXIT', Reason} ->
	    %% non-terminal exit, reply to caller
	    {{'EXIT',Reason}, S};
	{Reply, State} ->
	    {Reply, State}
    end.
    
handle_op({read, Key}, S) ->
    {read_op(Key, S), S};
handle_op({write, Obj}, S) ->
    {Res, S1} = write_op(Obj, S),
    {Res, S1};
handle_op({delete, Key}, S) ->
    {Res, S1} = delete_op(Key, S),
    {Res, S1};
handle_op({next, Pos, Key}, S) ->
    {next_op(Pos, Key, S), S};
handle_op({prev, Pos, Key}, S) ->
    {previous_op(Pos, Key, S), S};
handle_op(tab2list, S) ->
    {tab2list_op(S), S};
handle_op({match, Pat}, S) ->
    {match_op(Pat, match, S), S};
handle_op({match_object, Pat}, S) ->
    {match_op(Pat, match_object, S), S};
handle_op({range_match, Pat}, S) ->
    {range_op(Pat, S), S}.



read_op(Key, S) ->
    case bucket_by_key(Key, S, nocreate) of
	{_, _, _, _, Objs, _, _} ->
	    keysearch(Objs, S#state.keypos, Key);
	Other ->
	    Other
    end.


write_op(Obj, S) ->
    ?dbg("write_op(~p)~n", [Obj]),
    KeyPos = S#state.keypos,
    Key = element(KeyPos, Obj),
    case bucket_by_key(Key, S, create) of
	{BucketNo, GridPos, BucketF, Count, Objs, Ranges, S1} ->
	    {NewObjs, NewCount} = 
		keyreplaceadd(Key, KeyPos, Objs, Obj, Count),
	    if NewCount > S#state.split_threshold ->
		    ?dbg("bucket overflow~n", []),
		    S2 = split_bucket(BucketNo, GridPos, NewObjs, 
				      NewCount, Ranges, S1),
		    {ok, S2};
	       true ->
		    ?dbg("no split needed~n", []),
		    put_bucket(BucketF, BucketNo, GridPos, NewObjs, S1)
	    end;
	Other ->
	    ?dbg("unexpected: ~p~n", [Other]),
	    {Other, S}
    end.


delete_op(Key, S) ->
    ?dbg("delete_op(~p)~n", [Key]),
    KeyPos = S#state.keypos,
    case bucket_by_key(Key, S, nocreate) of
	{BucketNo, GridPos, BucketF, Count, Objs, Ranges, S1} ->
	    NewObjs = lists:keydelete(Key, KeyPos, Objs),
	    NewCount = length(NewObjs),
	    if NewCount < S#state.merge_threshold ->
		    ?dbg("delete of ~p causes merge~n", [Key]),
		    S2 = merge_bucket(BucketNo, GridPos, 
				      NewObjs, NewCount, Ranges, S1),
		    {ok, S2};
	       true ->
		    put_bucket(BucketF, BucketNo, GridPos, NewObjs, S1)
	    end;
	[] ->
	    ?dbg("no bucket for ~p~n", [Key]),
	    {ok, S};
	Other ->
	    {Other, S}
    end.


next_op(Dim, Key, S) ->
    KeyPos = S#state.keypos,
    case bucket_by_key(Key, S, nocreate) of
	{_, GridPos, _, _, Objs, _, S1} ->
	    case next_in_list(Objs, KeyPos, Key) of
		[] ->
		    next_in_grid(Dim, GridPos, Key, S1);
		Obj ->
		    [Obj]
	    end;
	Other ->
	    Other
    end.
	    
previous_op(Dim, Key, S) ->
    KeyPos = S#state.keypos,
    case bucket_by_key(Key, S, nocreate) of
	{_, GridPos, _, _, Objs, _, S1} ->
	    case previous_in_list(Objs, KeyPos, Key) of
		[] ->
		    previous_in_grid(Dim, GridPos, Key, S1);
		Obj ->
		    [Obj]
	    end;
	Other ->
	    Other
    end.

match_op(Pat, Type, S) ->
    KeyPat = 
	if Pat == '_' -> '_';
	   true ->
		element(S#state.keypos, Pat)
	end,
    Objs = 
	case KeyPat of
	    '_' ->
		tab2list_op(S);
	    P when atom(P) ->
		case {is_var(P),Type} of
		    true ->
			tab2list_op(S)
		end;
	    Tuple when size(Tuple) == S#state.dimensions ->
		case has_wildcards(Tuple) of
		    false ->
			read_op(Tuple, S);
		    true ->
			tab2list_op(S)
		end;
	    Other ->
		{error, pattern}
	end,
    if Objs == [] ->
	    [];
       true ->
	    T = ets:new(match_op, [{keypos, S#state.keypos}]),
	    store_temp(Objs, T),
	    Res = case Type of
		      match_object ->
			  ets:match_object(T, Pat);
		      match ->
			  ets:match(T, Pat)
		  end,
	    ets:delete(T),
	    Res
    end.

has_wildcards(T) when tuple(T) ->
    Vals = tuple_to_list(T),
    check_for_wildcards(Vals);
has_wildcards(L) when list(L) ->
    check_for_wildcards(L).

check_for_wildcards(['_'|_]) -> true;
check_for_wildcards([A|T]) when atom(A) ->
    case is_var(A) of
	true -> true;
	false -> check_for_wildcards(T)
    end;
check_for_wildcards([H|T]) when tuple(H) ->
    case has_wildcards(H) of
	true -> true;
	false -> check_for_wildcards(T)
    end;
check_for_wildcards([H|T]) when list(H) ->
    case has_wildcards(H) of
	true -> true;
	false -> check_for_wildcards(T)
    end;
check_for_wildcards([H|T]) ->
    check_for_wildcards(T);
check_for_wildcards([]) ->
    false.

%% is_var(Atom) -> bool().
 is_var(P) when atom(P) ->
    case atom_to_list(P) of
        [$_] -> true;
        [$$|Cs] -> digits(Cs);
        Other -> false
    end;
is_var(P) -> false.
 
digits([C|Cs]) when integer(C), C >= $0, C =< $9 -> digits(Cs);
digits([C|Cs]) -> false;
digits([]) -> true.

    

range_op(Pat, S) ->
    KeyPat = element(S#state.keypos, Pat),
    if size(KeyPat) /= S#state.dimensions ->
	    {error, dimensions};
       true ->
	    Objs =
		if KeyPat == '_' ->
			tab2list_op(S);
		   true ->
			Values = tuple_to_list(KeyPat),
			GridVals = range_match_grid(Values, S),
			Buckets = buckets(GridVals, S#state.grid),
			maybe_read_buckets(Buckets, S)
		end,
	    if Objs == [] -> [];
	       true ->
		    Temp = ets:new(range_op,[set,{keypos,S#state.keypos}]),
		    RawObjs = range_match_objects(Objs, S#state.keypos, 
						  tuple_to_list(KeyPat)),
		    store_temp(RawObjs, Temp),
		    MatchPat = setelement(S#state.keypos, Pat, '_'),
		    Res = ets:match_object(Temp, MatchPat),
		    ets:delete(Temp),
		    Res
	    end
    end.

store_temp([H|T], Tab) ->
    ets:insert(Tab, H),
    store_temp(T, Tab);
store_temp([], _) -> ok.

buckets([H|T], Grid) ->
    case ets:lookup(Grid, H) of
	[] -> buckets(T, Grid);
	[{_,B}] -> [{H, B}|buckets(T, Grid)]
    end;
buckets([], _) -> [].
	    


range_match_grid(Values, S) ->
    range_match_grid(Values, 1, S, []).

range_match_grid(['_'|T], Dim, S, Acc) -> 
    ?dbg("range_match_grid()~n", []),
    Range = [Px || {A,B,Px} <- whole_scale(element(Dim, S#state.scales))],
    range_match_grid(T, Dim+1, S, [Range|Acc]);
range_match_grid([{exact, Val}|T], Dim, S, Acc) ->
    Scale = element(Dim, S#state.scales),
    {_,_,Value} = search_scale(Scale, Val),
    range_match_grid(T, Dim+1, S, [[Value]|Acc]);
range_match_grid([{range,Min,Max}|T], Dim, S, Acc) ->
    Scale = element(Dim, S#state.scales),
    Values = range_search_scale(Scale, Min, Max),
    range_match_grid(T, Dim+1, S, [Values|Acc]);
range_match_grid([], _Dim, S, Acc) ->
    combine(lists:reverse(Acc), S#state.dimensions).

combine([], _) -> [];
combine(Vals, Dims) ->
    Empty = list_to_tuple(lists:duplicate(Dims, [])),
    combine(Vals, 1, [Empty]).

combine([Vals|T], Pos, Acc) ->
    ?dbg("combine(~p)~n", [Vals]),
    combine(T, Pos+1, [setelement(Pos, A, V) || A <- Acc,
						V <- Vals]);
combine([], _, Acc) -> Acc.
    

maybe_read_buckets(Buckets, S) ->
    Temp = ets:new(temp_buckets,[{keypos,2}]),
    store_temp(Buckets,Temp),
    Res = maybe_read_buckets1(ets:tab2list(Temp), S),
    ets:delete(Temp),
    Res.

maybe_read_buckets1([{_GridNo, Bucket}|T], S) ->
    case ets:lookup(S#state.buckets, Bucket) of
	[{_, 0}] ->
	    maybe_read_buckets1(T, S);
	_ ->
	    get_bucket(Bucket, S) ++ maybe_read_buckets1(T, S)
    end;
maybe_read_buckets1([], S) ->
    [].

tab2list_op(S) ->
    Buckets = ets:match_object(S#state.buckets, '_'),
    NonEmpty = [B || {B,Cnt} <- Buckets,
		     Cnt > 0],
    read_buckets(NonEmpty, S).

read_buckets([BN|T], S) ->
    Objs = get_bucket(BN, S),
    Objs ++ read_buckets(T, S);
read_buckets([], _) -> [].

bucket_by_key(Key, S, Create) ->
    if size(Key) /= S#state.dimensions ->
	    {{error, dimensions}, S};
       true ->
	    case grid_position(Key, S) of
		[] -> {{error, out_of_bounds}, S};
		{GridPos, Ranges} ->
		    case bucket_no(GridPos, S) of
			[] -> 
			    case Create of
				nocreate ->
				    ?dbg("no bucket for ~p "
					 "(Key = ~p)~n", [GridPos,Key]),
				    [];
				create ->
				    {BN,0,S1} = 
					new_bucket_no(GridPos,S),
				    ?dbg("new bucket (~p) for ~p "
					 "(Key = ~p)~n", 
					 [BN,GridPos,Key]),
				    BucketF = bucket_file(BN, S1),
				    {BN,GridPos,BucketF,0,[],Ranges,S1}
			    end;
			{BucketNo,Count} ->
%			    ?dbg("bucket for ~p is ~p (Key = ~p)~n",
%				 [GridPos,BucketNo,Key]),
			    BucketF = bucket_file(BucketNo, S),
			    Objs = get_bucket(BucketF, BucketNo, S),
			    {BucketNo,GridPos,BucketF,Count,Objs,Ranges,S}
		    end
	    end
    end.


%% next_on_scale(Scale, X, [Pos]) -> NextPos;
%% previous_on_scale(Scale, X, [Pos]) -> PrevPos.
%% There's a catch here: normally, next obj is the smallest in the right 
%% subtree, but it could also be the parent, if we're in the left subtree
%% of the parent. The converse is true for previous_on_scale/2
%%
next_on_scale({{{'_','_',P},_,_},_}, X, PosL) ->
    %% no need to check PosL - we must have started here, there is no next
    [];
next_on_scale({{{A,_,P},L,R},_}, X, PosL) when X < A ->
    case lists:member(P, PosL) of 
	true -> next_on_scale(R, X, PosL);
	false -> case next_on_scale(L, X, PosL) of
		     [] -> P;
		     Other -> Other
		 end
    end;
next_on_scale({{{A,'_',P},_,_},_}, X, PosL) ->
    %% there is no next after this
    case lists:member(P, PosL) of
	true -> [];
	false -> P
    end;
next_on_scale({{{_,B,P},_,R},D}, X, PosL) when X > B ->
    next_on_scale(R, X, PosL);
next_on_scale({{{A,_,P},L,R},D}, X, PosL) ->
    case lists:member(P, PosL) of
	true -> next_on_scale(R, X, PosL);
	false -> P
    end;
next_on_scale({[],_}, _, _) -> [].


previous_on_scale({{{'_','_',P},_,_},_}, X, PosL) ->
    %% no need to check PosL - we must have started here, there is no previous
    [];
previous_on_scale({{{_,B,P},L,R},_}, X, PosL) when X > B ->
    case lists:member(P, PosL) of 
	true -> previous_on_scale(L, X, PosL);
	false -> case previous_on_scale(R, X, PosL) of
		     [] -> P;
		     Other -> Other
		 end
    end;
previous_on_scale({{{'_',B,P},_,_},_}, X, PosL) ->
    %% there is no previous after this
    case lists:member(P, PosL) of
	true -> [];
	false -> P
    end;
previous_on_scale({{{A,_,P},L,_},D}, X, PosL) when X < A ->
    previous_on_scale(L, X, PosL);
previous_on_scale({{{_,_,P},L,R},D}, X, PosL) ->
    case lists:member(P, PosL) of
	true -> previous_on_scale(L, X, PosL);
	false -> P
    end;
previous_on_scale({[],_}, _, _) -> [].



find_parent_of(Pos, {{{A,B,P},{{X,L,R},C},Rp},Cp}) when element(3,X) == Pos ->
    {{{A,B,P},{{X,L,R},C},Rp},Cp};
find_parent_of(Pos, {{{A,B,P},{Lp,{X,L,R},C}},Cp}) when element(3,X) == Pos ->
    {{{A,B,P},{Lp,{X,L,R},C}},Cp};
find_parent_of(Pos, {{_,L,R},_}) ->
    case find_parent_of(Pos,L) of
	[] ->
	    find_parent_of(Pos,R)
    end;
find_parent_of(_, {[],_}) -> [].
		      
    

next_in_list([Obj,Obj1|_], Pos, Key) when element(Pos, Obj) == Key ->
    Obj1;
next_in_list([Obj|T], Pos, Key) ->
    next_in_list(T, Pos, Key);
next_in_list(_, _, _) -> [].

previous_in_list([Obj,Obj1|_], Pos, Key) when element(Pos, Obj1) == Key ->
    Obj;
previous_in_list([_|T], Pos, Key) ->
    previous_in_list(T, Pos, Key);
previous_in_list(_, _, _) -> [].


keysearch([Obj|Objs], Pos, Key) when element(Pos, Obj) == Key ->
    [Obj];
keysearch([Obj|Objs], Pos, Key) ->
    keysearch(Objs, Pos, Key);
keysearch([], _, _) -> [].

keyreplaceadd(Key, Pos, Objs, NewObj, Count) ->
    Objs1 = keyreplaceadd(Key, Pos, Objs, NewObj),
    {Objs1, length(Objs1)}.

keyreplaceadd(Key, Pos, [Obj|Objs], NewObj) when element(Pos, Obj) == Key ->
    [NewObj|Objs];
keyreplaceadd(Key, Pos, [Obj|Objs], NewObj) when element(Pos, Obj) < Key ->
    [Obj|keyreplaceadd(Key, Pos, Objs, NewObj)];
keyreplaceadd(_, _, Objs, NewObj) ->
    [NewObj|Objs].

grid_position(Key, S) -> 
    grid_position(tuple_to_list(Key), 1, S, PosAcc = [], RangeAcc = []).


grid_position([H|T], Dimension, S, Pacc, Racc) ->
    Scale = element(Dimension, S#state.scales),
    {A,B,Pos} = search_scale(Scale, H),
    grid_position(T, Dimension+1, S, [Pos|Pacc], [{A,B}|Racc]);
grid_position([], _, _, PA, RA) ->
    {list_to_tuple(lists:reverse(PA)), 
     list_to_tuple(lists:reverse(RA))}.


next_in_grid(Dim, GridNo, Key, S) ->
    Scale = element(Dim, S#state.scales),
    KeyComponent = element(Dim, Key),
    GridComponent = element(Dim, GridNo),
    next_in_grid(Dim, Scale, KeyComponent, [GridComponent], GridNo, S).

next_in_grid(Dim, Scale, KeyComponent, GridComps, GridNo, S) ->
    case next_on_scale(Scale, KeyComponent, GridComps) of
	[] ->
	    [];
	GridComp1 ->
	    GridNo1 = setelement(Dim, GridNo, GridComp1),
	    case bucket_no(GridNo1, S) of
		[] ->
		    next_in_grid(Dim, Scale, KeyComponent, 
				 [GridComp1|GridComps], GridNo1, S);
		{BN, 0} ->
		    next_in_grid(Dim, Scale, KeyComponent, 
				 [GridComp1|GridComps], GridNo1, S);
		{BN, _} ->
		    [Obj|_] = get_bucket(BN, S),
		    [Obj]
	    end
    end.

previous_in_grid(Dim, GridNo, Key, S) ->
    Scale = element(Dim, S#state.scales),
    KeyComponent = element(Dim, Key),
    GridComponent = element(Dim, GridNo),
    previous_in_grid(Dim, Scale, KeyComponent, [GridComponent], GridNo, S).

previous_in_grid(Dim, Scale, KeyComponent, GridComps, GridNo, S) ->
    case previous_on_scale(Scale, KeyComponent, GridComps) of
	[] ->
	    [];
	GridComp1 ->
	    GridNo1 = setelement(Dim, GridNo, GridComp1),
	    case bucket_no(GridNo1, S) of
		[] ->
		    previous_in_grid(Dim, Scale, KeyComponent, 
				     [GridComp1|GridComps], GridNo1, S);
		{BN, 0} ->
		    previous_in_grid(Dim, Scale, KeyComponent, 
				     [GridComp1|GridComps], GridNo1, S);
		{BN, _} ->
		    Objs = get_bucket(BN, S),
		    [hd(lists:reverse(Objs))]
	    end
    end.

bucket_no(GridPos, S) ->
    case ets:lookup(S#state.grid, GridPos) of
	[] -> [];
	[{_, BN}] ->
	    Count = case ets:lookup(S#state.buckets, BN) of
			[] -> 0;
			[{_, Cnt}] ->
			    Cnt
		    end,
	    {BN, Count}
    end.


new_bucket_no(GridPos, S) ->
    case S#state.next_bucket of
	{[H|T], Max} ->
	    ets:insert(S#state.grid, {GridPos, H}),
	    {H, 0, S#state{next_bucket = {T, Max}}};
	{[], Max} ->
	    ets:insert(S#state.grid, {GridPos, Max}),
	    {Max, 0, S#state{next_bucket = {[], Max+1}}}
    end.


split_bucket(Bucket, GridPos, Objs, Count, Ranges, S) ->
    %% GridPos here is one of possibly many grid positions pointing to this
    %% bucket.
    {Dim, S1} = split_direction(S),
    split_bucket(Dim, Dim, Bucket, GridPos, Objs, Count, Ranges, S1).

split_bucket(Dim, OrigDim, BucketA, GridPosA, Objs, Count, Ranges, S) ->
    ScalePosA = element(Dim, GridPosA),
    Scale = element(Dim, S#state.scales),
    {A,B} = find_scale_obj(Scale, ScalePosA),
    {ObjsToSplit, Rest} = range_filter(Objs, S#state.keypos, Dim,
				       element(Dim, Ranges)),
    if (Count - length(ObjsToSplit)) > S#state.split_threshold ->
	    %% not enough objects in this grid pos to make a split worthwhile
	    ?dbg("range_filter did not select enough objects (~p)~n", [Dim]),
	    split_other_dim(Dim, OrigDim, BucketA, 
			    GridPosA, Objs, Count, Ranges, S);
       true ->
	    SortedObjs = dimension_sort(Dim,S#state.keypos,ObjsToSplit),
	    case find_divisor(SortedObjs, 1, length(SortedObjs) div 2, 
			      S#state.keypos, Dim, []) of
		[] ->
		    %% didn't get a useful split
		    ?dbg("did not find a useful split (~p)~n", [Dim]),
		    split_other_dim(Dim, OrigDim, BucketA, 
				    GridPosA, Objs, Count, Ranges, S);
		{Div, ObjsA, ObjsB} ->
		    case {Div,A,B} of
			{X,'_','_'} ->
			    ok;
			{X,'_',B} when X < B -> 
			    ok;
			{X,A,'_'} when X >= A ->
			    ok;
			{X,A,B} when X > A, X < B ->
			    ok;
			_ ->
			    exit({terminal,{split_error,{Div, ObjsA, ObjsB},
					    {A,B,Dim,GridPosA,Scale}}})
		    end,
		    ?dbg("split (~p) on ~p-~p-~p: A: ~p, B: ~p~n", 
			 [Dim,A,Div,B,ObjsA,ObjsB]),
		    ScaleObjA = {A,Div,ScalePosA},
		    {ScalePosB, S0} = next_scale_no(S),
		    ScaleObjB = {Div,B,ScalePosB},
		    GridPosB = setelement(Dim,GridPosA,ScalePosB),
		    {BucketB, _, S1} =
			new_bucket_no(setelement(Dim,GridPosA,ScalePosB), S0),
		    ?dbg("split ~p -> new bucket (~p) for ~p A:~p - B:~p~n", 
			 [GridPosA,BucketB,GridPosB,ScaleObjA,ScaleObjB]),
		    {ok, S2} = put_bucket(BucketA, GridPosA, ObjsA++Rest, S1),
		    {ok, S3} = put_bucket(BucketB, GridPosB, ObjsB, S2),
		    split_bucket_pointers(Dim,ScalePosA,BucketA,
					  ScalePosB,BucketB,S3),
		    Scale1 = insert_scale_split(Scale, ScalePosA, 
						ScaleObjA, ScaleObjB, S3),
		    ?dbg("after scale split: ~p~n", [Scale1]),
		    S3#state{scales = 
			     setelement(Dim, S3#state.scales, Scale1)};
		Other ->
		    exit({terminal, {split_error, {Other, Scale}}})
	    end
    end.

split_other_dim(Dim,OrigDim,BucketA,GridPosA,Objs,Count,Ranges,S) ->
    case split_direction(S) of
	{Dim1, S1} when Dim1 == OrigDim ->
	    %% we've tried every split direction - can't split
	    ?dbg("couldn't split bucket ~p~n", [BucketA]),
	    {ok,S2} = put_bucket(BucketA, GridPosA, Objs, S1),
	    S2;
	{Dim1, S1} ->
	    split_bucket(Dim1,OrigDim,BucketA,GridPosA,Objs,Count,Ranges,S1)
    end.


range_filter(Objs, Pos, _, {'_','_'}) -> {Objs,[]};
range_filter(Objs, Pos, Dim, Range) -> 
    range_filter(Objs, Pos, Dim, Range, Inside = [], Outside = []).

range_filter([Obj|Objs], Pos, Dim, Range, In, Out) ->
    X = element(Dim, element(Pos, Obj)),
    {NewIn, NewOut} =
	case Range of
	    {'_',B} when X < B -> {[Obj|In], Out};
	    {A,'_'} when X >= A -> {[Obj|In],Out};
	    {A,B} when X >= A, X < B -> {[Obj|In],Out};
	    _ -> {In,[Obj|Out]}
	end,
    range_filter(Objs, Pos, Dim, Range, NewIn, NewOut);
range_filter([],_,_,_,In,Out) ->
    {In,Out}.


split_bucket_pointers(Dim,PosA,BucketA,PosB,BucketB, S) ->
    Wild = wild_key_pattern(S),
    KeyPat = setelement(Dim,Wild,PosA),
    Tab = S#state.grid,
    Pointers = ets:match_object(S#state.grid,{KeyPat,'_'}),
    ?dbg("readjust pointers, Buckets ~p and ~p (Dim:~p, PosB:~p):~n"
	 "   ~p~n", [BucketA,BucketB,Dim,PosB,Pointers]),
    split_pointers(Pointers,Tab, BucketA, BucketB, PosB, Dim).

split_pointers([{GridPos,BucketA}|Ps],Tab,BucketA,BucketB,PosB,Dim) ->
    GP1 = setelement(Dim,GridPos,PosB),
    ets:insert(Tab,{GP1,BucketB}),
    ?dbg("inserted ~p~n", [{GP1, BucketB}]),
    split_pointers(Ps,Tab,BucketA,BucketB,PosB,Dim);
split_pointers([{GridPos,OtherBucket}|Ps],Tab,BucketA,BucketB,PosB,Dim) ->
    GP1 = setelement(Dim, GridPos, PosB),
    ets:insert(Tab, {GP1, OtherBucket}),
    ?dbg("inserted ~p~n", [{GP1, OtherBucket}]),
    split_pointers(Ps,Tab,BucketA,BucketB,PosB,Dim);
split_pointers([],_,_,_,_,_) -> ok.



merge_bucket(Bucket, GridPos, Objs, Count, Ranges, S) ->
    {Dim, S1} = split_direction(S),
    merge_bucket(Dim, Dim, Bucket, GridPos, Objs, Count, Ranges, S1).

merge_bucket(Dim, OrigDim, BucketA, GridPosA, ObjsA, Count, Ranges, S) ->
    PosA = element(Dim, GridPosA),
    Scale = element(Dim, S#state.scales),
    SubTree = find_position(Scale, PosA),
    case SubTree of
	{{_,{[],_},{[],_}},_} ->
	    merge_other_dim(Dim,OrigDim,BucketA,GridPosA,ObjsA,Count,Ranges,S);
	{{_,L,R},_} ->
	    Dirs = case {L,R} of
		       {{[],_},R} -> [right];
		       {L,{[],_}} -> [left];
		       {{_,Lc},{_,Rc}} ->
			   if Lc > Rc ->
				   [right,left];
			      true ->
				   [left,right]
			   end
		   end,
	    case try_merge(Dirs,SubTree,PosA,Dim,OrigDim,
			   BucketA,GridPosA,ObjsA,Count,Ranges,S) of
		false ->
		    ?dbg("couldn't merge bucket ~p (Dim=~p)~n", 
			 [BucketA,Dim]),
		    merge_other_dim(Dim,OrigDim,BucketA,
				    GridPosA,ObjsA,Count,Ranges,S);
		S1 ->
		    S1
	    end
    end.

try_merge([],_,_,_,_,_,_,_,_,_,_) -> false;
try_merge([Dir|Dirs],SubTree,PosA,Dim,OrigDim,
	  BucketA,GridPosA,ObjsA,Count,Ranges,S) ->
    ObjB = case Dir of
	       left ->
		   {{_,L,_},_} = SubTree,
		   find_biggest(L);
	       right ->
		   {{_,_,R},_} = SubTree,
		   find_smallest(R)
	   end,
    case ObjB of
	[] ->
	    ?dbg("couldn't find a neighbor (~p)~n", [Dim]),
	    try_merge(Dirs,SubTree,PosA,Dim,OrigDim,
		      BucketA,GridPosA,ObjsA,Count,Ranges,S);
	{_,_,PosB} ->
	    GridPosB = setelement(Dim,GridPosA,PosB),
	    case bucket_no(GridPosB,S) of
		[] ->
		    %% no objects in this grid position
		    try_merge(Dirs,SubTree,PosA,Dim,OrigDim,
			      BucketA,GridPosA,ObjsA,Count,Ranges,S);
		{BucketB,CountB} ->
		    case Count + CountB of
			Sum when Sum < S#state.split_threshold ->
			    do_merge(Dir,PosA,PosB,BucketB,GridPosB,CountB,
				     SubTree,Dim,OrigDim,BucketA,
				     GridPosA,ObjsA,Count,Ranges,S);
			_ ->
			    ?dbg("merge (~p + ~p) would exceed threshold "
				 "(Dim=p)~n", [Count,CountB,Dim]),
			    try_merge(Dirs,SubTree,PosA,Dim,OrigDim,
				      BucketA,GridPosA,ObjsA,Count,Ranges,S)
		    end
	    end
    end.

do_merge(Dir,PosA,PosB,BucketB,GridPosB,CountB,SubTree,Dim,OrigDim,
	 BucketA,GridPosA,ObjsA,CountA,Ranges,S) ->
    ObjsB = 
	if BucketA == BucketB -> 
		?dbg("same bucket; we're only merging the grid~n", []),
		[];
	   true ->
		get_bucket(BucketB, S)
	end,
    NewObjs = lists:keysort(S#state.keypos, ObjsA ++ ObjsB),
    ?dbg("merging ~p (~p) with ~p (~p) (Dim=~p)~n"
	 "ObjsA = ~p~n"
	 "ObjsB = ~p~n"
	 "Res -> ~p~n", [BucketA, PosA,BucketB,PosB,Dim,
			 ObjsA,ObjsB,NewObjs]),
    SubTree1 =
	begin
	    {{{Aa,Ba,Pa},L,R},_} = SubTree,
	    case Dir of
		left ->
		    {{Ab,Bb,Pb}, L1} = find_delete_pos(L, PosB),
		    {{{Ab,Ba,Pa},L1,R},max(depth(L1),depth(R))+1};
		right ->
		    {{Ab,Bb,Pb}, R1} = find_delete_pos(R, PosB),
		    {{{Aa,Bb,Pa},L,R1},max(depth(L),depth(R1))+1}
	    end
	end,
    Scale = element(Dim,S#state.scales),
    Scale1 = balance_tree(replace_position(PosA, Scale, SubTree1)),
    ?dbg("before = ~p~nafter = ~p~n", [Scale,Scale1]),
    S1 = S#state{scales = setelement(Dim,S#state.scales,Scale1)},
    {ok, S2} = put_bucket(BucketA, GridPosA, NewObjs, S1),
    S3 = if BucketA == BucketB ->
		 %% don't write bucket again; just remove the GridPosB entry
		 ets:delete(S#state.grid, GridPosB),
		 S2;
	    true ->
		 {ok, S3x} = put_bucket(BucketB, GridPosB, [], S2),
		 S3x
	 end,
    merge_bucket_pointers(BucketB,BucketA,Dim,PosA,PosB,S),
    S3.

merge_bucket_pointers(BucketB,BucketA,Dim,PosA,PosB,S) ->
    Tab = S#state.grid,
    Wild = wild_key_pattern(S),
    GridPat = setelement(Dim,Wild,PosB),
    ?dbg("readjusting pointers after merge ~p to ~p (Dim=~p)~n",
	 [BucketB,BucketA,Dim]),
    Pointers = ets:match_object(Tab, {GridPat,BucketB}),
    merge_pointers(Pointers,Tab,Dim,PosA,BucketA,S).

merge_pointers([{GridPos,BucketB}|Objs],Tab,Dim,PosA,BucketA,S) ->
    GP1 = setelement(Dim,GridPos,PosA),
    ?dbg("delete ~p; insert ~p~n", [{GridPos,BucketB},{GP1,BucketA}]),
    ets:delete(Tab, GridPos),
    ets:insert(Tab, {GP1,BucketA}),
    merge_pointers(Objs,Tab,Dim,PosA,BucketA,S);
merge_pointers([],_,_,_,_,_) ->
    ok.


merge_other_dim(Dim,OrigDim,BucketA,GridPosA,ObjsA,Count,Ranges,S) ->
    case split_direction(S) of
	{Dim1, S1} when Dim1 == OrigDim ->
	    %% we've tried every direction - can't merge
	    ?dbg("cannot merge bucket ~p~n"
		 "Objs = ~p~n", [BucketA, ObjsA]),
	    {ok,S2} = put_bucket(BucketA, GridPosA, ObjsA, S1),
	    S2;
	{Dim1, S1} ->
	    merge_bucket(Dim1,OrigDim,BucketA,GridPosA,ObjsA,Count,Ranges,S1)
    end.




find_position({{{A,B,P},L,R},Cnt}, P) -> {{{A,B,P},L,R},Cnt};
find_position({[],_}, _) -> [];
find_position({{_,L,R},_},P) ->
    case find_position(L,P) of
	[] -> find_position(R,P);
	Other -> Other
    end.


find_delete_pos({{{A,B,P},{[],Dl},R},D}, P) ->
    {{A,B,P}, R};
find_delete_pos({{{A,B,P},L,{[],_}},D}, P) ->
    {{A,B,P}, L};
find_delete_pos({[],_}, _) -> [];
find_delete_pos({{X,L,R},D},P) ->
    case find_delete_pos(L,P) of
	[] ->
	    case find_delete_pos(R,P) of
		[] -> [];
		{Found,R1} ->
		    {Found, {{X,L,R1},depth(L,R1)}}
	    end;
	{Found,L1} ->
	    {Found,{{X,L1,R},depth(L1,R)}}
    end.


find_biggest({[],_}) -> [];
find_biggest({{X,_,{[],_}},_}) -> X;
find_biggest({{_,_,R},_}) -> find_biggest(R).

find_smallest({[],_}) -> [];
find_smallest({{X,{[],_},_},_}) -> X;
find_smallest({{_,L,_},_}) -> find_smallest(L).

replace_position(Pos, Scale, X) ->
    {ok,Scale1} = replace_position1(Pos,Scale,X),
    Scale2 = balance_tree(Scale1).


replace_position1(Pos, {[],_}, _) -> [];
replace_position1(Pos, {{{A,B,Pos},L,R},Cnt}, X) -> {ok, X};
replace_position1(Pos, {{{A,B,P},L,R},Cnt}, X) ->
    case replace_position1(Pos,L,X) of
	{ok, L1} -> 
	    Tree = {{{A,B,P},L1,R},depth(L1,R)},
	    {ok,Tree};
	[] ->
	    case replace_position1(Pos,R,X) of
		[] -> [];
		{ok, R1} ->
		    Tree = {{{A,B,P},L,R1},depth(L,R1)},
		    {ok, Tree}
	    end
    end.

balance_tree({{X,{{XL,LL,RL},DL},{{Xr,Lr,Rr},Dr}},D}) when (DL - Dr) > 2 ->
    R1 = {{Xr,Lr,Rr},Dr},
    D1 = depth(RL,R1),
    D2 = max(D1,depth(LL)) +1,
    {{XL,LL,{{X,RL,R1},D1}},D2};
balance_tree({{X,{{XL,LL,RL},DL},{{Xr,Lr,Rr},Dr}},D}) when (Dr - DL) > 2 ->
    D1 = depth(LL,RL),
    D2 = max(D1, depth(Lr)) +1,
    L1 = {{X,{{XL,LL,RL},D1},Lr},D2},
    {{Xr,L1,Rr}, depth(L1,Rr)};
balance_tree(T) -> T.


depth([]) -> 0;		    
depth({_,C}) -> C.

depth(L,R) -> max(depth(L),depth(R))+1.


max(A,B) when A > B -> A;
max(A,B) -> B.

    

return_bucket(BucketNo, S) ->
    case S#state.next_bucket of
	{L, BucketNo} ->
	    B1 = BucketNo-1,
	    S#state{next_bucket = {L -- [B1], B1}};
	{L, NewBucketNo} ->
	    S#state{next_bucket = {[BucketNo|L], NewBucketNo}}
    end.
	     


find_divisor([Obj,Obj1|Objs], Count, Count, KeyPos, Dim, Acc) ->
    Key = element(KeyPos, Obj),
    Div = element(Dim, Key),
    Key1 = element(KeyPos, Obj1),
    Div1 = element(Dim, Key1),
    if Div == Div1 ->
	    %% must have different key components on each side of the split
	    %% Continue (don't increment count) until the key comps differ
	    find_divisor([Obj1|Objs], Count, Count, KeyPos, Dim, [Obj|Acc]);
       true ->
	    {Div1, lists:reverse([Obj|Acc]), [Obj1|Objs]}
    end;
find_divisor([],_,_,_,_,_) -> [];
find_divisor([Obj],_,_,_,_,_) ->
    %% we couldn't split the bucket in this dimension
    [];
find_divisor([Obj|Objs], Count, Stop, KeyPos, Dim, Acc) ->
    find_divisor(Objs, Count+1, Stop, KeyPos, Dim, [Obj|Acc]).


find_scale_obj(Scale,P) ->
    {{{A,B,_},_,_},_} = find_position(Scale,P),
    {A,B}.


%% if we don't find the scale object, it must be an error (?)
insert_scale_split(Scale, Pos, ObjA, ObjB, S) ->
    SubTree = find_position(Scale,Pos),
    SubTree1 = begin
		   {{_,L,R},Count} = SubTree,
		   case {depth(L),depth(R)} of
		       {Ld,Rd} when Ld > Rd ->
			   %% more objects in the left subtree - split right
			   Rd1 = Rd+1,
			   Depth = max(depth(L),Rd1)+1,
			   {{ObjA,L,{{ObjB,{[],0},R},Rd1}},Depth};
		       {Ld,Rd} ->
			   %% more objects in the right subtree - split left
			   Ld1 = Ld+1,
			   Depth = max(Ld1, depth(R))+1,
			   {{ObjB,{{ObjA,L,{[],0}},Ld1},R}, Depth}
		   end
	       end,
    replace_position(Pos, Scale, SubTree1).


%% tree structure is Tree ::= {Node, Count}, 
%% Node ::= {Obj, L : Tree, R : Tree}
%% Obj  ::= {A,B,Pos}
%%
search_scale({{Obj,L,R},D}, X) ->
    case Obj of
	{'_','_',P} ->			Obj;
	{'_',B,P} when X < B ->		Obj;
	{'_',B,_} ->			search_scale(R,X);
	{A,'_',P} when X >= A ->	Obj;
	{A,'_',_} ->			search_scale(L, X);
	{A,B,P} when X >= A, X < B ->	Obj;
	{A,B,P} when X < B ->		search_scale(L, X);
	_ ->
	    search_scale(R, X)
    end.



range_search_scale([],_,_) -> [];
range_search_scale({{{'_','_',P},_,_},_}, _Min, _Max) -> [P];
range_search_scale({{{'_',B,P},L,R},_Cnt}, Min, Max) ->
    if Max =< B ->
	    [P];
       true ->
	    [P|range_find_max(R, Max)]
    end;
range_search_scale({{{A,'_',P},L,R},_}, Min, Max) ->
    if Min >= A ->
	    [P];
       true ->
	    [P|range_find_min(L, Min)]
    end;
range_search_scale({{{A,B,P},L,R},_}, Min, Max) when A >= Min, B =< Max ->
    Ps1 = range_find_min(L, Min),
    [P|Ps1] ++ range_find_max(R, Max);
range_search_scale({{{A,B,P},L,R},_}, Min, Max) when A >= Min ->
    range_search_scale(L, Min, Max);
range_search_scale({{_,_,R},_}, Min, Max) ->
    range_search_scale(R, Min, Max).
    

range_find_min({{{'_',_,P},_,R},_}, Min) ->
    [P|range_find_min(R, Min)];
range_find_min({{{A,_,P},L,R},_}, Min) when A >= Min ->
    Ps = range_find_min(L, Min),
    [P|Ps] ++ range_find_min(R, Min);
range_find_min({{_,_,R},_}, Min) ->
    range_find_min(R, Min);
range_find_min({[],_}, _) -> [].

range_find_max({{{_,'_',P},L,_},_}, Max) ->
    [P|range_find_max(L, Max)];
range_find_max({{{_,B,P},L,R},_}, Max) when B =< Max ->
    Ps = range_find_max(R, Max),
    [P|Ps] ++ range_find_max(L, Max);
range_find_max({{_,L,_},_}, Max) ->
    range_find_max(L, Max);
range_find_max({[],_}, _) -> [].


whole_scale({{Obj,L,R},_}) -> [Obj|whole_scale(L)] ++ whole_scale(R);
whole_scale({[],_}) -> [].


range_match_objects([Obj|T], Pos, Pat) ->
    Key = element(Pos, Obj),
    case match_key(tuple_to_list(Key), Pat) of
	true ->
	    [Obj|range_match_objects(T, Pos, Pat)];
	false -> range_match_objects(T, Pos, Pat)
    end;
range_match_objects([], _, _) ->
    [].

match_key([V|Vals], ['_'|Pats]) -> match_key(Vals, Pats);
match_key([V|Vals], [{exact, V}|Pats]) -> match_key(Vals, Pats);
match_key([V|Vals], [{range,'_','_'}|Pats]) -> match_key(Vals,Pats);
match_key([V|Vals], [{range,A,'_'}|Pats]) when V >= A->
    match_key(Vals,Pats);
match_key([V|Vals], [{range,'_',B}|Pats]) when V =< B ->
    match_key(Vals, Pats);
match_key([V|Vals], [{range,A,B}|Pats]) when V >= A, V =< B ->
    match_key(Vals, Pats);
match_key([], []) -> true;
match_key(_, _) -> false.


get_bucket(BucketNo, S) ->
    BucketF = bucket_file(BucketNo, S),
    get_bucket(BucketF, BucketNo, S).

get_bucket(BucketF, BucketNo, S) ->
    case file:read_file(BucketF) of
	{error, enoent} ->
	    %% probably some previous error, or bucket has been emptied
	    [];
	{ok, Bin} ->
	    Objs = binary_to_term(Bin),
	    ?dbg("get_bucket(~p) -> ~p~n", [BucketNo,Objs]),
	    Objs
    end.

put_bucket(BucketNo, GridPos, Objs, S) ->
    BucketF = bucket_file(BucketNo, S),
    put_bucket(BucketF, BucketNo, GridPos, Objs, S).

put_bucket(BucketF, BucketNo, GridPos, [], S) ->
    ?dbg("put_bucket(~p,~n"
	 "           ~p, [])~n", [BucketF,BucketNo]),
    Res = case file:delete(BucketF) of
	      {error, enoent} -> ok;
	      Other ->
		  Other
	  end,
    ets:delete(S#state.grid, GridPos),
    ets:delete(S#state.buckets, BucketNo),
    S1 = return_bucket(BucketNo, S),
    {Res, S1};
put_bucket(BucketF, BucketNo, GridPos, Objs, S) ->
    %% update bucket count
    ?dbg("put_bucket(~p,~n"
	 "           ~p, ~p)~n", [BucketF,BucketNo,Objs]),
    ets:insert(S#state.buckets, {BucketNo,length(Objs)}),
    {file:write_file(BucketF, term_to_binary(Objs)), S}.



bucket_file(Pos, S) ->
    PosStr = if integer(Pos) -> integer_to_list(Pos);
		list(Pos) -> Pos
	     end,
    Filename = filename:join([S#state.dir, 
			      S#state.name ++ ".buckets",
			      PosStr ++ ".bin"]).

new_gridfile(Dir, Name, Options, S0) ->
    Options1 = valid_options(Options),
    S = parse_options(Options, S0#state{dir = Dir, name = Name}),
    TopFile = topfile(S),
    case file:read_file_info(TopFile) of
	{error, enoent} ->
	    write_options(TopFile, S),
	    ok = file:make_dir(filename:join(Dir, Name ++ ".buckets")),
	    S;
	_ ->
	    exit({exists, TopFile})
    end.

valid_options([{dimensions,X}|T]) ->
    [{dimensions,X}|valid_options(T)];
valid_options([{keypos,X}|T]) -> 
    [{keypos,X}|valid_options(T)];
valid_options([{split_threshold,X}|T]) -> 
    [{split_threshold,X}|valid_options(T)];
valid_options([{merge_threshold,X}|T]) -> 
    [{merge_threshold,X}|valid_options(T)];
valid_options([_|T]) ->
    valid_options(T);
valid_options([]) ->
    [].

open_gridfile({open, Dir, Name}, S) ->
    open_gridfile(Dir, Name, S);
open_gridfile({new, Dir, Name, Options}, S) ->
    new_gridfile(Dir, Name, Options, S).

open_gridfile(Dir, Name, S) ->
    TopFile = filename:join(Dir, Name ++ ".bin"),
    case file:read_file(TopFile) of
	{ok, Bin} ->
	    Options = binary_to_term(Bin),
	    parse_options(Options, S#state{dir = Dir,
					   name = Name});
	{error, Reason} ->
	    exit(Reason)
    end.
    
close_gridfile(S) ->
    write_options(S),
    ets:delete(S#state.grid),
    ets:delete(S#state.buckets).

split_direction(S) ->
    Dir = S#state.split_direction,
    Next = next_split_direction(Dir, S#state.dimensions),
    {Dir, S#state{split_direction = Next}}.

next_split_direction(Dim, Dim) -> 1;
next_split_direction(Dim, _) -> Dim+1.
    


      
tab(Type) ->
    ets:new(Type, [set, public]).



req(GF, Req) ->
    case gen_server:call(GF, Req, infinity) of
	{'EXIT', Reason} -> exit(Reason);
	Other -> Other
    end.

parse_options([{dimensions, X}|T], S) ->
    parse_options(T, S#state{dimensions = X});
parse_options([{keypos, X}|T], S) ->
    parse_options(T, S#state{keypos = X});
parse_options([{split_threshold, X}|T], S) ->
    parse_options(T, S#state{split_threshold = X});
parse_options([{merge_threshold, X}|T], S) ->
    parse_options(T, S#state{merge_threshold = X});
parse_options([{next_scale_no, X}|T], S) ->
    parse_options(T, S#state{next_scale_no = X});
parse_options([{split_direction, X}|T], S) ->
    parse_options(T, S#state{split_direction = X});
parse_options([{scales, X}|T], S) ->
    parse_options(T, S#state{scales = X});
parse_options([{next_bucket, X}|T], S) ->
    parse_options(T, S#state{next_bucket = X});
parse_options([{grid, X}|T], S) ->
    initialise_tab(X, S#state.grid),
    parse_options(T, S);
parse_options([{buckets, X}|T], S) ->
    initialise_tab(X, S#state.buckets),
    parse_options(T, S);
parse_options([_|T], S) ->
    parse_options(T, S);
parse_options([], S) ->
    S1 = verify_scales(S).

verify_scales(S) when size(S#state.scales) == S#state.dimensions -> S;
verify_scales(S) ->
    {Scales, S1} = make_scales(S),
    S1#state{scales = Scales}.

make_scales(S) ->
    ScaleL = [scale(No) || No <- lists:seq(1,S#state.dimensions)],
    {list_to_tuple(ScaleL), S#state{next_scale_no = S#state.dimensions+1}}.

scale(No) ->
    {{{'_','_',No},{[],0},{[],0}},1}.

next_scale_no(S) ->
    No = S#state.next_scale_no,
    {No, S#state{next_scale_no = No+1}}.

wild_key_pattern(S) ->
    list_to_tuple(lists:duplicate(S#state.dimensions, '_')).


write_options(S) -> write_options(topfile(S), S).

write_options(TopFile, S) ->
    Opts = [{dimensions, S#state.dimensions},
	    {keypos, S#state.keypos},
	    {split_threshold, S#state.split_threshold},
	    {split_direction, S#state.split_direction},
	    {scales, S#state.scales},
	    {next_bucket, S#state.next_bucket},
	    {grid, ets:tab2list(S#state.grid)},
	    {buckets, ets:tab2list(S#state.buckets)}],
    file:write_file(TopFile, term_to_binary(Opts)).

topfile(S) ->
    filename:join(S#state.dir, S#state.name ++ ".bin").


initialise_tab([H|T], Tab) ->
    ets:insert(Tab, H),
    initialise_tab(T, Tab);
initialise_tab([], _) ->
    ok.



dimension_sort(Dim, Pos, [X]) ->
    element(Dim,element(Pos,X)),  %% error check
    [X];
dimension_sort(Dim, Pos, [])  -> [];
dimension_sort(Dim, Pos, X)   -> split_and_keysort(X, Dim, Pos).
 
split_and_keysort(List, D, P) ->
    L2 = length(List) div 2,
    X = lists:sublist(List, L2),
    Y = lists:nthtail(L2, List),
    keymerge(D,P, dimension_sort(D,P, X), dimension_sort(D,P, Y), []).
 
 
keymerge(D,P, [H1|T1], [H2|T2], L) ->
    K1 = element(D, element(P,H1)),
    K2 = element(D, element(P,H2)),
    if
        K1 > K2 ->
            keymerge(D,P, [H1|T1], T2, [H2|L]);
        true ->
            keymerge(D,P, T1, [H2|T2], [H1|L])
    end;
keymerge(D,P, T1, [H2|T2], L) ->
    keymerge(D,P, T1, T2, [H2|L]);
keymerge(D,P,[H|T], T2, L) ->
    keymerge(D,P,T, T2, [H|L]);
keymerge(D,P, [], [], L) ->
    lists:reverse(L).

