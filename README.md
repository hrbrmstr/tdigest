
![License](https://img.shields.io/badge/License-MIT-blue.svg)

# MergingDigest

Wicked Fast, Accurate Quantiles Using ‘t-Digests’

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

## What’s Inside The Tin

The following functions are implemented:

  - `td_add`: Add a value to the t-Digest with the specified count
  - `td_create`: Allocate a new histogram
  - `td_merge`: Merge one t-Digest into another
  - `td_quantile_of`: Return the quantile of the value
  - `td_total_count`: Total items contained in the t-Digest
  - `td_value_at`: Return the value at the specified quantile
  - `tquantile`: Calculate sample quantiles from a t-Digest

## Installation

#### Proof it’s faster

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

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
