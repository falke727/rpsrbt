# rbtrie をつくる Makefile
VPATH = src include
CPPFLAGS = -I include
#INCLUDE = -I include

#CXX = g++ -O3 -Wall -Wextra -pedantic -std=c++0x -lm  # O は最適化用 
CXX = g++ -g3 -Wall -Wextra -pedantic -std=c++0x -lm 

rbtrie: main.o openfile.o rpsrbt.o classify.o
#rbtrie: rbtrie.o openfile.o trie.o classify.o check.o measure.o
#	g++ -g3 -Wall -Wextra -pedantic -std=c++0x -lm main.o openfile.o classify.o rbt.o check.o -o main
	g++ -O3 -Wall -Wextra -pedantic -std=c++0x -lm main.o openfile.o rpsrbt.o classify.o -o main

clean:
	rm -rf *.o *.dSYM
