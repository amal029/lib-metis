type node =
  | Seq of string * int list * edge
  | Join of string * int list * edge
  | Split of string * int list * edge list
  | Empty
and edge =
  | Edge of int option * node
