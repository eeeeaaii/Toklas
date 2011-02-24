# the implicit rule for .o:.cpp is 
# $(CXX) -c $(CPPFLAGS) $(CFLAGS)

ARCH := UBUNTU

CXX := g++
CPPFLAGS := -Wall -D$(ARCH)
CFLAGS :=

OBJECTS := ast.o aststatements.o astexpressions.o astunits.o asttyping.o astprogram.o memory.o operations.o symboltable.o type.o
SOURCES := $(OBJECTS:.o=.cpp)

TESTS := 1.tok 2.tok 3.tok 4.tok 5.tok
RESULTS := $(TESTS:.tok=.result)

BACKUPDIR := ../toklasbackup

all: toklas ut


toklas: lexer.o parser.o $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(CFLAGS) main.cpp lexer.o parser.o $(OBJECTS) -o toklas

parser.o: toklas.tab.c toklas.tab.h
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c toklas.tab.c -o parser.o

lexer.o: lex.yy.c toklas.tab.h
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c lex.yy.c -o lexer.o

lex.yy.c: toklas.l toklas.tab.h astprogram.h globals.h
	flex toklas.l

toklas.tab.h: toklas.tab.c

toklas.tab.c: toklas.y astprogram.h symboltable.h 
	bison --report=all,itemset --graph=toklas.dot -d toklas.y

ut: $(RESULTS)

%.result: %.tok
	toklas -V $*.tok  > $*.result
	@if [ -e $*.correct ] ; \
		then diff $*.result $*.correct > $*.errors && (rm $*.errors && echo $*.tok: passed ) || (echo $*.tok: failed) ; \
		else cp $*.result $*.correct && echo $*.tok: replaced ; \
	fi

depend:
	makedepend $(SOURCES)

graph:
	dot -Tpdf toklas.dot -o toklas.pdf
	echo "digraph G {" > tmp1.dot
	makedepend -f tmp1.dot $(SOURCES)
	echo "}" >> tmp1.dot
	cat tmp1.dot | sed "s/#.*//g" > tmp2.dot
	cat tmp2.dot | sed "s/:\(.*\)/ -> subgraph {\1};/g" > tmp3.dot
	cat tmp3.dot | sed "s/\./_/g" > tmp4.dot
	cat tmp4.dot | sed "s/_o/_h/g" > tmp5.dot
	tred tmp5.dot > incdep.dot
	dot -Tpdf incdep.dot -o dependencies.pdf
	rm tmp*.dot

clean:
	-rm toklas *.o toklas.tab.c toklas.tab.h toklas.output lex.yy.c *.errors *.result outf

backup:
	test -a $(BACKUPDIR) || mkdir $(BACKUPDIR)
	-rm -f $(BACKUPDIR)/*   # could also do this by appending "|| true"
	cp *.cpp $(BACKUPDIR)
	cp *.h $(BACKUPDIR)
	cp *.correct $(BACKUPDIR)
	cp *.bak $(BACKUPDIR)
	cp *.tok $(BACKUPDIR)
	cp *.l $(BACKUPDIR)
	cp *.y $(BACKUPDIR)
	cp *.txt $(BACKUPDIR)
	cp *.odg $(BACKUPDIR)
	cp Makefile $(BACKUPDIR)



# DO NOT DELETE THIS LINE -- make depend depends on it
