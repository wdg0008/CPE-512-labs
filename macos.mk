# macOS toolchain configuration

# Apple Clang through the active Xcode developer tools.
ifeq ($(origin CXX),default)
    #CXX := xcrun --sdk macosx clang++
    CXX := clang++
endif

# find the openmp library installed by Homebrew
OMP_PREFIX := $(shell brew --prefix libomp)

OMP_CXXFLAGS := -Xpreprocessor -fopenmp -I$(OMP_PREFIX)/include
OMP_LDFLAGS  := -L$(OMP_PREFIX)/lib
OMP_LIBS     := -lomp

PTHREAD_CXXFLAGS := -pthread
PTHREAD_LDFLAGS  :=
