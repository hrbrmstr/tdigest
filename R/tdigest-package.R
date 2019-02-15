#' Accurate Quantiles Using 't-Digests'
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
#' - URL: <https://gitlab.com/hrbrmstr/tdigest>
#' - BugReports: <https://gitlab.com/hrbrmstr/tdigest/issues>
#'
#' @md
#' @name tdigest
#' @docType package
#' @author Bob Rudis (bob@@rud.is)
#' @useDynLib tdigest, .registration = TRUE
#' @importFrom Rcpp sourceCpp
NULL