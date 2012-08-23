OTHERSDIR=/macports-1.9.1/lib/ocaml/
BATTERIES=$(HOME)/batteries-included/_build/src
BATLIBB=batteries.cma
BATLIBBOPT=batteries.cmxa
METISDIR=$(HOME)/Dropbox/IBM_Work/START_WORK_IRELAND/lib-metis/lib
METISUTILDIR=$(HOME)/Dropbox/IBM_Work/START_WORK_IRELAND/lib-metis/Utils
CC=ocamlc
CCOPT=ocamlopt
LIBMETIS=lib-metis.cma
METISUTIL=util-metis.cma
LIBMETISN=lib-metis.cmxa
METISUTILN=util-metis.cmxa
NUMSLIB=nums.cma
NUMSLIBOPT=nums.cmxa
UNIXLIB=unix.cma
UNIXLIBOPT=unix.cmxa
BIGARRAYLIB=bigarray.cma
BIGARRAYLIBOPT=bigarray.cmxa
METISLDIR=/macports-1.9.1/lib

all: clean test test.native


test:
	$(CC) -I $(METISDIR) -I $(METISUTILDIR) -c parse_metis_file.ml
	$(CC) -I $(METISDIR) -I $(METISUTILDIR) -I $(OTHERSDIR) -I $(BATTERIES) $(NUMSLIB) $(UNIXLIB) $(BIGARRAYLIB) \
	$(BATLIBB) $(LIBMETIS) $(METISUTIL) -I $(METISLDIR) parse_metis_file.cmo -o $@

test.native:
	$(CCOPT) -I $(OTHERSDIR) -I $(METISDIR) -I $(METISUTILDIR) -I	\
	$(BATTERIES) $(BIGARRAYLIBOPT) $(NUMSLIBOPT) $(UNIXLIBOPT)	\
	 $(BATLIBBOPT) $(LIBMETISN) $(METISUTILN) -I $(METISLDIR) parse_metis_file.ml -o $@


clean:
	rm -rf test test.native *.cmi *.cmo *.cmx *.o
