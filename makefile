CC=g++ -std=c++17
CFLAGS=-Wall -Wextra -Werror

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	LFLAGS+=-lpthread
endif

all : ant gauss winograd

ant :	ants.cpp mainwindow.cpp s21_graph.cpp s21_graph_algorithms.cpp ants.hpp mainwindow.hpp s21_graph.hpp s21_graph_algorithms.hpp
	$(CC) $(CFLAGS) ants.cpp mainwindow.cpp s21_graph.cpp s21_graph_algorithms.cpp -o ant.out $(LFLAGS)

gauss : gauss.cpp mainwindow.cpp slough.cpp loaderMatrix.cpp gauss.hpp mainwindow.hpp slough.hpp loaderMatrix.hpp
	$(CC) $(CFLAGS) gauss.cpp mainwindow.cpp slough.cpp loaderMatrix.cpp -o gauss.out $(LFLAGS)

winograd : winograd.cpp mainwindow.cpp grape.cpp loaderMatrix.cpp winograd.hpp mainwindow.hpp grape.hpp loaderMatrix.hpp
	$(CC) $(CFLAGS) winograd.cpp mainwindow.cpp grape.cpp loaderMatrix.cpp -o winograd.out $(LFLAGS)

clean :
	$(RM) *.out

rebuild : clean all

cppcheck :
	cppcheck --std=c++17 --enable=all --check-config --suppress=missingIncludeSystem *.cpp *.hpp
