open Buffer
open Metis

exception Internal_compiler_error of string

module List = Batteries.List
module Hashtbl = Batteries.Hashtbl

let num_nodes = ref 0
let num_edges = ref 0
let format = "011"
let constraints = "1"

(* The main list *)
let ml = ref []

(* The parent child associative map *)
let cp = Hashtbl.create 50
(* let nums = Hashtbl.create 50 *)

(* The buffer that holds the metis connections *)
let buffer = create 80

let get_edge_child = function | Edge (_,x) -> x
let get_edge_weight = function | Edge (w,_) -> match w with Some x -> x | None -> 1

let update_num_edge = function 
  | Edge(_,x) -> 
    match x with 
      | Empty -> ()
      | _ -> num_edges := !num_edges + 1

(* Debug code *)
let print_list = function
  | Seq (_,x,y,_) -> print_endline ("Seq" ^ " " ^ (string_of_int x) ^ " " ^ (string_of_int y))
  | Join (_,x,y,_) -> print_endline ("Join" ^ " " ^ (string_of_int x) ^ " " ^ (string_of_int y))
  | Split (_,x,y,_) -> print_endline ("Split" ^ " " ^ (string_of_int x) ^ " " ^ (string_of_int y))
  | Empty -> print_endline "empty"

let get_node_num node =
  match List.index_of node (List.rev !ml) with
    | Some x -> 
      (* DEBUG *)
      let () = IFDEF DEBUG THEN print_endline ("Printing : " ^ (string_of_int x)) ELSE () ENDIF in
      string_of_int (x+1) (* (Hashtbl.find nums node) *)
    | None -> (match node with 
	| Empty -> " "
	| _ -> raise (Internal_compiler_error "Cannot associate a node num !!"))

let print_edge_connections i = function
  | Edge(w,x) ->
    let w = (match w with None -> 1 | Some x -> x) in
    match x with
      | Empty -> ()
      | _ -> add_string buffer (i ^ " " ^ (string_of_int w) ^ " ")

let add_to_hash parent = function
  | Edge (_,x) as s -> 
    let ps2 = (match parent with | Seq(st,_,_,_) | Split (st,_,_,_) | Join (st,_,_,_) -> st | Empty -> "") in
    let ps = (match x with | Seq(st,_,_,_) | Split (st,_,_,_) | Join (st,_,_,_) -> st | Empty -> "") in
    let () = IFDEF DEBUG THEN print_endline ("Attaching parent: " ^ ps2)  ELSE () ENDIF in
    let () = IFDEF DEBUG THEN print_endline ("Child is:: " ^ ps)  ELSE () ENDIF in
    let allbs = Hashtbl.find_all cp x in
    let parents = List.map (fun (p,_) -> p) allbs in
    let doit = List.exists ((=) parent) parents in
    if not doit then 
      (match x with | Empty -> () | _ -> Hashtbl.add cp x (parent, s))
    else ()

let rec fill_parents = function
  | Split (_,_,_,l) as s -> let () = List.iter (add_to_hash s) l in List.iter (fun x -> fill_parents (get_edge_child x)) l
  | Seq (_,_,_,e) | Join (_,_,_,e) as s -> let () = add_to_hash s e in fill_parents (get_edge_child e)
  | Empty -> ()

let print_nums = List.iteri (fun i x -> (match x with
  | Seq (s,_,_,_) -> print_endline ("Seq " ^ s ^"---,---" ^ (string_of_int (i+1)))
  | Split (s,_,_,_) -> print_endline ("Split " ^ s ^"---,---" ^ (string_of_int (i+1)))
  | Join (s,_,_,_) -> print_endline ("Join " ^ s ^"---,---" ^ (string_of_int (i+1)))
  | _ -> ()))

