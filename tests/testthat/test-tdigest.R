context("basic test")

td <- td_create(10)

expect_is(td, "tdigest")

expect_true(is_tdigest(td))
expect_equal(td_total_count(td), 0)

expect_true(is.nan(td_value_at(td, 0)))
expect_true(is.nan(td_value_at(td, 0.5)))
expect_true(is.nan(td_value_at(td, 1)))
expect_true(is.nan(td_value_at(td, -0.1)))
expect_true(is.nan(td_value_at(td, 1.1)))

td_add(td, 0, 1)
td_add(td, 10, 1)

expect_equal(td_total_count(td), 2)

expect_equal(td_value_at(td, 0.1), 0)
expect_equal(td_value_at(td, 0.5), 5)

td <- td_create(1000)
td_add(td, 1, 1)
td_add(td, 10, 1)

expect_equal(td_quantile_of(td, 0.99), 0)
expect_equal(td_quantile_of(td, 1), 0.25)
expect_equal(td_quantile_of(td, 5.5), 0.5)

context("bigger, vectorised test")

set.seed(1492)
x <- sample(0:100, 1000000, replace = TRUE)
td <- tdigest(x, 1000)

expect_true(is_tdigest(td))
expect_false(is_tdigest(x))

expect_equal(td_total_count(td), 1000000)

expect_equal(
  ceiling(
    tquantile(td, c(0, .01, .1, .2, .3, .4, .5, .6, .7, .8, .9, .99, 1))
  ),
  c(0, 1, 10, 20, 30, 40, 51, 61, 71, 81, 91, 100, 100)
)

expect_equal(
  ceiling(quantile(td)),
  c(0, 25, 51, 76, 100)
)
