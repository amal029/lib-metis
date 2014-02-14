BATTERIES=`ocamlfind query batteries`
METISDIR=../lib
METISUTILDIR=../Utils
CC=ocamlc
CCOPT=ocamlopt
LIBMETIS=lib-metis.cma
METISUTIL=util-metis.cma
LIBMETISN=lib-metis.cmxa
METISUTILN=util-metis.cmxa
METISSTUB=../lib/metis_stubs.o

all: clean test test.native


test:
	$(CC) -I $(METISDIR) -I $(METISUTILDIR) -c parse_metis_file.ml
	ocamlfind $(CC) -custom -I $(METISDIR) -I $(METISUTILDIR)	\
	$(LIBMETIS) $(METISUTIL) $(METISSTUB) parse_metis_file.cmo -o	\
	$@ -linkpkg -package batteries

test.native:
	ocamlfind $(CCOPT) -I $(METISDIR) -I $(METISUTILDIR)		\
	$(LIBMETISN) $(METISUTILN) $(METISSTUB) parse_metis_file.ml -o	\
	$@ -linkpkg -package batteries


clean:
	rm -rf test test.native *.cmi *.cmo *.cmx *.o
