PLATFORM ?= $(shell hostname -s)
CXXFLAGS += -O3
COMPILER ?=intel
$(info use make COMPILER=gnu or make COMPILER=intel to select compiler)

ifeq (${COMPILER},intel)
CC=icc
CXX=icpc
CXXFLAGS += -openmp -vec_report=6
endif

ifeq (${COMPILER},gnu)
CC=gcc
CXX=g++
CXXFLAGS += -fopenmp
endif