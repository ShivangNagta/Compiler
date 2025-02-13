all:
	g++ -std=c++20 -Iinclude  main.cpp lexer.cpp -o bin/main

clean:
	rm -f bin/main