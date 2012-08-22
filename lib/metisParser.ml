open Metis
module List = Batteries.List
module Hashtbl = Batteries.Hashtbl
module String = Batteries.String
module Array = Batteries.Array

exception Internal_compiler_error of string
exception Error of string

let n_map = Hashtbl.create 50

type node =
  | S of string * int list * edge list ref
and edge =
  | E of int option * int option

(* DEBUG *)
(* let print_edge (E(x,y)) =  *)
(*   let () = (match x with *)
(*     | Some x -> print_string (" Edge weight: " ^ (string_of_int x) ^ " ")  *)
(*     | None -> print_string ("1")) in *)
(*   (match y with *)
(*     | Some x -> print_string (" Edge connection:" ^ (string_of_int x) ^ " ")  *)
(*     | None -> print_string "Empty") *)

(* (\* DEBUG *\) *)
(* let print_node (S(x,y,z)) = *)
(*   let () = print_string ("Id: " ^ x) in *)
(*   let () = print_string (" Weights : ") in  *)
(*   let () = List.iter (fun y -> (print_string ((string_of_int y) ^ " "))) y in *)
(*   let () = print_string ("Conn indices : ") in  *)
(*   let () = List.iter print_edge !z in  *)
(*   print_endline "\n" *)

let get_node_weights constraints ss =
  let ss = String.nsplit ss " " in
  List.take constraints ss

let edge_weights = function
  | "001" -> true
  | "011" -> true
  | "101" -> true
  | _ -> false

let rec take_while_even i = function
  | h::t -> if BatInt.modulo i 2 = 0 then 
      let () = IFDEF DEBUG THEN print_endline h ELSE () ENDIF in
      (int_of_string h) :: take_while_even (i+1) t else take_while_even (i+1) t
  | [] -> []

let rec take_while_odd i = function
  | h::t -> if BatInt.modulo i 2 <> 0 then (int_of_string h) :: take_while_odd (i+1) t else take_while_odd (i+1) t
  | [] -> []

let rec filter_all_less_than_me i p counter ews = function
  | h::t -> if h > i then begin p := (List.nth ews counter)::!p; (h :: filter_all_less_than_me i p (counter+1) ews t) end 
    else filter_all_less_than_me i p (counter+1) ews t
  | [] -> []

let rec build_node_edges constraints format nodes i = function
  | h::t -> 
    (* First get the edges only *)
    let () = IFDEF DEBUG THEN print_endline ("***" ^ h ^ "***") ELSE () ENDIF in
    let les = String.nsplit h " " in
    let les = List.drop constraints les in
    let () = IFDEF DEBUG THEN List.iter (fun x -> print_endline (x ^ "////")) les ELSE () ENDIF in
    let (ess,ews) = 
      if edge_weights format then
	(* We need to read the edge weights too!! *)
	let r1 = take_while_even 0 les in
	let r2 = take_while_odd 0 les in
	if (List.length r1 <> List.length r2) then
	  raise (Error ("Node " ^ (string_of_int i) ^ " does not have all its edges and weights specified!!"))
	else (r1,r2)
      else 
	(* Just read the edge indices *)
	(List.map (fun x -> (int_of_string x)) les, []) in
    (* We first need to filter all the arugments from ess and ews, which are smaller than my own index *)
    let () = IFDEF DEBUG THEN print_endline ((string_of_int (List.length ess)) ^ " = " ^ (string_of_int (List.length ews))) ELSE () ENDIF in
    let p = ref [] in
    let ess = filter_all_less_than_me i p 0 ews ess in
    let ews = !p in
    (* let ews = filter_all_less_than_me i ews in *)
    let () = IFDEF DEBUG THEN print_endline ("AFTER REMOVAL " ^ (string_of_int (List.length ess)) ^ " = " ^ (string_of_int (List.length ews))) ELSE () ENDIF in
    let () = IFDEF DEBUG THEN List.iter (fun x -> print_endline ((string_of_int x) ^ "^^^^^^")) ess ELSE () ENDIF in
    (* Finally we can make the edge list to attach to the node *)
    let edges = 
      if ess = [] then
	[E(None,None)]
      else
	if ews = [] then
	  List.map (fun es -> E (None, Some (es-1))) ess
	else 
	  List.map2 (fun es ew -> E (Some ew, Some (es-1))) ess ews in
    (* Finally get the node and set its edge list *)
    let () = nodes.(i-1) <- (match nodes.(i-1) with S (x,y,z) -> S(x,y,(ref edges))) in
    (* Finally do the next one !! *)
    build_node_edges constraints format nodes (i+1) t
  | [] -> ()

