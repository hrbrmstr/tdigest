
![License](https://img.shields.io/badge/License-MIT-blue.svg)

# T-Digest

Adaptive histogram based on something like streaming k-means crossed with Q-digest.


This implementation is a descendent of Ted MergingDigest, available at:
[https://github.com/tdunning/t-digest/](https://github.com/tdunning/t-digest/)


And contains the work of  Andrew Werner originally available at:
[https://github.com/ajwerner/tdigestc](https://github.com/ajwerner/tdigestc)

## Description

The t-Digest construction algorithm uses a variant of 1-dimensional
k-means clustering to produce a very compact data structure that allows
accurate estimation of quantiles. This t-Digest data structure can be
used to estimate quantiles, compute other rank statistics or even to
estimate related measures like trimmed means. The advantage of the
t-Digest over previous digests for this purpose is that the t-Digest
handles data with full floating point resolution. The accuracy of
quantile estimates produced by t-Digests can be orders of magnitude more
accurate than those produced by previous digest algorithms. Methods are
provided to create and update t-Digests and retreive quantiles from the
accumulated distributions.

See [the original paper by Ted Dunning & Otmar
Ertl](https://arxiv.org/abs/1902.04023) for more details on t-Digests.

## What’s Inside

The following functions are implemented:

  - `td_add`: Add a value to the t-Digest with the specified count
  - `td_create`: Allocate a new histogram
  - `td_reset`: Empty out a histogram and re-initialise it
  - `td_free`: Frees the memory associated with the t-Digest
  - `td_compress`: Re-examines a the t-Digest to determine whether some centroids are redundant
  - `td_merge`: Merge one t-Digest into another
  - `td_cdf`:  Returns the fraction of all points added which are &le; x.
  - `td_quantile`: Returns an estimate of the cutoff such that a specified fraction of the data added to the t-Digest would be less than or equal to the cutoff.
  - `td_size`: Return the number of points that have been added to the t-Digest
  - `td_centroid_count`: Return the number of centroids being used by the t-Digest
  - `td_min`: Get the minimum value from the histogram.  Will return __DBL_MAX__ if the histogram is empty
  - `td_max`: Get the maximum value from the histogram.  Will return __DBL_MIN__ if the histogram is empty

## Build notes

``` 
# Build
git clone https://github.com/filipecosta90/tdigest.git
cd tdigest/
git submodule update --init --recursive
make
```
## Benchmarking

Assuming you've followed the previous build steps, it should be as easy as:
``` 
# Run the benchmark
make bench
```

### Ingestion

#### master
``` c
tdigest$ make bench
build/tests/histogram_benchmark --benchmark_min_time=10
2020-02-13 18:59:18
Running build/tests/histogram_benchmark
Run on (8 X 3900 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 1.73, 1.11, 0.88
------------------------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------------
BM_td_add_uniform_dist/100/10000000   1217132495 ns   1215202532 ns           11 Centroid_Count=76 Total_Compressions=205.048k items_per_second=748.098k/s
BM_td_add_uniform_dist/200/10000000   1338093787 ns   1338023019 ns           11 Centroid_Count=120 Total_Compressions=100.695k items_per_second=679.428k/s
BM_td_add_uniform_dist/300/10000000   1426454139 ns   1426372894 ns           10 Centroid_Count=171 Total_Compressions=60.85k items_per_second=701.079k/s
BM_td_add_uniform_dist/400/10000000   1489910255 ns   1489865942 ns            9 Centroid_Count=218 Total_Compressions=40.927k items_per_second=745.779k/s
BM_td_add_uniform_dist/500/10000000   1541433574 ns   1541231001 ns            9 Centroid_Count=253 Total_Compressions=32.621k items_per_second=720.924k/s
BM_td_add_lognormal_dist/100/10000000 1279235916 ns   1279181475 ns           11 Centroid_Count=75 Total_Compressions=204.968k items_per_second=710.682k/s
BM_td_add_lognormal_dist/200/10000000 1396900530 ns   1396842464 ns           10 Centroid_Count=124 Total_Compressions=91.795k items_per_second=715.9k/s
BM_td_add_lognormal_dist/300/10000000 1432398499 ns   1423146494 ns           10 Centroid_Count=167 Total_Compressions=60.787k items_per_second=702.668k/s
BM_td_add_lognormal_dist/400/10000000 1523148779 ns   1507221068 ns            9 Centroid_Count=207 Total_Compressions=40.828k items_per_second=737.192k/s
BM_td_add_lognormal_dist/500/10000000 1551385694 ns   1551317295 ns            9 Centroid_Count=259 Total_Compressions=32.652k items_per_second=716.237k/s
```

#### perf.improvements branch

``` c
tdigest$ make bench
build/tests/histogram_benchmark --benchmark_min_time=10
2020-02-13 19:03:31
Running build/tests/histogram_benchmark
Run on (8 X 3900 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 1.04, 1.10, 0.94
------------------------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------------
BM_td_add_uniform_dist/100/10000000    620549600 ns    620516429 ns           22 Centroid_Count=56 Total_Compressions=404.425k items_per_second=732.528k/s
BM_td_add_uniform_dist/200/10000000    685883111 ns    685849746 ns           20 Centroid_Count=110 Total_Compressions=182.358k items_per_second=729.023k/s
BM_td_add_uniform_dist/300/10000000    727127573 ns    727096338 ns           19 Centroid_Count=153 Total_Compressions=115.006k items_per_second=723.86k/s
BM_td_add_uniform_dist/400/10000000    732475918 ns    732445725 ns           19 Centroid_Count=190 Total_Compressions=85.779k items_per_second=718.573k/s
BM_td_add_uniform_dist/500/10000000    745449159 ns    745410047 ns           19 Centroid_Count=238 Total_Compressions=68.738k items_per_second=706.076k/s
BM_td_add_lognormal_dist/100/10000000  640487037 ns    640461066 ns           22 Centroid_Count=56 Total_Compressions=404.56k items_per_second=709.716k/s
BM_td_add_lognormal_dist/200/10000000  687814951 ns    687778693 ns           20 Centroid_Count=104 Total_Compressions=182.567k items_per_second=726.978k/s
BM_td_add_lognormal_dist/300/10000000  716653546 ns    716618062 ns           19 Centroid_Count=146 Total_Compressions=114.663k items_per_second=734.444k/s
BM_td_add_lognormal_dist/400/10000000  741198076 ns    741168930 ns           14 Centroid_Count=197 Total_Compressions=63.375k items_per_second=963.729k/s
BM_td_add_lognormal_dist/500/10000000  744030362 ns    742928616 ns           18 Centroid_Count=238 Total_Compressions=65.103k items_per_second=747.791k/s
```

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
