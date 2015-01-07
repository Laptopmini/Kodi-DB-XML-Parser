# $Id: Makefile,v 1.11 2013-10-15 16:37:56-07 - - $

MKFILE    = Makefile
DEPSFILE  = ${MKFILE}.deps
NOINCLUDE = clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
VALGRIND  = valgrind --leak-check=full --show-reachable=yes

#
# Definitions of list of files:
#
HSOURCES  = astree.h auxlib.h
CSOURCES  = astree.cpp auxlib.cpp main.cpp
ETCSRC    = README ${MKFILE} ${DEPSFILE}
EXECBIN   = parser
ALLCSRC   = ${CSOURCES}
OBJECTS   = ${ALLCSRC:.cpp=.o}
ALLSRC    = ${ETCSRC} ${HSOURCES} ${CSOURCES}
LISTSRC   = ${ALLSRC}

MISCFILES  = ${MKFILE} README
ALLSOURCES = ${HSOURCES} ${CSOURCES} ${MISCFILES}

#
# Definitions of the compiler and compilation options:
#
GCC       = g++ -g -O0 -Wall -Wextra -std=gnu++11
MKDEPS    = g++ -MM -std=gnu++11

#
# The first target is always ``all'', and hence the default,
# and builds the executable images
#
all : ${EXECBIN}

#
# Build the executable image from the object files.
#
${EXECBIN} : ${OBJECTS}
	${GCC} -o${EXECBIN} ${OBJECTS}

#
# Build an object file form a C source file.
#
%.o : %.cpp
	${GCC} -c $<

#
# Make a listing from all of the sources
#
lis : ${LISTSRC} tests
	mkpspdf List.source.ps ${LISTSRC}
	mkpspdf List.output.ps ${REPORTS} \
		${foreach test, ${TESTINS:.in=}, \
		${patsubst %, ${test}.%, in out err}}

#
# Clean and spotless remove generated files.
#
clean :
	- rm ${OBJECTS} ${ALLGENS} ${REPORTS} ${DEPSFILE}
	- rm ${foreach test, ${TESTINS:.in=}, \
		${patsubst %, ${test}.%, out err}}
	- rm *.str *.tok

spotless : clean
	- rm ${EXECBIN} List.*.ps List.*.pdf


#
# Build the dependencies file using the C preprocessor
#
deps : ${ALLCSRC}
	@ echo "# ${DEPSFILE} created `date` by ${MAKE}" >${DEPSFILE}
	${MKDEPS} ${ALLCSRC} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps
	
ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

