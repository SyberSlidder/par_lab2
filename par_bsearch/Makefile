
CC=mpic++

all: exe run 

exe: main.cpp
	${CC} -fopenmp main.cpp -o search

clean: 
	rm search
	
run: search
	ibrun -np 4 ./search 100000 10 200 2 2 6 