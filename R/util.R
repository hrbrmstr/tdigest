is_null_xptr <- function(x) {
  .Call("is_null_xptr_", x, PACKAGE="tdigest")
}

#' @rdname td_create
#' @param td t-digest object
#' @export
is_tdigest <- function(td) {
  if (inherits(td, "tdigest")) {
    if (is_null_xptr(td)) {
      message("Object was a populated tdigest but is now an invalid xptr")
      FALSE
    } else {
      TRUE
    }
  } else {
    FALSE
  }
}