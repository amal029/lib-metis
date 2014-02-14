LIB=../lib
BATTERIES=`ocamlfind query batteries`
CC=ocamlc
CCOPT=ocamlopt
LIBMETIS=lib-metis.cma
LIBMETISN=lib-metis.cmxa

all: clean util-metis.cma util-metis.cmxa


util-metis.cma:
	ocamlfind $(CC) -I $(LIB) -I $(BATTERIES) $(LIBMETIS) -c metisPrint.ml
	ocamlfind $(CC) -a metisPrint.cmo -o $@ 

util-metis.cmxa:
	ocamlfind $(CCOPT) -I $(LIB) -I $(BATTERIES) $(LIBMETISN) -c metisPrint.ml
	ocamlfind $(CCOPT) -a metisPrint.cmx -o $@ 


clean:
	rm -rf *.cmo *.cmx *.cma *.cmxa *.o *.a *.cmi
