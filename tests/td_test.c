/**
 * td_test.c
 * Written by Filipe Oliveira and released to the public domain,
 * as explained at http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

#include <stdio.h>
#include "tdigest.h"

#include "minunit.h"

#define STREAM_SIZE 1000000

static double __randU(double M, double N)
{
    return M + (rand() / (RAND_MAX / (N - M)));
}

int tests_run = 0;

td_histogram_t *histogram = NULL;

static void load_histograms()
{
    const int compression = 500;

    int i;
    if (histogram)
    {
        td_free(histogram);
    }
    histogram = td_new(compression);

    for (i = 0; i < STREAM_SIZE; i++)
    {
        td_add(histogram, __randU(0, 10), 1);
    }
}

// MU_TEST(test_init)
// {
//     td_histogram_t *h = NULL;
//     int r = td_init(100, &h);
//     mu_assert(r == 0, "Failed to allocate td_histogram");
//     mu_assert(h != NULL, "Failed to allocate hdr_histogram");
//     mu_assert_double_eq(h->compression, 100);

//     mu_assert(td_compression(h) < h->cap, "False: buffer size < compression");
//     mu_assert_int_eq(h->unmerged_count, 0);
//     mu_assert_int_eq(h->merged_count, 0);
//     mu_assert_double_eq(td_size(h), 0);
// }

MU_TEST(test_td_size)
{
    load_histograms();
    mu_assert(td_size(histogram) == STREAM_SIZE, "td_size(histogram) != STREAM_SIZE");
}

MU_TEST(test_td_max)
{
    load_histograms();
    mu_assert_double_eq_epsilon(td_max(histogram), 10.0, 0.001);
}

MU_TEST(test_td_min)
{
    load_histograms();
    mu_assert_double_eq_epsilon(td_min(histogram), 0.0, 0.001);
}

MU_TEST(test_quantiles)
{
    load_histograms();
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.0), 0.0, 0.001);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.1), 1.0, 0.001);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.2), 2.0, 0.002);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.3), 3.0, 0.03);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.4), 4.0, 0.04);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.5), 5.0, 0.05);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.6), 6.0, 0.04);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.7), 7.0, 0.03);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.8), 8.0, 0.02);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.9), 9.0, 0.01);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.999), 9.99, 0.01);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.9999), 9.999, 0.01);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 0.9999), 9.999, 0.01);
    mu_assert_double_eq_epsilon(td_quantile(histogram, 1.0), 10.0, 0.001);
}

MU_TEST_SUITE(test_suite)
{
    // MU_RUN_TEST(test_init);
    MU_RUN_TEST(test_td_size);
    MU_RUN_TEST(test_td_max);
    MU_RUN_TEST(test_td_min);
    MU_RUN_TEST(test_quantiles);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
