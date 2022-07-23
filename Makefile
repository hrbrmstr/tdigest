#----------------------------------------------------------------------------------------------------
# simple Makefile for T-Digest, relies on cmake to do the actual build.  
# Use CMAKE_LIBRARY_OPTIONS,CMAKE_LIBRARY_SHARED_OPTIONS,CMAKE_LIBRARY_STATIC_OPTIONS or CMAKE_FULL_OPTIONS argument to this Makefile to pass options to cmake.
#----------------------------------------------------------------------------------------------------

CC?=gcc
INFER?=./deps/infer
INFER_DOCKER?=redisbench/infer-linux64:1.0.0
ROOT=$(shell pwd)
SRCDIR := $(ROOT)/src
TESTDIR := $(ROOT)/tests/unit
BENCHDIR := $(ROOT)/tests/benchmark


ifndef CMAKE_LIBRARY_SHARED_OPTIONS
	CMAKE_LIBRARY_SHARED_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=OFF \
		-DENABLE_CODECOVERAGE=OFF \
		-DBUILD_TESTS=OFF \
		-DBUILD_BENCHMARK=OFF \
		-DBUILD_EXAMPLES=OFF
endif

ifndef CMAKE_LIBRARY_STATIC_OPTIONS
	CMAKE_LIBRARY_STATIC_OPTIONS=\
		-DBUILD_SHARED=OFF \
		-DBUILD_STATIC=ON \
		-DENABLE_CODECOVERAGE=OFF \
		-DBUILD_TESTS=OFF \
		-DBUILD_BENCHMARK=OFF \
		-DBUILD_EXAMPLES=OFF
endif

ifndef CMAKE_LIBRARY_OPTIONS
	CMAKE_LIBRARY_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=ON \
		-DENABLE_CODECOVERAGE=OFF \
		-DBUILD_TESTS=OFF \
		-DBUILD_EXAMPLES=OFF
endif

ifndef CMAKE_FULL_OPTIONS
	CMAKE_FULL_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=ON \
		-DBUILD_TESTS=ON \
		-DBUILD_BENCHMARK=ON \
		-DBUILD_EXAMPLES=ON
endif


ifndef CMAKE_PROFILE_OPTIONS
	CMAKE_PROFILE_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=OFF \
		-DENABLE_CODECOVERAGE=OFF \
		-DBUILD_TESTS=OFF \
		-DBUILD_BENCHMARK=ON \
		-DBUILD_EXAMPLES=OFF \
		-DENABLE_PROFILE=ON
endif


ifndef CMAKE_SANITIZE_OPTIONS
	CMAKE_SANITIZE_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=OFF \
		-DENABLE_CODECOVERAGE=OFF \
		-DBUILD_TESTS=ON \
		-DBUILD_BENCHMARK=OFF \
		-DBUILD_EXAMPLES=OFF \
		-DENABLE_PROFILE=OFF \
		-DENABLE_SANITIZERS=ON
endif

ifndef CMAKE_TEST_OPTIONS
	CMAKE_TEST_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=ON \
		-DBUILD_TESTS=ON \
		-DENABLE_CODECOVERAGE=ON \
		-DBUILD_BENCHMARK=OFF \
		-DBUILD_EXAMPLES=OFF
endif

ifndef CMAKE_BENCHMARK_OPTIONS
	CMAKE_BENCHMARK_OPTIONS=\
		-DBUILD_SHARED=ON \
		-DBUILD_STATIC=OFF \
		-DENABLE_CODECOVERAGE=OFF \
		-DBUILD_TESTS=OFF \
		-DBUILD_BENCHMARK=ON \
		-DBUILD_EXAMPLES=OFF \
		-DENABLE_PROFILE=OFF
endif

default: full

# just build the static library. Do not build tests or benchmarks
library_static:
	( mkdir -p build; cd build ; cmake $(CMAKE_LIBRARY_STATIC_OPTIONS) .. ; $(MAKE) )

