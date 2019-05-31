compile_lib:./src/*.cpp consui.h
	g++ -std=c++11 -c src/*.cpp -I ./
	ar -cr consui.a *.o
	rm *.o

compile:./consui.a consui.h ./*.cpp
	g++ -std=c++11 -o main ./*.cpp ../consui.a -I ../ -l ncurses

run:
	./main

clean:
	rm *.o
	rm *.a
