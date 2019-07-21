
[![Travis-CI Build
Status](https://travis-ci.org/hrbrmstr/tdigest.svg?branch=master)](https://travis-ci.org/hrbrmstr/tdigest)
[![AppVeyor Build
Status](https://ci.appveyor.com/api/projects/status/github/hrbrmstr/tdigest?branch=master&svg=true)](https://ci.appveyor.com/project/hrbrmstr/tdigest)
[![Coverage
Status](https://codecov.io/gh/hrbrmstr/tdigest/branch/master/graph/badge.svg)](https://codecov.io/gh/hrbrmstr/tdigest)
[![CRAN\_Status\_Badge](https://www.r-pkg.org/badges/version/tdigest)](https://cran.r-project.org/package=tdigest)

# tdigest

Wicked Fast, Accurate Quantiles Using ‘t-Digests’

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

See [the original paper by Ted
Dunning](https://raw.githubusercontent.com/tdunning/t-digest/master/docs/t-digest-paper/histo.pdf)
for more details on t-Digests.

## What’s Inside The Tin

The following functions are implemented:

  - `td_add`: Add a value to the t-digest with the specified count
  - `td_create`: Allocate a new histogram
  - `td_merge`: Merge one t-digest into another
  - `td_quantile_of`: Return the quantile of the value
  - `td_total_count`: Total items contained in the t-digest
  - `td_value_at`: Return the value at the specified quantile
  - `tquantile`: Calculate sample quantiles from a t-digest

## Installation

``` r
install.packages("tdigest", repos = "https://cinc.rud.is")
# or
devtools::install_git("https://git.rud.is/hrbrmstr/tdigest.git")
# or
devtools::install_git("https://git.sr.ht/~hrbrmstr/tdigest")
# or
devtools::install_gitlab("hrbrmstr/tdigest")
# or
devtools::install_bitbucket("hrbrmstr/tdigest")
# or
devtools::install_github("hrbrmstr/tdigest")
```

## Usage

``` r
library(tdigest)

# current version
packageVersion("tdigest")
## [1] '0.3.0'
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

quantile(td)
## [1]  0  0  5 10 10
```

#### Bigger (and Vectorised)

``` r
td <- tdigest(c(0, 10), 10)

is_tdigest(td)
## [1] TRUE

td_value_at(td, 0.1) == 0
## [1] TRUE
td_value_at(td, 0.5) == 5
## [1] TRUE

set.seed(1492)
x <- sample(0:100, 1000000, replace = TRUE)
td <- tdigest(x, 1000)

td_total_count(td)
## [1] 1e+06

tquantile(td, c(0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99, 1))
##  [1]   0.0000000   0.8099857   9.6725790  19.7533723  29.7448283  39.7544675  49.9966628  60.0235148  70.2067574
## [10]  80.3090454  90.2594642  99.4269454 100.0000000

quantile(td)
## [1]   0.00000  24.74751  49.99666  75.24783 100.00000
```

#### ALTREP-aware

``` r
N <- 1000000
x.altrep <- seq_len(N) # this is an ALTREP in R version >= 3.5.0

td <- tdigest(x.altrep)
td[0.1]
## [1] 93051
td[0.5]
## [1] 491472.5
length(td)
## [1] 1000000
```

#### Proof it’s faster

``` r
microbenchmark::microbenchmark(
  tdigest = tquantile(td, c(0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99, 1)),
  r_quantile = quantile(x, c(0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99, 1))
)
## Unit: microseconds
##        expr       min         lq        mean    median         uq       max neval cld
##     tdigest     7.943     9.4015    20.94626    11.957    32.9395    48.487   100  a 
##  r_quantile 52305.639 53309.4185 55386.25517 54038.227 56644.9055 94300.294   100   b
```

## tdigest Metrics

| Lang         | \# Files |  (%) | LoC |  (%) | Blank lines |  (%) | \# Lines |  (%) |
| :----------- | -------: | ---: | --: | ---: | ----------: | ---: | -------: | ---: |
| C            |        3 | 0.27 | 350 | 0.65 |          46 | 0.36 |       26 | 0.11 |
| R            |        6 | 0.55 | 139 | 0.26 |          31 | 0.24 |      135 | 0.58 |
| Rmd          |        1 | 0.09 |  36 | 0.07 |          40 | 0.31 |       45 | 0.19 |
| C/C++ Header |        1 | 0.09 |  10 | 0.02 |          10 | 0.08 |       28 | 0.12 |

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
