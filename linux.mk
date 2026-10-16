# Linux/PBS toolchain configuration

ifeq ($(origin CXX),default)
    CXX := g++
endif

OMP_CXXFLAGS := -fopenmp
OMP_LDFLAGS  :=
OMP_LIBS     :=

PTHREAD_CPPFLAGS :=
PTHREAD_LDFLAGS :=
PTHREAD_LIBS    := -pthread