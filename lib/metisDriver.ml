open Metis
let generate_metis_file constraints format file node = MetisCodegen.process constraints format file node
let parse_metis_file file = MetisParser.get_format file
let get_data file = MetisAdjacency.process file
