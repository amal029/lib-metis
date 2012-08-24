(* Declaring the external functions *)

external metis_PartGraphRecursive : nativeint->nativeint->nativeint array->nativeint array->nativeint array->nativeint array->nativeint array->nativeint-> float array -> nativeint array -> (nativeint * nativeint array) = "Metis_PartGraphRecursiveBytecode" "Metis_PartGraphRecursiveNative"

external metis_PartGraphKWay : nativeint->nativeint->nativeint array->nativeint array->nativeint array->nativeint array->nativeint array->nativeint-> float array -> nativeint array -> (nativeint * nativeint array) = "Metis_PartGraphKWayBytecode" "Metis_PartGraphKWayNative"

external metis_SetDefaultOptions : nativeint array -> unit = "Metis_SetDefaultOptions"

external metis_PartMeshDual: nativeint->nativeint->nativeint array->nativeint array->nativeint array->nativeint array->nativeint->nativeint->float array-> (nativeint*nativeint array * nativeint array) = "Metis_PartMeshDualByteCode" "Metis_PartMeshDualNative"

external metis_PartMeshNodal: nativeint->nativeint->nativeint array->nativeint array->nativeint array->nativeint array->nativeint->float array-> (nativeint*nativeint array * nativeint array) = "Metis_PartMeshNodalByteCode" "Metis_PartMeshNodalNative"

external metis_NodeND: nativeint -> nativeint array -> nativeint array -> nativeint array -> (nativeint array * nativeint array) 
  = "Metis_NodeND"


type node =
  | Seq of string * int list * edge
  | Join of string * int list * edge
  | Split of string * int list * edge list
  | Empty
and edge =
  | Edge of int option * node
