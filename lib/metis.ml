(* Declaring the external functions *)

external metis_PartGraphRecursive : nativeint->nativeint->nativeint array->nativeint array->nativeint array->nativeint array->nativeint array->nativeint-> float array -> nativeint array -> (nativeint * nativeint array) = "Metis_PartGraphRecursiveBytecode" "Metis_PartGraphRecursiveNative"
external metis_PartGraphRecursive : nativeint->nativeint->nativeint array->nativeint array->nativeint array->nativeint array->nativeint array->nativeint-> float array -> nativeint array -> (nativeint * nativeint array) = "Metis_PartGraphKWayBytecode" "Metis_PartGraphKWayNative"

type node =
  | Seq of string * int list * edge
  | Join of string * int list * edge
  | Split of string * int list * edge list
  | Empty
and edge =
  | Edge of int option * node
