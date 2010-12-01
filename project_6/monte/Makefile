PROJECT_NAME = MonteCarlo
# NVCC is path to nvcc. Here it is assumed that /usr/local/cuda is on one's PATH.
NVCC = /usr/local/cuda/bin/nvcc
CUDAPATH = /usr/local/cuda
BUILD_DIR = build
NVCCFLAGS = -I$(CUDAPATH)/include
LFLAGS = -L$(CUDAPATH)/lib64 -lcuda -lcudart -lcurand -lm

# if we run just `make`, will build and then clean.
all: build
# in order to build, we make the build dir, compile kernels, compile host code, and link.
build: build_dir
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o $(BUILD_DIR)/$(PROJECT_NAME) *.cu
build_dir:
	mkdir -p $(BUILD_DIR)
# run `make run` to run project file. provided for simplicity.
run:
	./$(BUILD_DIR)/$(PROJECT_NAME)

