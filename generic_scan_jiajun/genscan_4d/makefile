all:scan run

CC=g++
CXXFLAGS += -fopenmp
scan:scan.o genscan.o
	@g++ $(CXXFLAGS) scan.c genscan.c -o scan

run:scan
	@./scan 10

clean:
	@/bin/rm -rf *.o
