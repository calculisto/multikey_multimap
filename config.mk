DEPENDENCIES_HEADERS=

PROJECT=multikey_multimap
LINK.o=${LINK.cc}
CXXFLAGS+=-std=c++2a -Wall -Wextra $(foreach dir, ${DEPENDENCIES_HEADERS}, -I../${dir})
LDLIBS+= -lfmt

