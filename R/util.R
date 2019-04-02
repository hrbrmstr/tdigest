is_null_xptr <- function(x) {
  .Call("is_null_xptr_", x)
}