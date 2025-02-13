all:
	g++ -std=c++20 -Iinclude src/main_new.cpp src/lexer.cpp src/parser_new.cpp -o bin/main_new

run:
	./bin/main_new

clean:
	rm -f bin/main_new