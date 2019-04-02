
[![Travis-CI Build
Status](https://travis-ci.org/hrbrmstr/tdigest.svg?branch=master)](https://travis-ci.org/hrbrmstr/tdigest)
[![Coverage
Status](https://codecov.io/gh/hrbrmstr/tdigest/branch/master/graph/badge.svg)](https://codecov.io/gh/hrbrmstr/tdigest)
[![CRAN\_Status\_Badge](https://www.r-pkg.org/badges/version/tdigest)](https://cran.r-project.org/package=tdigest)

# tdigest

Accurate Quantiles Using ‘t-Digests’

## Description

The t-digest construction algorithm uses a variant of 1-dimensional
k-means clustering to produce a very compact data structure that allows
accurate estimation of quantiles. This t-digest data structure can be
used to estimate quantiles, compute other rank statistics or even to
estimate related measures like trimmed means. The advantage of the
t-digest over previous digests for this purpose is that the t-digest
handles data with full floating point resolution. With small changes,
the t-digest can handle values from any ordered set for which we can
compute something akin to a mean. The accuracy of quantile estimates
produced by t-digests can be orders of magnitude more accurate than
those produced by previous digest algorithms.

## What’s Inside The Tin

The following functions are implemented:

  - `tdigest`: Create a new t-digest histogram from a vector
  - `td_add`: Add a value to the t-digest with the specified count
  - `td_create`: Allocate a new histogram
  - `td_merge`: Merge one t-digest into another
  - `td_quantile_of`: Return the quantile of the value
  - `td_total_count`: Total items contained in the t-digest
  - `td_value_at`: Return the value at the specified quantile
  - `tquantile`: Create a new t-digest histogram from a vector

## Installation

``` r
devtools::install_git("https://sr.ht.com/~hrbrmstr/tdigest.git")
# or
devtools::install_git("https://gitlab.com/hrbrmstr/tdigest.git")
# or (if you must)
devtools::install_github("hrbrmstr/tdigest")
```

## Usage

``` r
library(tdigest)

# current version
packageVersion("tdigest")
## [1] '0.1.0'
```

### Basic (Low-level interface)

``` r
td <- td_create(10)

td
## <tdigest; size=0>

td_total_count(td)
## [1] 0

td_add(td, 0, 1) %>% 
  td_add(10, 1)
## <tdigest; size=2>

td_total_count(td)
## [1] 2

td_value_at(td, 0.1) == 0
## [1] TRUE
td_value_at(td, 0.5) == 5
## [1] TRUE
```

#### Bigger (and Vectorised)

``` r
td <- tdigest(c(0, 10), 10)

td_value_at(td, 0.1) == 0
## [1] TRUE
td_value_at(td, 0.5) == 5
## [1] TRUE

set.seed(1492)
x <- sample(0:100, 1000000, replace = TRUE)
td <- tdigest(x, 1000)

td_total_count(td)
## [1] 1e+06

tquantile(td, c(0, .01, .1, .2, .3, .4, .5, .6, .7, .8, .9, .99, 1))
##  [1]   0.0000000   0.8632378   9.6763281  19.7028368  29.7718982  39.9706864  50.0032181  60.0859360  70.1951621
## [10]  80.2785864  90.3290326  99.5151872 100.0000000
```

#### Proof it’s faster

``` r
microbenchmark::microbenchmark(
  tdigest = tquantile(td, c(0, .01, .1, .2, .3, .4, .5, .6, .7, .8, .9, .99, 1)),
  r_quantile = quantile(x, c(0, .01, .1, .2, .3, .4, .5, .6, .7, .8, .9, .99, 1))
)
## Unit: microseconds
##        expr       min        lq        mean     median        uq       max neval
##     tdigest     4.542     6.033    16.79928    20.5475    21.787    47.159   100
##  r_quantile 62035.574 63652.134 66540.00544 64935.1235 68307.315 85123.407   100
```

## tdigest Metrics

| Lang         | \# Files |  (%) | LoC |  (%) | Blank lines |  (%) | \# Lines |  (%) |
| :----------- | -------: | ---: | --: | ---: | ----------: | ---: | -------: | ---: |
| C            |        3 | 0.27 | 337 | 0.77 |          45 | 0.42 |       26 | 0.15 |
| R            |        6 | 0.55 |  60 | 0.14 |          18 | 0.17 |       77 | 0.43 |
| Rmd          |        1 | 0.09 |  28 | 0.06 |          33 | 0.31 |       48 | 0.27 |
| C/C++ Header |        1 | 0.09 |  10 | 0.02 |          10 | 0.09 |       28 | 0.16 |

## Code of Conduct

Please note that this project is released with a [Contributor Code of
Conduct](CONDUCT.md). By participating in this project you agree to
abide by its terms.
