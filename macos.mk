# macOS toolchain configuration

# Apple Clang through the active Xcode developer tools.
# CXX := xcrun --sdk macosx clang++
ifeq ($(origin CXX),default)
	CXX := clang++
endif

# Add pthread_barrier code since macOS does not have it in its pthread library.
PTHBAR_DIR := $(ROOT_DIR)pthread_barrier_macos
PTHBAR_SRC := $(PTHBAR_DIR)/pthread_barrier.c
PTHBAR_HDR := $(PTHBAR_DIR)/pthread_barrier.h

PTHBAR_OBJ := $(PTHBAR_DIR)/pthread_barrier.o
PTHBAR_LIB := $(PTHBAR_DIR)/libpthread_barrier.a

PTHREAD_CPPFLAGS := -I$(PTHBAR_DIR)

$(PTHBAR_OBJ): $(PTHBAR_SRC) $(PTHBAR_HDR)
	$(CC) $(PTHREAD_CPPFLAGS) $(CFLAGS) -pthread -c $< -o $@

$(PTHBAR_LIB): $(PTHBAR_OBJ)
	ar rcs $@ $^

# find the openmp library installed by Homebrew
OMP_PREFIX := $(shell brew --prefix libomp)

OMP_CXXFLAGS := -Xpreprocessor -fopenmp -I$(OMP_PREFIX)/include
OMP_LDFLAGS  := -L$(OMP_PREFIX)/lib
OMP_LIBS     := -lomp

PTHREAD_LDFLAGS := -L$(PTHBAR_DIR)
PTHREAD_LIBS    := -pthread -lpthread_barrier
