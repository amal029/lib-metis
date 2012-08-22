BATTERIES=$(HOME)/batteries-included/_build/src
BATLIBB=batteries.cma
BATLIBBOPT=batteries.cmxa
CC=ocamlc
CCOPT=ocamlopt

all: clean lib-metis.cma lib-metis.cmxa


lib-metis.cma:
	$(CC) -c metis.ml
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBB) -c metisCodegen.ml
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBB) -c metisParser.ml
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBB) -c metisDriver.ml
	$(CC) -a metisParser.cmo metisCodegen.cmo metisDriver.cmo -o $@

lib-metis.cmxa:
	$(CCOPT) -c metis.ml
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBBOPT) -c metisCodegen.ml
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBBOPT) -c metisParser.ml
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBBOPT) -c metisDriver.ml
	$(CCOPT) -a metisParser.cmx metisCodegen.cmx metisDriver.cmx -o $@


clean:
	rm -rf *.cmo *.cmx *.cma *.cmxa *.o *.a *.cmi
