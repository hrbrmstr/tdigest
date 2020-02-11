
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

``` r
microbenchmark::microbenchmark(
  tdigest = tquantile(td, c(0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99, 1)),
  r_quantile = quantile(x, c(0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99, 1))
)
## Unit: microseconds
##        expr      min        lq       mean    median        uq        max neval cld
##     tdigest     8.02     9.175    20.2545    10.185    32.682     43.003   100  a 
##  r_quantile 52657.60 53307.742 55924.6932 54093.988 56487.027 108778.946   100   b
```

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
