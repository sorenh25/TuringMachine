main: TM.o main.cpp
	g++ -Wall main.cpp TM.o -std=gnu++11 -static-libstdc++ -o TM-teamSorenHolm

TM.o: TM.cpp TM.hpp
	g++ TM.cpp -Wall -c -std=gnu++11 -static-libstdc++

run: main
	TM-teamSorenHolm testFiles/TM1.txt testFiles/TM1-tape.txt

clean:
	rm TM-teamSorenHolm TM.o
