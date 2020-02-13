
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


## Microbenchmark

### Ingestion

#### master
``` c
tdigest/build$ ./tests/histogram_benchmark --benchmark_min_time=10
2020-02-11 20:02:33
Running ./tests/histogram_benchmark
Run on (8 X 3900 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 0.41, 0.64, 0.83
------------------------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------------
BM_td_add_uniform_dist/100/10000000   1176119202 ns   1175850805 ns           12 Centroid_Count=74 items_per_second=708.707k/s
BM_td_add_uniform_dist/200/10000000   1278471605 ns   1278168624 ns           11 Centroid_Count=124 items_per_second=711.245k/s
BM_td_add_uniform_dist/300/10000000   1342766206 ns   1342446036 ns           10 Centroid_Count=172 items_per_second=744.909k/s
BM_td_add_uniform_dist/400/10000000   1383712449 ns   1383379933 ns           10 Centroid_Count=212 items_per_second=722.867k/s
BM_td_add_uniform_dist/500/10000000   1414613825 ns   1414327524 ns           10 Centroid_Count=258 items_per_second=707.05k/s
BM_td_add_lognormal_dist/100/10000000 1175074553 ns   1175934915 ns           12 Centroid_Count=74 items_per_second=708.656k/s
BM_td_add_lognormal_dist/200/10000000 1277987660 ns   1278668415 ns           11 Centroid_Count=123 items_per_second=710.967k/s
BM_td_add_lognormal_dist/300/10000000 1337314230 ns   1337816350 ns           10 Centroid_Count=164 items_per_second=747.487k/s
BM_td_add_lognormal_dist/400/10000000 1381403323 ns   1381770352 ns           10 Centroid_Count=216 items_per_second=723.709k/s
BM_td_add_lognormal_dist/500/10000000 1414399110 ns   1414644710 ns           10 Centroid_Count=258 items_per_second=706.891k/s
```

#### perf.improvements branch

``` c
tdigest/build$ ./tests/histogram_benchmark --benchmark_min_time=10
2020-02-11 23:14:02
Running ./tests/histogram_benchmark
Run on (8 X 3900 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 1.04, 0.89, 0.66
------------------------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------------
BM_td_add_uniform_dist/100/10000000    712450620 ns    712435331 ns           20 Centroid_Count=58 items_per_second=701.818k/s
BM_td_add_uniform_dist/200/10000000    750092888 ns    750076049 ns           18 Centroid_Count=109 items_per_second=740.666k/s
BM_td_add_uniform_dist/300/10000000    781008883 ns    780995728 ns           14 Centroid_Count=151 items_per_second=914.583k/s
BM_td_add_uniform_dist/400/10000000    807423012 ns    807401866 ns           13 Centroid_Count=194 items_per_second=952.724k/s
BM_td_add_uniform_dist/500/10000000    819856779 ns    819832154 ns           17 Centroid_Count=235 items_per_second=717.507k/s
BM_td_add_lognormal_dist/100/10000000  699143606 ns    699132082 ns           20 Centroid_Count=54 items_per_second=715.172k/s
BM_td_add_lognormal_dist/200/10000000  746849189 ns    746836572 ns           18 Centroid_Count=101 items_per_second=743.878k/s
BM_td_add_lognormal_dist/300/10000000  783043547 ns    783026370 ns           14 Centroid_Count=155 items_per_second=912.212k/s
BM_td_add_lognormal_dist/400/10000000  806705545 ns    806688685 ns           13 Centroid_Count=198 items_per_second=953.566k/s
BM_td_add_lognormal_dist/500/10000000  815863658 ns    815799172 ns           17 Centroid_Count=237 items_per_second=721.054k/s
```

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
