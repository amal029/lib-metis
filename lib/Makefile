BATTERIES=$(HOME)/batteries-included/_build/src
BATLIBB=batteries.cma
BATLIBBOPT=batteries.cmxa
CC=ocamlc
CCOPT=ocamlopt

all: clean lib-metis.cma lib-metis.cmxa

driver:
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" driver.ml

driver_opt:
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" driver.ml

lib-metis.cma:
	$(CC) -c metis.ml
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBB) -c metisCodegen.ml
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBB) -c metisParser.ml
	$(CC) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBB) -c metisDriver.ml
	$(CC) -a *.cmo -o $@

lib-metis.cmxa:
	$(CCOPT) -c metis.ml
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBBOPT) -c metisCodegen.ml
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBBOPT) -c metisParser.ml
	$(CCOPT) -pp "camlp4o pa_macro.cmo -UDEBUG" -I $(BATTERIES) $(BATLIBBOPT) -c metisDriver.ml
	$(CCOPT) -a *.cmx -o $@


clean:
	rm -rf *.cmo *.cmx *.cma *.cmxa *.o *.a *.cmi
