BATTERIES=$(HOME)/batteries-included/_build/src
BATLIBB=batteries.cma
BATLIBBOPT=batteries.cmxa
CC=ocamlc
CCOPT=ocamlopt
METISIDIR=/macports-1.9.1/include/metis/
METISLDIR=/macports-1.9.1/lib
METISLIB=metis
OCAMLDIR=/macports-1.9.1/lib/ocaml
GCC=clang

all: clean metis lib-metis.cma lib-metis.cmxa

metis:
	$(GCC) -O3 -c -I$(METISIDIR) -I$(OCAMLDIR) metis_stubs.c -o metis_stubs.o

lib-metis.cma:
	$(CC) -c metis.ml
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBB) -c metisCodegen.ml
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBB) -c metisParser.ml
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBB) -c metisDriver.ml
	$(CC) -a -custom metisParser.cmo metisCodegen.cmo metisDriver.cmo \
	-I $(METISLDIR) -cclib -l$(METISLIB) -o $@

lib-metis.cmxa:
	$(CCOPT) -c metis.ml
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBBOPT) -c metisCodegen.ml
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBBOPT) -c metisParser.ml
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBBOPT) -c metisDriver.ml
	$(CCOPT) -a metisParser.cmx metisCodegen.cmx metisDriver.cmx \
	-I $(METISLDIR) -cclib -l$(METISLIB) -o $@


clean:
	rm -rf *.cmo *.cmx *.cma *.cmxa *.o *.a *.cmi
