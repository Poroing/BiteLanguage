main: main.o lexer.o
	clang++ -o main main.o lexer.o

main.o: main.cpp lexer.hpp
	clang++ -std=c++11 -c -o main.o main.cpp

lexer.o: lexer.cpp lexer.hpp
	clang++ -std=c++11 -c -o lexer.o lexer.cpp

clean:
	rm -f main.o lexer.o main
