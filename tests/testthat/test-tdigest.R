context("basic test")

td <- td_create(10)

expect_is(td, "tdigest")

expect_equal(td_total_count(td), 0)

td_add(td, 0, 1)
td_add(td, 10, 1)

expect_equal(td_total_count(td), 2)

expect_equal(td_value_at(td, 0.1), 0)
expect_equal(td_value_at(td, 0.5), 5)

