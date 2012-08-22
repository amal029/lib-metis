open Metis

let rec print_node = function
  | Seq (x,y,z) | Join (x,y,z) -> 
    let () = print_string ("Node id: " ^ x) in
    let () = List.iter (fun x -> print_string (" weights: " ^ (string_of_int x) ^ " ")) y in
    print_edge z
  | Split (x,y,z) -> 
    let () = print_string ("Node id: " ^ x) in
    let () = List.iter (fun x -> print_string (" weights: " ^ (string_of_int x) ^ " ")) y in
    List.iter print_edge z
  | Empty -> print_endline "Empty node\n"
and print_edge (Edge(x,y)) =
  let () = print_string ("Edge (w,c): ") in
  let () = (match x with
    | Some x -> print_string ((string_of_int x) ^ "-------") 
    | None -> print_string ("-----")) in
  (* let () = print_endline "\n" in *)
  print_node y

let print node = print_node node
