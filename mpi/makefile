CXXFLAGS += -O3
MPICC ?= mpicc
MPICXX ?= mpicxx
CXXFLAGS += -openmp

EXE=mpi_hello.exe mpi_pi_reduce.exe mpi_ring_sr.exe ser_pi_calc.exe
SRC=dboard.c
OBJ=$(SRC:.c=.o)
LIBEX=libex.a
LIBS=-lm $(LIBEX)

AR=ar
ARFLAGS=cr
RANLIB=ranlib

all: $(LIBEX) $(EXE)

$(LIBEX): $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)
	$(RANLIB) $@

%.exe: %.o
	$(MPICXX) $(CXXFLAGS) $< -o $  $@  $(LIBS)

%.o: %.c
	$(MPICC) $(CXXFLAGS) -c $< -o $ $@

%.o: %.cpp
	$(MPICXX) $(CXXFLAGS) -c $< -o $ $@

.PHONY: clean

clean:
	@-$(RM) *.o $(EXE) *.out *.d $(LIBEX)