let rec build_metis_graph nodes i (S(x,y,z)) =
  (try
     Hashtbl.find n_map i
   with
     | Not_found -> 
       (* Then we need to build it!! *)
       if List.length !z > 1 then
	 let x = string_of_int ((int_of_string x)+1) in
	 let tp = Split (x,y, List.map (fun (E(x,y)) -> 
	   Edge (x,
		   (match y with 
		     | Some i -> build_metis_graph nodes i nodes.(i)
		     | None -> Empty))) !z) in
	 let () = Hashtbl.add n_map i tp in tp
       else 
	 let x = string_of_int ((int_of_string x)+1) in
	 let tp = Seq (x,y, (fun (E(x,y)) -> 
	   Edge (x,
		   (match y with 
		     | Some i -> build_metis_graph nodes i nodes.(i)
		     | None -> Empty))) (List.hd !z)) in
	 let () = Hashtbl.add n_map i tp in tp)

let get_format file = 
  try
    let ifd = open_in file in
    let buffer = ref [] in
    (try
       while true do
	 buffer := (input_line ifd) :: !buffer
       done
     with
       | End_of_file -> close_in ifd);
    let lines = (List.rev !buffer) in
    let lines = List.map String.trim lines in
    (* DEBUG READ IN LINES *)
    let () = IFDEF DEBUG THEN List.iter (fun x -> print_endline x) lines ELSE () ENDIF in
    (* Find out the number of nodes *)
    let info_list = (String.nsplit (List.hd lines) " ") in
    let num_nodes = int_of_string (List.nth info_list 0) in
    (* DEBUG NUMBER OF NODES IN THE GRAPH *)
    let () = IFDEF DEBUG THEN print_endline ("NUM_NODES: " ^ (string_of_int num_nodes)) ELSE () ENDIF in
    (* Find out the format *)
    let format = List.nth info_list 2 in
    (*  DEBUG The FORMAT OF THE FILE *)
    let () = IFDEF DEBUG THEN print_endline ("FORMAT: " ^ format) ELSE () ENDIF in
    (* Find out the number of constraints *)
    let constraints = int_of_string (List.nth info_list 3) in
    (*  DEBUG The CONSTRAINTS OF THE FILE *)
    let () = IFDEF DEBUG THEN print_endline ("CONSTRAINTS: " ^ (string_of_int constraints)) ELSE () ENDIF in
    (* Now parse the file line by line and build the metis graph *)
    let lines = List.tl lines in
    let nodes = Array.init num_nodes (fun i ->
      let wl = get_node_weights constraints (List.nth lines i) in
      S ((string_of_int i),(List.map (fun x -> (int_of_string x)) wl),ref []))  in
    (* DEBUG THE NODES READ *)
    (* let () = IFDEF DEBUG THEN  *)
    (*   Array.iteri (fun i x -> print_string ("Line number " ^ (string_of_int i) ^ "-->" ^ " node is: "); print_node x) nodes  *)
    (*   ELSE () ENDIF in *)
    (* Now we will build the edges between things!! *)
    let () = build_node_edges constraints format nodes 1 lines in
    (* DEBUG THE NODES READ *)
    let () = IFDEF DEBUG THEN 
      Array.iteri (fun i x -> print_string ("Line number " ^ (string_of_int i) ^ "-->" ^ " node is: "); print_node x) nodes 
      ELSE () ENDIF in
  (* Build the metis format *)
  build_metis_graph nodes 0 nodes.(0)
  with
    | Sys_error x -> raise (Error x)
