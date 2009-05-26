%%%----------------------------------------------------------------------
%%% File    : libutil.erl
%%% Author  : Jan Hölscher <jan.hoelscher@esblab.com>
%%% Purpose : Generic Helper
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
-module(libutil).
-compile(export_all).

toString(Val)->
  to_string(Val).
to_string(S) when is_atom(S)->
  atom_to_list(S);
to_string(S) when is_binary(S)->
  binary_to_list(S);
to_string(S) when is_tuple(S)->
  tuple_to_list(S);
to_string(S) when is_integer(S)->
  integer_to_list(S);
to_string(S) ->
  S.

toInteger(Val) when is_atom(Val)->
  toInteger(atom_to_list(Val));
toInteger(Val) when is_binary(Val)->
  toInteger(binary_to_list(Val));
toInteger([]) ->
  0;
toInteger(Val) when is_list(Val)->
  list_to_integer(Val);
toInteger(Val) ->
  Val.

toFloat(Val) when is_atom(Val)->
  toFloat(atom_to_list(Val));
toFloat(Val) when is_binary(Val)->
  toFloat(binary_to_list(Val));
toFloat([]) ->
  0.0;
toFloat(Val) when is_list(Val)->
  list_to_float(Val);
toFloat(Val) ->
  Val.

toNumber(Val) when is_atom(Val)->
  toNumber(atom_to_list(Val));
toNumber(Val) when is_binary(Val)->
  toNumber(binary_to_list(Val));
toNumber([]) ->
  0.0;
toNumber(Val) when is_list(Val)->
  Val1=try list_to_float(Val) of
         Float->Float
       catch
         _:_->Val
       end,
  try list_to_integer(Val1) of
    Int->Int
  catch
    _:_->Val1
  end;
toNumber(Val) ->
  Val.

string_replace(Src, Needle, Replacement)->
  Tok=string:tokens(Src,Needle),
  Str=lists:flatten([[X, Replacement]||X<-Tok]),
  string:substr(Str,1,length(Str)-length(Replacement)).

trim(Input) ->
  {_,LS,_} = regexp:sub(Input, "^[ \r\n]*", ""),
  {_,RS,_} = regexp:sub(LS, "[ \r\n]*$", ""),
  RS.


is_string(String) ->
  Fun = fun(Char) ->
            if 
              is_float(Char)->false;
              Char < 0 -> false;
              Char > 255 -> false;
              true -> true
            end
        end,
  case is_list(String) of
    false -> false;
    true -> lists:all(Fun, String)
  end.

