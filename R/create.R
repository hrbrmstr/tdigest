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
