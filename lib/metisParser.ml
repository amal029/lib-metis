exception Internal_compiler_error of string
exception Error of string

module List = Batteries.List
module Hashtbl = Batteries.Hashtbl
module String = Batteries.String
module Array = Batteries.Array
module M = Metis

let n_map = Hashtbl.create 50

type node =
  | S of string * int list * edge list ref
and edge =
  | E of int option * int option

let get_node_weights constraints s =
  let ss = String.nsplit ss " " in
  List.take ss (constraints-1)

let edge_weights = function
  | "001" -> true
  | "011" -> true
  | "101" -> true
  | _ -> false

let rec take_while_even i = function
  | h::t -> if i%2 = 0 then (int_of_string h) :: take_while_even (i+1) t else take_while_even (i+1) t
  | [] -> []

let rec take_while_odd i = function
  | h::t -> if i%2 <> 0 then (int_of_string h) :: take_while_even (i+1) t else take_while_even (i+1) t
  | [] -> []

let rec filter_all_less_than_me i = function
  | h::t -> if h > i then h else filter_all_less_than_me i t else filter_all_less_than_me i t
  | [] -> []

let rec build_node_edges constraints format nodes i = function
  | h::t -> 
    (* First get the edges only *)
    let les = String.nsplit h " " in
    let les = List.drop les constraints in
    let (ess,ews) = 
      if edge_weights then
	(* we need to read the edge weights too!! *)
	let r1 = take_while_even 0 les in
	let r2 = take_while_odd 0 les in
	if (List.length r1 <> List.length r2) then
	  raise (Error ("Node " ^ (string_of_int i) ^ " does not have all its edges and weights specified!!"))
	else (r1,r2)
      else 
	(* Just read the edge indices *)
	List.map (fun x -> (int_of_string x) x) les, [] in
    (* We first need to filter all the arugments from ess and ews, which are smaller than my own index *)
    let ess = filter_all_less_than_me i ess in
    let ews = filter_all_less_than_me i ws in
    (* Finally we can make the edge list to attach to the node *)
    let edges = 
      if ews = [] then
	List.map (fun es -> E (None, Some (es-1)))
      else 
	List.map2 (fun es ew -> E (Some ew, Some (es-1))) in
    (* Finally get the node and set its edge list *)
    let () = nodes(i-1) <- (match nodes(i-1) with S (x,y,z) -> S(x,y,edges)) in
    (* Finally do the next one !! *)
    build_node_edges constraints format (i+1) t
  | [] -> ()

let rec build_metis_graph nodes i S(x,y,z) =
  (try
     Hashtbl.find n_map i
   with
     | Not_found -> 
       (* Then we need to build it!! *)
       if !z > 1 then
	 let tp = M.Split (x,!y, List.map (fun E(x,y) -> 
	   M.Edge (x,
		   (match y with 
		     | Some i -> build_metis_graph nodes i nodes.(i)
		     | None -> M.Empty)))) in
	 let () = Hashtbl.add n_map i tp in tp
       else 
	 let tp = M.Seq (x,!y, List.map (fun E(x,y) -> 
	   M.Edge (x,
		   (match y with 
		     | Some i -> build_metis_graph nodes i nodes.(i)
		     | None -> M.Empty)))) in
	 let () = Hashtbl.add n_map i tp in tp)

let get_format file = 
  try
    let ifd = open_in file in
    let buffer = ref [] in
    let lines = 
      (try
	 while true do
	   buffer := (input_line ifd) :: !buffer
	 done
       with
	 | End_of_file -> close_in ifd; List.rev !buffer) in
    (* Find out the number of nodes *)
    let info_list = (String.nsplit (List.hd lines) " ") in
    let num_nodes = int_of_string (List.nth info_list 0) in
    (* Find out the number of edges *)
    let num_edges = int_of_string (List.nth info_list 1) in
    (* Find out the format *)
    let format = List.nth info_list 2 in
    (* Find out the number of constraints *)
    let constraints = int_of_string (List.nth info_list 3) in
    (* Now parse the file line by line and build the metis graph *)
    let nodes = Array.create num_nodes (fun i x -> 
      let wl = get_node_weights x in
      S ((string_of_int i),wl,ref [])) (List.tl lines) in
    (* Now we will build the edges between things!! *)
    let () = build_node_edges constraints format nodes 1 (List.tl lines) in
    (* Build the metis format *)
    build_metis_graph nodes 0 nodes.(0)
  with
    | Sys_error -> print_endline ("Could not open file " ^ file ^ " for reading")
