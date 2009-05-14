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
toInteger(Val) when is_list(Val)->
  list_to_integer(Val);
toInteger(Val) ->
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

