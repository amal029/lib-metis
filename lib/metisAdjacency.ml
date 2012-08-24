exception Error of string
exception Internal_compiler_error of string

module List = Batteries.List
module Array = Batteries.Array
module String = Batteries.String

let edge_weights = function
  | "001" -> true
  | "011" -> true
  | "101" -> true
  | _ -> false

let rec take_while_even i = function
  | h::t -> if BatInt.modulo i 2 = 0 then 
      (int_of_string h) :: take_while_even (i+1) t else take_while_even (i+1) t
  | [] -> []

let rec take_while_odd i = function
  | h::t -> if BatInt.modulo i 2 <> 0 then (int_of_string h) :: take_while_odd (i+1) t else take_while_odd (i+1) t
  | [] -> []


let rec get_adjncy_array con_start counter constraints format adjncy xadj vwgt adjwgt = function
  | h::t -> 
    let node_info = (String.nsplit h " ") in
    let cons = List.take constraints node_info in
    let node_info = List.drop constraints node_info in
    let () = List.iteri (fun i x -> vwgt.(i+con_start) <- int_of_string x) cons in
    let (edges,edge_ws) = 
      if edge_weights format then (take_while_even 0 node_info, take_while_odd 0 node_info)
      else (List.map int_of_string node_info, List.make (List.length node_info) 0) in
    let () = IFDEF DEBUG THEN List.iter (fun x -> print_string ((string_of_int x))) edges ELSE () ENDIF in
    let () = IFDEF DEBUG THEN print_endline "\n*******" ELSE () ENDIF in
    let () = xadj.(counter) <- (xadj.(counter-1)+List.length edges) in
    let () = List.iteri (fun i x -> adjncy.(i+xadj.(counter-1)) <- x) edges in
    let () = List.iteri (fun i x -> adjwgt.(i+xadj.(counter-1)) <- x) edge_ws in
    let () = IFDEF DEBUG THEN for i = 0 to counter do print_int xadj.(i); print_string " " done; print_endline "\n"; () ELSE () ENDIF in
    let () = IFDEF DEBUG THEN for i = 0 to xadj.(counter)-1 do print_int adjncy.(i); print_string " " done; print_endline "\n"; () ELSE () ENDIF in
    get_adjncy_array (con_start+List.length cons) (counter+1) constraints format adjncy xadj vwgt adjwgt t
  | [] -> ()

let process file = 
  try
    let ifd = open_in file in
    let buffer = ref [] in
    (try
       while true do
	 buffer := (input_line ifd) :: !buffer
       done
     with
       | End_of_file -> close_in ifd);
    (* Get only the lines *)
    let lines = (List.rev !buffer) in
    let info_list = (String.nsplit (List.hd lines) " ") in
    let num_nodes = int_of_string (List.nth info_list 0) in
    let num_edges = int_of_string (List.nth info_list 1) in
    let format = List.nth info_list 2 in
    let constraints = int_of_string (List.nth info_list 3) in
    let lines = List.map String.trim (List.tl lines) in
    let adjncy = Array.make (2*num_edges) 0 in
    let xadj = Array.make (num_nodes+1) 0 in
    let vwgt  = Array.make (num_nodes * constraints) 0 in
    let adjwgt = if edge_weights format then Array.make (num_edges*2) 1 else [||] in
    let () = IFDEF DEBUG THEN print_endline ("XADJ SIZE: " ^ (string_of_int (Array.length xadj)) ^ " ADJNCY SIZE: " ^ (string_of_int(Array.length adjncy))) ELSE () ENDIF in
    let () = get_adjncy_array 0 1 constraints format adjncy xadj vwgt adjwgt lines in 
    (num_nodes,constraints,xadj,adjncy,vwgt,[||],adjwgt)
  with
    | Sys_error x -> raise (Error x)
    | _ as s -> raise s
