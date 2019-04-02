context("basic test")

td <- td_create(10)

expect_is(td, "tdigest")

expect_equal(td_total_count(td), 0)

td_add(td, 0, 1)
td_add(td, 10, 1)

expect_equal(td_total_count(td), 2)

expect_equal(td_value_at(td, 0.1), 0)
expect_equal(td_value_at(td, 0.5), 5)


context("bigger test")

set.seed(1492)
x <- sample(0:100, 1000000, replace = TRUE)
td <- tdigest(x, 1000)
expect_equal(td_total_count(td), 1000000)

expect_identical(
  as.integer(
    tquantile(td, c(0, .01, .1, .2, .3, .4, .5, .6, .7, .8, .9, .99, 1))
  ),
  c(0L, 0L, 9L, 19L, 29L, 39L, 50L, 60L, 70L, 80L, 90L, 99L, 100L)
)
