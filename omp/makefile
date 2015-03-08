include make.inc

EXE= omp_hello.exe omp_workshare1.exe omp_workshare2.exe \
	omp_reduction2.exe omp_mm.exe omp_getEnvInfo.exe \
	omp_reduction.exe  ex1task.exe axpy.exe simple.exe\


all: ${EXE}

%.exe: %.o
	$(CXX) $(CXXFLAGS) $< -o $  $@ 

%.o: %.c
	$(CC) $(CXXFLAGS) -c $< -o $ $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $ $@

.PHONY: clean

clean:
	@-$(RM) *.o $(EXE) *.out *.d

