(* This test parses a metis file format and prints out the results *)

let file = "../examples/jacobi2D.grf" in
(* let file = "../examples/matmul.our.grf" in *)
(* let top_node = MetisDriver.parse_metis_file file in *)
(* Print the parsed file !! *)
(* let () = MetisPrint.print top_node in *)
(* Print the adjacency arrays file !! *)
let (nvtxs,ncon,xadj,adjncy,vwgt,vsize,adjwgt) = MetisDriver.get_data file in
let () = print_endline "XADJ ARRAY" in
for i = 0 to Array.length xadj -1 do
  print_int xadj.(i); print_string " ";
done;
print_endline "\n";
let () = print_endline "ADJNCY/ADJWGT ARRAY \n" in
for i = 0 to Array.length adjncy - 1 do
  print_int adjncy.(i); print_string " "; print_int adjwgt.(i); print_string " ";
done;
print_endline "\n";
let () = print_endline "VWGTS" in
for i = 0 to Array.length vwgt -1 do
  print_int vwgt.(i); print_string " ";
done;
print_endline "\n";

(* Convert all to nativeints *)
let nvtxs = Nativeint.of_int nvtxs in
let ncon  = Nativeint.of_int ncon in
let xadj = Array.map Nativeint.of_int xadj in
let adjncy = Array.map Nativeint.of_int adjncy in
let adjwgt = Array.map Nativeint.of_int adjwgt in
let vsize = Array.map Nativeint.of_int vsize in
let vwgt = Array.map Nativeint.of_int vwgt in

(* Lets try doing some real partitoning now!! *)
let (objval,part) = Metis.metis_PartGraphRecursive nvtxs ncon xadj adjncy vwgt vsize adjwgt (Nativeint.of_int 2) [||] [||] in
let () = print_endline (" Objective value: " ^ (Nativeint.to_string objval)) in
for i = 0 to Array.length part - 1 do
  print_endline ("(i:part) = " ^ (string_of_int (i+1)) ^ " : " ^ (Nativeint.to_string part.(i)))
done;

let (objval,part) = Metis.metis_PartGraphKWay nvtxs ncon xadj adjncy vwgt vsize adjwgt (Nativeint.of_int 2) [||] [||] in
let () = print_endline (" Objective value: " ^ (Nativeint.to_string objval)) in
for i = 0 to Array.length part - 1 do
  print_endline ("(i:part) = " ^ (string_of_int (i+1)) ^ " : " ^ (Nativeint.to_string part.(i)))
done;

