#' Create a new t-digest histogram from a vector
#'
#' @param vec vector (will be converted to `double` if not already double
#' @param compression compression
#' @export
tdigest <- function(vec, compression) {
  if (!is.double(vec)) vec <- as.double(vec)
  .Call("Rtdig", vec=vec, compression=compression)
}

#' Create a new t-digest histogram from a vector
#'
#' @param td t-digest object
#' @param probs numeric vector of probabilities with values in range 0:1
#' @export
tquantile <- function(td, probs) {
  if (!is.double(probs)) probs <- as.double(probs)
  .Call("Rtquant", tdig=td, probs=probs)
}

#' Printer for t-idgest objects
#'
#' @param x t-tigest object
#' @param ... unused
#' @keywords internal
#' @export
print.tdigest <- function(x, ...) {
  cat(
    "<tdigest; size=",
    formatC(td_total_count(x), format="fg", big.mark=","),
    ">\n", sep=""
  )
}

#' Allocate a new histogram
#'
#' @param compression compression
#' @export
td_create <- function(compression) {
  .Call("Rtd_create", compression=compression)
}

#' Total items contained in the t-digest
#'
#' @param td t-digest object
#' @export
td_total_count <- function(td) {
  .Call("Rtd_total_count", td)
}

#' Add a value to the t-digest with the specified count
#'
#' @param td t-digest object
#' @param val value
#' @param count count
#' @export
td_add <- function(td, val, count) {
  .Call("Rtd_add", tdig=td, val=val, count=count)
}

#' Return the value at the specified quantile
#'
#' @param td t-digest object
#' @param q quantile (range 0:1)
#' @export
td_value_at <- function(td, q) {
  .Call("Rtd_value_at", tdig=td, q=q)
}

#' Return the quantile of the value
#'
#' @param td t-digest object
#' @param val value
#' @export
td_quantile_of <- function(td, val) {
  .Call("Rtd_quantile_of", tdig=td, val=val)
}

#' Merge one t-digest into another
#'
#' @param from,into t-digests
#' @return `into`
#' @export
td_merge <- function(from, into) {
  .Call("Rtd_merge", from=from, into=into)
}
