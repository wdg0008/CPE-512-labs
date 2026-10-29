# Linux/PBS toolchain configuration

INTEL ?=

ifeq ($(INTEL), 1)
    CXX := icpx
    OMP_CXXFLAGS := -qopenmp
else # ifeq ($(origin CXX),default)
    CXX := g++
    OMP_CXXFLAGS := -fopenmp
endif

OMP_LDFLAGS  :=
OMP_LIBS     :=

PTHREAD_CPPFLAGS :=
PTHREAD_LDFLAGS :=
PTHREAD_LIBS    := -pthread
