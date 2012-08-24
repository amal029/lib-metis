(* This test parses a metis file format and prints out the results *)

let file = "../examples/jacobi2D.our.grf" in
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
