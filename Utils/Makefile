OTHERSDIR=/macports-1.9.1/lib/ocaml/
BATTERIES=$(HOME)/batteries-included/_build/src
BATLIBB=batteries.cma
BATLIBBOPT=batteries.cmxa
METISDIR=$(HOME)/Dropbox/IBM_Work/START_WORK_IRELAND/lib-metis/lib
CC=ocamlc
CCOPT=ocamlopt
LIBMETIS=lib-metis.cma
LIBMETISN=lib-metis.cmxa
NUMSLIB=nums.cma
NUMSLIBOPT=nums.cmxa
UNIXLIB=unix.cma
UNIXLIBOPT=unix.cmxa
BIGARRAYLIB=bigarray.cma
BIGARRAYLIBOPT=bigarray.cmxa

all: clean util-metis.cma util-metis.cmxa


util-metis.cma:
	$(CC) -I $(METISDIR) -I $(OTHERSDIR) -I $(BATTERIES) $(NUMSLIB) $(UNIXLIB) $(BIGARRAYLIB) \
	$(BATLIBB) $(LIBMETIS) -c metisPrint.ml
	$(CC) -a metisPrint.cmo -o $@

util-metis.cmxa:
	$(CCOPT) -I $(OTHERSDIR) -I $(METISDIR) -I $(BATTERIES) $(BIGARRAYLIBOPT) $(NUMSLIBOPT) $(UNIXLIBOPT) $(BATLIBBOPT) \
	$(LIBMETISN) -c metisPrint.ml
	$(CCOPT) -a metisPrint.cmx -o $@


clean:
	rm -rf *.cmo *.cmx *.cma *.cmxa *.o *.a *.cmi
