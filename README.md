
![License](https://img.shields.io/badge/License-MIT-blue.svg)
[![CircleCI](https://circleci.com/gh/RedisBloom/t-digest-c.svg?style=svg)](https://circleci.com/gh/RedisBloom/t-digest-c)
[![codecov](https://codecov.io/gh/RedisBloom/t-digest-c/branch/master/graph/badge.svg)](https://codecov.io/gh/RedisBloom/t-digest-c)

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
git clone https://github.com/RedisBloom/t-digest-c.git
cd t-digest-c/
git submodule update --init --recursive
make
```

## Testing 
Assuming you've followed the previous build steps, it should be as easy as:
``` 
# Run the unit tests
make test
```

## Benchmarking

Assuming you've followed the previous build steps, it should be as easy as:
``` 
# Run the benchmark
make bench
```

### Ingestion

#### master branch (~121ns/iter for compression 100)
``` 
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

#### [perf.improvements](https://github.com/filipecosta90/tdigest/tree/perf.improvements) branch (~71ns/iter for compression 100)
Making usage of naive quick sort with array of primary data-types instead of array of centroids ( struct ). Naive since it starts always at the beginning and the initial centroids are already sorted.
```
2020-02-14 14:51:03
Running build/tests/histogram_benchmark
Run on (8 X 3900 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 1.50, 1.02, 0.96
------------------------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------------
BM_td_add_uniform_dist/100/10000000    718939047 ns    718903714 ns           20 Centroid_Count=69 Total_Compressions=370.58k items_per_second=695.503k/s
BM_td_add_uniform_dist/200/10000000    804681013 ns    804650949 ns           17 Centroid_Count=114 Total_Compressions=155.468k items_per_second=731.044k/s
BM_td_add_uniform_dist/300/10000000    891044330 ns    891005893 ns           16 Centroid_Count=163 Total_Compressions=97.161k items_per_second=701.454k/s
BM_td_add_uniform_dist/400/10000000   1033516962 ns   1033465854 ns           14 Centroid_Count=203 Total_Compressions=63.638k items_per_second=691.156k/s
BM_td_add_uniform_dist/500/10000000   1095685997 ns   1095635532 ns           13 Centroid_Count=251 Total_Compressions=47.155k items_per_second=702.086k/s
BM_td_add_lognormal_dist/100/10000000  736637497 ns    735809155 ns           18 Centroid_Count=66 Total_Compressions=332.592k items_per_second=755.027k/s
BM_td_add_lognormal_dist/200/10000000  807975314 ns    807936447 ns           17 Centroid_Count=114 Total_Compressions=155.149k items_per_second=728.071k/s
BM_td_add_lognormal_dist/300/10000000  900674982 ns    900632289 ns           16 Centroid_Count=160 Total_Compressions=97.167k items_per_second=693.957k/s
BM_td_add_lognormal_dist/400/10000000  964961965 ns    964920578 ns           15 Centroid_Count=207 Total_Compressions=68.071k items_per_second=690.903k/s
BM_td_add_lognormal_dist/500/10000000 1078639324 ns   1078597549 ns           14 Centroid_Count=249 Total_Compressions=50.766k items_per_second=662.236k/s
```

#### [perf.qsort.central](https://github.com/filipecosta90/tdigest/tree/perf.qsort.central) branch (~60ns/iter for compression 100)
Making usage of quick sort but with central pivot
```
build/tests/histogram_benchmark --benchmark_min_time=10
2020-02-14 15:01:48
Running build/tests/histogram_benchmark
Run on (8 X 3900 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 0.78, 0.80, 0.89
------------------------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------------
BM_td_add_uniform_dist/100/10000000    607977623 ns    607950826 ns           23 Centroid_Count=69 Total_Compressions=425.081k items_per_second=715.161k/s
BM_td_add_uniform_dist/200/10000000    652122141 ns    652103931 ns           21 Centroid_Count=117 Total_Compressions=192.466k items_per_second=730.237k/s
BM_td_add_uniform_dist/300/10000000    675920388 ns    675903292 ns           21 Centroid_Count=158 Total_Compressions=127.208k items_per_second=704.525k/s
BM_td_add_uniform_dist/400/10000000    693811791 ns    693769234 ns           20 Centroid_Count=202 Total_Compressions=90.646k items_per_second=720.701k/s
BM_td_add_uniform_dist/500/10000000    715648500 ns    715058842 ns           20 Centroid_Count=243 Total_Compressions=72.365k items_per_second=699.243k/s
BM_td_add_lognormal_dist/100/10000000  616520092 ns    616500798 ns           22 Centroid_Count=70 Total_Compressions=407.848k items_per_second=737.299k/s
BM_td_add_lognormal_dist/200/10000000  659716761 ns    659695213 ns           21 Centroid_Count=117 Total_Compressions=192.261k items_per_second=721.834k/s
BM_td_add_lognormal_dist/300/10000000  686360957 ns    686338645 ns           20 Centroid_Count=158 Total_Compressions=121.153k items_per_second=728.503k/s
BM_td_add_lognormal_dist/400/10000000  710243244 ns    710176968 ns           20 Centroid_Count=202 Total_Compressions=90.684k items_per_second=704.05k/s
BM_td_add_lognormal_dist/500/10000000  727330010 ns    727310898 ns           19 Centroid_Count=247 Total_Compressions=68.802k items_per_second=723.646k/s
```
## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
