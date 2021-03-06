# $Id: Makefile,v 1.5 2014-11-17 03:23:11-08 - - $

MKFILE = Makefile
DEPSFILE = ${MKFILE}.deps
NOINCLUDE = cid clean spotless
NEEDINCL = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
VALGRIND = valgrind --leak-check=full --show-reachable=yes

#
## Definitions of list of files:
#
HSOURCES  = astree.h lyutils.h  auxlib.h  stringset.h 
CSOURCES  = astree.cpp lyutils.cpp auxlib.cpp stringset.cpp main.cpp
LSOURCES  = scanner.l
YSOURCES  = parser.y
ETCSRC    = README ${MKFILE} ${DEPSFILE}
CLGEN     = yylex.cpp
HYGEN     = yyparse.h
CYGEN     = yyparse.cpp
CGENS     = ${CYGEN} ${CLGEN}
ALLGENS   = ${CGENS}
EXECBIN   = oc
ALLCSRC   = ${CGENS} ${HYGEN} ${CSOURCES}
OBJECTS   = ${ALLCSRC:.cpp=.o}
LREPORT   = yylex.output
YREPORT   = yyparse.output
IREPORT   = ident.output
REPORTS   = ${LREPORT} ${YREPORT} ${IREPORT}
ALLSRC    = ${ETCSRC} ${YSOURCES} ${LSOURCES} ${HSOURCES} ${CSOURCES}
TESTINS   = ${wildcard test?.in}
LISTSRC   = ${ALLSRC} ${HYGEN}

GPP   = g++ -g -O0 -Wall -Wextra -std=gnu++11

all : ${EXECBIN}

#
## Build the executable image from the object files.
#
${EXECBIN} : ${OBJECTS}
	${GPP} -o ${EXECBIN} ${OBJECTS}

#
# Build an object file form a C++ source file.
#
%.o : %.cpp
	${GPP} -c $<

#
## Build the parser.
#
${CYGEN} ${HYGEN} : ${YSOURCES}
	bison --report=state  --defines=${HYGEN} --output=${CYGEN} ${YSOURCES}

#
## Build the scanner.
#
${CLGEN} : ${LSOURCES}
	flex --outfile=${CLGEN} ${LSOURCES} 2>${LREPORT}
	- grep -v '^  ' ${LREPORT}

#
#Check sources into an RCS subdirectory.
#
ci : ${ALLSRC}
	cid + ${ALLSRC}

#
## Make a listing from all of the sources
#
lis : ${LISTSRC} tests
	mkpspdf List.source.ps ${LISTSRC}
	mkpspdf List.output.ps ${REPORTS} \
		${foreach test, ${TESTINS:.in=}, \
		${patsubst %, ${test}.%, in out err}}

#
## Clean and spotless remove generated files.
#
clean :
	- rm ${OBJECTS} ${ALLGENS} ${EXECBIN} *.output  *.str *.tok *.ast 

spotless : clean
	- rm ${EXECBIN} List.*.ps List.*.pdf


#
## Build the dependencies file using the C preprocessor
#
deps : ${ALLCSRC}
	@ echo "# ${DEPSFILE} created `date` by ${MAKE}" >${DEPSFILE}
	${MKDEPS} ${ALLCSRC} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

test: ${EXECBIN}
	   touch ${TESTINS}
		make --no-print-directory ${TESTINS:.in=.out}

tests : ${EXECBIN}
	touch ${TESTINS}
	make --no-print-directory ${TESTINS:.in=.out}

%.out %.err : %.in ${EXECBIN}
	( ${VALGRIND} ${EXECBIN} -ly -@@ $< \
	;  echo EXIT STATUS $$? 1>&2 \
	) 1>$*.out 2>$*.err

#
## Everything
#
again :
	gmake --no-print-directory spotless deps ci all lis
	
ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