let rec fill_nums = function
  | Split (_,_,_,l) as s -> 
    ml := s :: !ml;
    List.iteri (fun i x -> fill_nums (get_edge_child x)) l

  | Seq (_,_,_,e) as s ->
    (try
       let lls = Hashtbl.find_all cp s in
       let () = List.iter (fun (parent,_) -> ignore (List.find ((=) parent) !ml)) lls in
       ml := s :: !ml;
       fill_nums (get_edge_child e)
     with
       | Not_found -> ())

  | Join (st,_,_,e) as s ->
    (* Don't out it in if there already is one in there !! *)
    (try
       let lls = Hashtbl.find_all cp s in
       let () = List.iter (fun (parent,_) -> ignore (List.find ((=)parent) !ml)) lls in
       ml := s :: !ml;
       fill_nums (get_edge_child e)
     with
       | Not_found -> ())
  | Empty -> ()

let rec build_metis_file = function
  | Split (st,w1,w2,edge_list) as s -> 
    let () = IFDEF DEBUG THEN print_endline ("got a split node" ^ " " ^ st) ELSE () ENDIF in
    (* First increment the num_nodes *)
    num_nodes := !num_nodes + 1;
    (* Next increment the number of edges *)
    let () = List.iter update_num_edge edge_list in
    (* Next append your own weights *)
    let w2 = if w2 = 0 then 1 else w2 in
    let () = add_string buffer ((string_of_int (w1*w2)) ^ " ") in
    (* Next add the edges and their weights from this thing *)
    let () = List.iter (fun x -> print_edge_connections (get_node_num (match x with | Edge (_,x) -> x)) x) edge_list in 
    (* Make the edges for your own parents *)
    (try
       let (parent,edge) = Hashtbl.find cp s in
       (* Get parents node_num *)
       (match parent with
	 | Empty -> add_string buffer "\n"
	 | _ -> let pnum = get_node_num parent in 
		print_edge_connections pnum edge; 
		add_string buffer "\n")
     with
       | Not_found -> raise (Internal_compiler_error " I don't have a parent!!"))

  | Empty -> 
    let () = IFDEF DEBUG THEN print_endline "got an empty node" ELSE () ENDIF in
    ()

  | Join (st,w1,w2,edge) as s -> 
    let () = IFDEF DEBUG THEN print_endline ("got a Join node " ^ st) ELSE () ENDIF in
    (* First increment the num_nodes *)
    num_nodes := !num_nodes + 1;
    (* Next increment the number of edges *)
    let () = update_num_edge edge in
    (* Next append your own weights *)
    let w2 = if w2 = 0 then 1 else w2 in
    let () = add_string buffer ((string_of_int (w1*w2)) ^ " ") in
    (* Next add the edges and their weights from this thing *)
    let () = print_edge_connections (get_node_num (match edge with | Edge (_,x) -> x)) edge in 
    (* Make the edges for your own parents *)
    (try
       let lls = Hashtbl.find_all cp s in
       (* Get parents node_num *)
       let () = List.iter (fun (parent,edge) ->
	 (match parent with
	   | Empty -> add_string buffer "\n"
	   | _ -> let pnum = get_node_num parent in
		  let () = print_edge_connections pnum edge in ())) lls in add_string buffer "\n"
     with
       | Not_found -> raise (Internal_compiler_error " I don't have a parent!!"))

  | Seq (st,w1,w2,edge) as s -> 
    let () = IFDEF DEBUG THEN print_endline ("got a Seq node " ^ st) ELSE () ENDIF in
    (* First increment the num_nodes *)
    num_nodes := !num_nodes + 1;
    (* Next increment the number of edges *)
    let () = update_num_edge edge in
    (* Next append your own weights *)
    let w2 = if w2 = 0 then 1 else w2 in
    let () = add_string buffer ((string_of_int (w1*w2)) ^ " ") in
    (* Next add the edges and their weights from this thing *)
    let () = print_edge_connections (get_node_num (match edge with | Edge (_,x) -> x)) edge in 
    (* Make the edges for your own parents *)
    (try
       let lls = Hashtbl.find_all cp s in
       let () = IFDEF DEBUG THEN print_endline (string_of_int (List.length lls)) ELSE () ENDIF in
	(* Get parents node_num *)
       let () = List.iter (fun (parent,edge) ->
	 (match parent with
	   | Empty -> add_string buffer "\n"
	   | _ -> let pnum = get_node_num parent in
		  let () = print_edge_connections pnum edge in ())) lls in add_string buffer "\n"
     with
       | Not_found -> raise (Internal_compiler_error " I don't have a parent!!"))

let process file top_node = 
  (* Add a dummy parent for the top_node *)
  let () = Hashtbl.add cp top_node (Empty, Edge(None,Empty)) in
  let () = fill_parents top_node in
  let () = fill_nums top_node in
  let () = IFDEF DEBUG THEN print_nums (List.rev !ml) ELSE () ENDIF in
  let () = List.iter build_metis_file (List.rev !ml) in
  (* Now append the top line to a buffer and concate the 2 buffers *)
  let b1 = create 80 in
  let () = add_string b1 ((string_of_int !num_nodes) ^ " " ^ (string_of_int !num_edges) ^ " " ^ format ^ " " ^ constraints ^ "\n") in
  let () = add_buffer b1 buffer in
  let ochan = open_out file in
  (* let ochan = BatIO.output_channel ochan ~cleanup:true in *)
  let () = output_buffer ochan b1 in
  ignore (close_out ochan)
