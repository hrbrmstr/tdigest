#' Create a new t-digest histogram from a vector
#'
#' The t-digest construction algorithm uses a variant of 1-dimensional
#' k-means clustering to produce a very compact data structure that allows
#' accurate estimation of quantiles. This t-digest data structure can be used
#' to estimate quantiles, compute other rank statistics or even to estimate
#' related measures like trimmed means. The advantage of the t-digest over
#' previous digests for this purpose is that the t-digest handles data with
#' full floating point resolution. With small changes, the t-digest can handle
#' values from any ordered set for which we can compute something akin to a mean.
#' The accuracy of quantile estimates produced by t-digests can be orders of
#' magnitude more accurate than those produced by previous digest algorithms.
#'
#' @param vec vector (will be converted to `double` if not already double). NOTE that this
#'        is ALTREP-aware and will not materialize the passed-in object in order to
#'        add the values to the t-Digest.
#' @param compression the input compression value; should be >= 1.0; this
#'        will control how aggressively the TDigest compresses data together.
#'        The original t-Digest paper suggests using a value of 100 for a good
#'        balance between precision and efficiency. It will land at very small
#'        (think like 1e-6 percentile points) errors at extreme points in the
#'        distribution, and compression ratios of around 500 for large data sets
#'        (~1 million datapoints). Defaults to 100.
#' @export
#' @return a tdigest object
#' @references <https://raw.githubusercontent.com/tdunning/t-digest/master/docs/t-digest-paper/histo.pdf>
#' @importFrom stats quantile
#' @useDynLib tdigest, .registration = TRUE
#' @examples
#' set.seed(1492)
#' x <- sample(0:100, 1000000, replace = TRUE)
#' td <- tdigest(x, 1000)
#' tquantile(td, c(0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99, 1))
#' quantile(td)
tdigest <- function(vec, compression=100) {
  if (!is.double(vec)) vec <- as.double(vec)
  .Call("Rtdig", vec=vec, compression=compression)
}

#' Calcuate sample quantiles from a t-digest
#'
#' @param td t-digest object
#' @param probs numeric vector of probabilities with values in range 0:1
#' @export
#' @return a numeric vector
#' @references <https://raw.githubusercontent.com/tdunning/t-digest/master/docs/t-digest-paper/histo.pdf>
#' @examples
#' set.seed(1492)
#' x <- sample(0:100, 1000000, replace = TRUE)
#' td <- tdigest(x, 1000)
#' tquantile(td, c(0, .01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99, 1))
#' quantile(td)
tquantile <- function(td, probs) {
  stopifnot(inherits(td, "tdigest"))
  stopifnot(!is_null_xptr(td))
  if (!is.double(probs)) probs <- as.double(probs)
  .Call("Rtquant", tdig=td, probs=probs, PACKAGE="tdigest")
}

#' @param x numeric vector whose sample quantiles are wanted
#' @param ... unused
#' @rdname tquantile
#' @export
quantile.tdigest <- function(x, probs = seq(0, 1, 0.25), ...) {
  tquantile(x, probs=probs)
}

#' @rdname tdigest
#' @param x t-tigest object
#' @param ... unused
#' @keywords internal
#' @export
print.tdigest <- function(x, ...) {
  stopifnot(inherits(x, "tdigest"))
  if (is_null_xptr(x)) {
    cat("<tdigest; invalid xptr>\n")
  } else {
    cat(
      "<tdigest; size=",
      formatC(td_total_count(x), format="fg", big.mark=","),
      ">\n", sep=""
    )
  }
  invisible(x)
}

#' Allocate a new histogram
#'
#' @param compression the input compression value; should be >= 1.0; this
#'        will control how aggressively the TDigest compresses data together.
#'        The original t-Digest paper suggests using a value of 100 for a good
#'        balance between precision and efficiency. It will land at very small
#'        (think like 1e-6 percentile points) errors at extreme points in the
#'        distribution, and compression ratios of around 500 for large data sets
#'        (~1 million datapoints). Defaults to 100.
#' @export
#' @return a tdigest object
#' @references <https://raw.githubusercontent.com/tdunning/t-digest/master/docs/t-digest-paper/histo.pdf>
#' @examples
#' td <- td_create(10)
td_create <- function(compression=100) {
  compression <- as.double(compression[1])
  .Call("Rtd_create", compression=compression, PACKAGE="tdigest")
}

#' Total items contained in the t-digest
#'
#' @param td t-digest object
#' @export
#' @examples
#' td <- td_create(10)
#' td_add(td, 0, 1)
#' td_total_count(td)
#' length(td)
td_total_count <- function(td) {
  stopifnot(inherits(td, "tdigest"))
  stopifnot(!is_null_xptr(td))
  .Call("Rtd_total_count", td=td, PACKAGE="tdigest")
}

#' Add a value to the t-digest with the specified count
#'
#' @param td t-digest object
#' @param val value
#' @param count count
#' @export
#' @examples
#' td <- td_create(10)
#' td_add(td, 0, 1)
td_add <- function(td, val, count) {
  stopifnot(inherits(td, "tdigest"))
  stopifnot(!is_null_xptr(td))
  val <- as.double(val[1])
  stopifnot(!is.na(val))
  count <- as.double(count[1])
  .Call("Rtd_add", tdig=td, val=val, count=count, PACKAGE="tdigest")
}

#' Return the value at the specified quantile
#'
#' @param td t-digest object
#' @param q quantile (range 0:1)
#' @export
#' @examples
#' td <- td_create(10)
#'
#' td_add(td, 0, 1) %>%
#'   td_add(10, 1)
#'
#' td_value_at(td, 0.1)
#' td_value_at(td, 0.5)
#' td[0.1]
#' td[0.5]
td_value_at <- function(td, q) {
  stopifnot(inherits(td, "tdigest"))
  stopifnot(!is_null_xptr(td))
  q <- as.double(q[1])
  .Call("Rtd_value_at", tdig=td, q=q, PACKAGE="tdigest")
}

#' Return the quantile of the value
#'
#' @param td t-digest object
#' @param val value
#' @export
td_quantile_of <- function(td, val) {
  stopifnot(inherits(td, "tdigest"))
  stopifnot(!is_null_xptr(td))
  val <- as.double(val[1])
  .Call("Rtd_quantile_of", tdig=td, val=val, PACKAGE="tdigest")
}

#' Merge one t-digest into another
#'
#' @param from,into t-digests
#' @return `into`
#' @return a tdigest object
#' @export
td_merge <- function(from, into) {
  stopifnot(inherits(from, "tdigest"))
  stopifnot(inherits(into, "tdigest"))
  stopifnot(!is_null_xptr(from))
  stopifnot(!is_null_xptr(into))
  .Call("Rtd_merge", from=from, into=into, PACKAGE="tdigest")
}


#' @rdname td_total_count
#' @param x a tdigest object
#' @export
length.tdigest <- function(x) {
  td_total_count(x)
}

#' @rdname td_value_at
#' @param x a tdigest object
#' @param i quantile (range 0:1)
#' @param ... unused
#' @export
`[.tdigest` <- function(x, i, ...) {
  if (length(x) == 0) return(NULL)
  i <- as.double(i[1])
  if ((i<=0) || (i>1)) return(NULL)
  td_value_at(x, i)
}