# just build the shared library. Do not build tests or benchmarks
library_shared:
	( mkdir -p build; cd build ; cmake $(CMAKE_LIBRARY_SHARED_OPTIONS) .. ; $(MAKE) )

# just build the static and shared libraries. Do not build tests or benchmarks
library_all:
	( mkdir -p build; cd build ; cmake $(CMAKE_LIBRARY_OPTIONS) .. ; $(MAKE) )

# just build the static and shared libraries and produce measurements
# of accuracy versus compression factor for fixed data size
# TODO:

# just build the static and shared libraries and tests
unit_tests: 
	( mkdir -p build; cd build ; cmake $(CMAKE_TEST_OPTIONS) .. ; $(MAKE) ; $(MAKE) test)

test:
	$(MAKE) unit_tests

coverage:
	( mkdir -p build; cd build ; cmake $(CMAKE_TEST_OPTIONS) .. ; $(MAKE) ; $(MAKE) test; make coverage; )
	
format:
	clang-format -style=file -i $(SRCDIR)/*.c
	clang-format -style=file -i $(SRCDIR)/*.h
	clang-format -style=file -i $(TESTDIR)/*.c
	clang-format -style=file -i $(TESTDIR)/*.h
	clang-format -style=file -i $(BENCHDIR)/*.cpp

lint:
	clang-format -style=file -Werror -n $(SRCDIR)/*.c
	clang-format -style=file -Werror -n $(SRCDIR)/*.h
	clang-format -style=file -Werror -n $(TESTDIR)/*.c
	clang-format -style=file -Werror -n $(TESTDIR)/*.h
	clang-format -style=file -Werror -n $(BENCHDIR)/*.cpp

# build all
full:
	( mkdir -p build; cd build ; cmake $(CMAKE_FULL_OPTIONS) .. ; $(MAKE) )

# static-analysis-docker:
# 	$(MAKE) clean
# 	docker run -v $(ROOT)/:/t-digest-c/ --user "$(username):$(usergroup)" $(INFER_DOCKER) bash -c "cd t-digest-c && CC=clang infer run --keep-going --fail-on-issue --biabduction -- make test"

clean: distclean

distclean:
	rm -rf build/* 

sanitize: clean
	( mkdir -p build; cd build ; cmake $(CMAKE_SANITIZE_OPTIONS) .. ; $(MAKE) VERBOSE=1 )
	$(SHOW) build/tests/td_test

profile: clean
	( mkdir -p build; cd build ; cmake $(CMAKE_PROFILE_OPTIONS) .. ; $(MAKE) VERBOSE=1 2> $(basename $@).compiler_stedrr_output.txt )

bench: clean
	( mkdir -p build; cd build ; cmake $(CMAKE_BENCHMARK_OPTIONS) .. ; $(MAKE) VERBOSE=1 )
	$(SHOW) build/tests/histogram_benchmark --benchmark_min_time=5 --benchmark_out=results.json --benchmark_out_format=json

bench-quantile: clean
	( mkdir -p build; cd build ; cmake $(CMAKE_BENCHMARK_OPTIONS) .. ; $(MAKE) VERBOSE=1 )
	$(SHOW) build/tests/histogram_benchmark  --benchmark_min_time=5 --benchmark_filter="BM_td_quantile_lognormal_dist_given_array*|BM_td_quantiles_*"

perf-stat-bench:
	( mkdir -p build; cd build ; cmake $(CMAKE_PROFILE_OPTIONS) .. ; $(MAKE) VERBOSE=1 )
	$(SHOW) perf stat build/tests/histogram_benchmark --benchmark_min_time=10

perf-record-bench: clean
	( mkdir -p build; cd build ; cmake $(CMAKE_PROFILE_OPTIONS) .. ; $(MAKE) VERBOSE=1 )
	$(SHOW) perf record -g -o perf.data.td_add \
		build/tests/histogram_benchmark

perf-report-bench:
	$(SHOW) perf report -g "graph,0.5,caller" -i perf.data.td_add

perf-report-bench-pprof:
	go tool pprof -web perf.data.td_add
