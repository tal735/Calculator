all: calculator

clean:
	rm main.o helper.o calculator

calculator: main.o helper.o
	g++ -o calculator main.o helper.o

main.o: helper.h Definitions.h Node.h
	g++ -c main.cpp

helper.o: helper.h
	g++ -c helper.cpp

