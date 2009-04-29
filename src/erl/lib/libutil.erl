-module(libutil).
-compile(export_all).


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

string_replace(Src, Needle, Replacement)->
  Tok=string:tokens(Src,Needle),
  Str=lists:flatten([[X, Replacement]||X<-Tok]),
  string:substr(Str,1,length(Str)-length(Replacement)).

trim(Input) ->
   {_,LS,_} = regexp:sub(Input, "^[ \r\n]*", ""),
   {_,RS,_} = regexp:sub(LS, "[ \r\n]*$", ""),
   RS.
