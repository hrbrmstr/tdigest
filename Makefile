#----------------------------------------------------------------------------------------------------
# simple Makefile for T-Digest, relies on cmake to do the actual build.  
# Use CMAKE_LIBRARY_OPTIONS,CMAKE_LIBRARY_SHARED_OPTIONS,CMAKE_LIBRARY_STATIC_OPTIONS or CMAKE_FULL_OPTIONS argument to this Makefile to pass options to cmake.
#----------------------------------------------------------------------------------------------------

CC?=gcc
INFER?=./deps/infer
INFER_DOCKER?=redisbench/infer-linux64:1.0.0
ROOT=$(shell pwd)
SRCDIR := $(ROOT)/src

ifndef CMAKE_LIBRARY_SHARED_OPTIONS
	CMAKE_LIBRARY_SHARED_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=OFF \
		-DENABLE_FRAME_POINTER=ON \
		-DBUILD_TESTS=OFF \
		-DBUILD_BENCHMARK=OFF \
		-DBUILD_EXAMPLES=OFF
endif

ifndef CMAKE_LIBRARY_STATIC_OPTIONS
	CMAKE_LIBRARY_STATIC_OPTIONS=\
		-DBUILD_SHARED=OFF \
		-DBUILD_STATIC=ON \
		-DENABLE_FRAME_POINTER=ON \
		-DBUILD_TESTS=OFF \
		-DBUILD_BENCHMARK=OFF \
		-DBUILD_EXAMPLES=OFF
endif

ifndef CMAKE_LIBRARY_OPTIONS
	CMAKE_LIBRARY_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=ON \
		-DENABLE_FRAME_POINTER=ON \
		-DBUILD_TESTS=OFF \
		-DBUILD_BENCHMARK=OFF \
		-DBUILD_EXAMPLES=OFF
endif

ifndef CMAKE_FULL_OPTIONS
	CMAKE_FULL_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=ON \
		-DENABLE_FRAME_POINTER=ON \
		-DBUILD_TESTS=ON \
		-DBUILD_BENCHMARK=ON \
		-DBUILD_EXAMPLES=ON
endif


ifndef CMAKE_TEST_OPTIONS
	CMAKE_TEST_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=ON \
		-DENABLE_FRAME_POINTER=ON \
		-DBUILD_TESTS=ON \
		-DENABLE_CODECOVERAGE=ON \
		-DBUILD_BENCHMARK=OFF \
		-DBUILD_EXAMPLES=OFF
endif

default: full

# just build the static library. Do not build tests or benchmarks
library_static:
	( cd build ; cmake $(CMAKE_LIBRARY_STATIC_OPTIONS) .. ; $(MAKE) )

# just build the shared library. Do not build tests or benchmarks
library_shared:
	( cd build ; cmake $(CMAKE_LIBRARY_SHARED_OPTIONS) .. ; $(MAKE) )

# just build the static and shared libraries. Do not build tests or benchmarks
library_all:
	( cd build ; cmake $(CMAKE_LIBRARY_OPTIONS) .. ; $(MAKE) )

# just build the static and shared libraries and produce measurements
# of accuracy versus compression factor for fixed data size
# TODO:

# just build the static and shared libraries and tests
unit_tests: 
	( cd build ; cmake $(CMAKE_TEST_OPTIONS) .. ; $(MAKE) ; $(MAKE) test )

test:
	$(MAKE) unit_tests

coverage:
	( cd build ; cmake $(CMAKE_TEST_OPTIONS) .. ; $(MAKE) ; $(MAKE) test; make coverage; )
	
format:
	clang-format -style=file -i $(SRCDIR)/*

lint:
	clang-format -style=file -Werror -n $(SRCDIR)/*

# build all
full:
	( cd build ; cmake $(CMAKE_FULL_OPTIONS) .. ; $(MAKE) )

static-analysis-docker:
	$(MAKE) clean
	docker run -v $(ROOT)/:/RedisBloom/ --user "$(username):$(usergroup)" $(INFER_DOCKER) bash -c "cd RedisBloom && CC=clang infer run --fail-on-issue --biabduction --skip-analysis-in-path ".*rmutil.*"  -- make"

clean: distclean

distclean:
	rm -rf build/* 

bench: full
	$(SHOW) build/tests/histogram_benchmark --benchmark_min_time=10