

[![Project Status: Active – The project has reached a stable, usable
state and is being actively
developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)
[![Signed
by](https://img.shields.io/badge/Keybase-Verified-brightgreen.svg)](https://keybase.io/hrbrmstr)
![Signed commit
%](https://img.shields.io/badge/Signed_Commits-3%25-lightgrey.svg)

[![cran
checks](https://cranchecks.info/badges/worst/tdigest.png)](https://cranchecks.info/pkgs/tdigest)
[![CRAN
status](https://www.r-pkg.org/badges/version/tdigest.png)](https://www.r-pkg.org/pkg/tdigest)
![Minimal R
Version](https://img.shields.io/badge/R%3E%3D-3.5.0-blue.svg)
![License](https://img.shields.io/badge/License-MIT-blue.svg)

# tdigest

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
provided to create and update t-Digests and retrieve quantiles from the
accumulated distributions.

See [the original paper by Ted Dunning & Otmar
Ertl](https://arxiv.org/abs/1902.04023) for more details on t-Digests.

## What’s Inside The Tin

The following functions are implemented:

- `as.list.tdigest`: Serialize a tdigest object to an R list or
  unserialize a serialized tdigest list back into a tdigest object
- `td_add`: Add a value to the t-Digest with the specified count
- `td_create`: Allocate a new histogram
- `td_merge`: Merge one t-Digest into another
- `td_quantile_of`: Return the quantile of the value
- `td_total_count`: Total items contained in the t-Digest
- `td_value_at`: Return the value at the specified quantile
- `tquantile`: Calculate sample quantiles from a t-Digest

## Installation

``` r
install.packages("tdigest") # NOTE: CRAN version is 0.4.1
# or
remotes::install_gitlab("hrbrmstr/tdigest")
```

NOTE: To use the ‘remotes’ install options you will need to have the
[{remotes} package](https://github.com/r-lib/remotes) installed.

## Usage

``` r
library(tdigest)

# current version
packageVersion("tdigest")
## [1] '0.4.2'
```

### Basic (Low-level interface)

``` r
td <- td_create(10)

td
## <tdigest; size=0; compression=10; cap=70>

td_total_count(td)
## [1] 0

td_add(td, 0, 1) %>% 
  td_add(10, 1)
## <tdigest; size=2; compression=10; cap=70>

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

#### Serialization

These \[de\]serialization functions make it possible to create &
populate a tdigest, serialize it out, read it in at a later time and
continue populating it enabling compact distribution accumulation &
storage for large, “continuous” datasets.

``` r
set.seed(1492)
x <- sample(0:100, 1000000, replace = TRUE)
td <- tdigest(x, 1000)

tquantile(td, c(0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99, 1))
##  [1]   0.0000000   0.8099857   9.6725790  19.7533723  29.7448283  39.7544675  49.9966628  60.0235148  70.2067574
## [10]  80.3090454  90.2594642  99.4269454 100.0000000

str(in_r <- as.list(td), 1)
## List of 7
##  $ compression   : num 1000
##  $ cap           : int 6010
##  $ merged_nodes  : int 226
##  $ unmerged_nodes: int 0
##  $ merged_count  : num 1e+06
##  $ unmerged_count: num 0
##  $ nodes         :List of 2
##  - attr(*, "class")= chr [1:2] "tdigest_list" "list"

td2 <- as_tdigest(in_r)
tquantile(td2, c(0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99, 1))
##  [1]   0.0000000   0.8099857   9.6725790  19.7533723  29.7448283  39.7544675  49.9966628  60.0235148  70.2067574
## [10]  80.3090454  90.2594642  99.4269454 100.0000000

identical(in_r, as.list(td2))
## [1] TRUE
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
##        expr       min        lq        mean     median        uq     max neval
##     tdigest     3.198     3.731     7.79369     4.4895    12.792    16.4   100
##  r_quantile 39197.353 39445.444 40069.38938 39584.8030 40062.945 43613.3   100
```

## tdigest Metrics

| Lang         | \# Files |  (%) | LoC |  (%) | Blank lines |  (%) | \# Lines |  (%) |
|:-------------|---------:|-----:|----:|-----:|------------:|-----:|---------:|-----:|
| C            |        3 | 0.15 | 499 | 0.36 |          71 | 0.29 |       45 | 0.10 |
| R            |        6 | 0.30 | 161 | 0.12 |          35 | 0.14 |      156 | 0.34 |
| C/C++ Header |        1 | 0.05 |  24 | 0.02 |          16 | 0.07 |       30 | 0.06 |
| SUM          |       10 | 0.50 | 684 | 0.50 |         122 | 0.50 |      231 | 0.50 |

{cloc} 📦 metrics for tdigest

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
