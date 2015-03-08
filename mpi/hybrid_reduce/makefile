PLATFORM ?= $(shell hostname -s)
USE_GPU ?= false
CXX_COMPILER ?=icpc
CUDA_LIBS ?= -Xlinker -rpath $(CUDA_PATH)/lib -L/$(CUDA_PATH)/lib -lcudart
CUDA_INC ?= -I$(CUDA_PATH)/include -I$(CUDA_SDK_PATH)/common/inc
MPICXX ?= mpicxx
NVCC ?= nvcc

CXXFLAGS= -g ${OPENMP_FLAGS} -O2

# module load cuda
# module load cuda_SDK
ifeq ($(USE_GPU),true)
CUDA_PATH=$(TACC_CUDA_DIR)
CUDA_SDK_PATH=$(TACC_CUDASDK_DIR)
CUDA_LIBS= -Wl,-rpath,-L${TACC_CUDA_LIB} -L${TACC_CUDA_LIB} -lcudart
MPICXX=mpicxx
endif


ifeq ($(CXX_COMPILER),icpc)
CXXFLAGS+=-openmp
else
CXXFLAGS+=-fopenmp
endif

SRC=mpi_reduce.cpp
OBJ=${SRC:.cpp=.o}
EXE=${SRC:.cpp=.exe}
CUDA_SRC=reduction_kernel.cu gpu_reduce.cpp 
CUDA_OBJ=${CUDA_SRC:.cu=.o}
CUDA_OBJ+=${CUDA_SRC:.cpp=.o}

ifeq ($(USE_GPU),true)
CXXFLAGS+=-DUSE_GPU
CXXFLAGS+=${CUDA_INC}
LDFLAGS+=${CUDA_LIBS}
CUDAFLAGS=-m64 -arch=compute_20 -code=sm_20
OBJ+=${CUDA_OBJ}
endif



all: $(EXE)

mpi_reduce.exe: ${OBJ} 
	${MPICXX} ${CXXFLAGS} ${OBJ} -o mpi_reduce.exe ${LDFLAGS} 

%.o: %.cpp
	${MPICXX} ${CXXFLAGS} -c -o $@ $<

%.o: %.cu
	$(NVCC) $(CUDAFLAGS) $(CUDA_INC) -c -o $@ $<

run:
	export KMP_AFFINITY=scatter; export OMP_NUM_THREADS=4; mpirun -np 2 ./mpi_reduce.exe

clean:
	/bin/rm -rf *.o $(EXE)

