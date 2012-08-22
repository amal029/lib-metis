(* This test parses a metis file format and prints out the results *)

let top_node = MetisDriver.parse_metis_file "../examples/jacobi2D.grf" in
(* Print the parsed file !! *)
MetisPrint.print top_node
