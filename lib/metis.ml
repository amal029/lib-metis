type node =
  | Seq of string * int * int * edge
  | Join of string * int * int * edge
  | Split of string * int * int * edge list
  | Empty
and edge =
  | Edge of int option * node
