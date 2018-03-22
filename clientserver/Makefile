# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of GNU's libstdc++.
# -g is for debugging.
CPPFLAGS =  -std=c++11 -I.
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += -std=c++11
CXXFLAGS += -g
CXXFLAGS += $(DEPFLAGS)
LDFLAGS   = -g -L.

# Libraries
LDLIBS = -lclientserver
# Targets
PROGS = newsserver newsclient messagehandler
all: libclientserver.a $(PROGS)

# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.
server: newsserver
	./newsserver 3000

client: newsclient
	./newsclient localhost 3000


newsserver: newsserver.o messagehandler.o  article.o newsgroup.o database.o 
newsclient: newsclient.o
libclientserver.a: connection.o server.o
	ar rv libclientserver.a  connection.o server.o
	ranlib libclientserver.a

# Phony targets
.PHONY: all clean distclean

# Standard clean
clean:
	rm -f *.o $(PROGS)

distclean: clean
	-rm *.d libclientserver.a


# Include the *.d files
SRC = $(wildcard *.cc)
-include $(SRC:.cc=.d)
