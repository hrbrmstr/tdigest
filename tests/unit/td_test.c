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

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static double randfrom(double M, double N) { return M + (rand() / (RAND_MAX / (N - M))); }

int tests_run = 0;

td_histogram_t *histogram = NULL;

static void load_histograms(void) {
    const int compression = 500;

    int i;
    if (histogram) {
        td_free(histogram);
    }
    histogram = td_new(compression);

    for (i = 0; i < STREAM_SIZE; i++) {
        mu_assert(td_add(histogram, randfrom(0, 10), 1) == 0, "Insertion");
    }
}

MU_TEST(test_basic) {
    td_histogram_t *t = td_new(10);
    mu_assert(t != NULL, "created_histogram");
    mu_assert_long_eq(0, t->unmerged_weight);
    mu_assert_long_eq(0, t->merged_weight);
    mu_assert(td_add(t, 0.0, 1) == 0, "Insertion");
    // with one data point, all quantiles lead to Rome
    mu_assert_double_eq(0.0, td_quantile(t, .0));
    mu_assert_double_eq(0.0, td_quantile(t, 0.5));
    mu_assert_double_eq(0.0, td_quantile(t, 1));
    mu_assert(td_add(t, 10.0, 1) == 0, "Insertion");
    mu_assert_double_eq(0.0, td_min(t));
    mu_assert_double_eq(10.0, td_max(t));
    mu_assert_double_eq(2.0, td_size(t));
    mu_assert(t != NULL, "Failed to allocate hdr_histogram");
    mu_assert_double_eq(10.0, t->compression);
    mu_assert(td_compression(t) < t->cap, "False: buffer size < compression");
    mu_assert_double_eq(0.0, td_quantile(t, .0));
    mu_assert_double_eq(0.0, td_quantile(t, .1));
    mu_assert_double_eq(10.0, td_quantile(t, .99));
    td_reset(t);
    td_reset(NULL);
    td_free(t);
}

MU_TEST(test_overflow) {
    td_histogram_t *t = td_new(10);
    td_histogram_t *t2 = td_new(10);
    mu_assert(t != NULL, "created_histogram");
    mu_assert(t2 != NULL, "created_histogram");
    mu_assert_long_eq(0, t->unmerged_weight);
    mu_assert_long_eq(0, t->merged_weight);
    mu_assert_long_eq(0, t2->unmerged_weight);
    mu_assert_long_eq(0, t2->merged_weight);
    mu_assert(td_add(t, 5.0, __LONG_LONG_MAX__ - 1) == 0, "Insertion of __LONG_LONG_MAX__");
    mu_assert(td_add(t, 5.0, __LONG_LONG_MAX__ - 1) == EDOM,
              "second insertion of __LONG_LONG_MAX__ should overflow");
    mu_assert_long_eq(__LONG_LONG_MAX__ - 1, t->merged_weight + t->unmerged_weight);
    // overflow on merge
    mu_assert(td_add(t2, 5.0, __LONG_LONG_MAX__ - 1) == 0, "First insertion of __LONG_LONG_MAX__");
    mu_assert_long_eq(__LONG_LONG_MAX__ - 1, t2->merged_weight + t2->unmerged_weight);
    mu_assert(td_add(t2, 1.0, 1) == 0, "Insertion of 1");
    mu_assert(td_add(t2, 5.0, __LONG_LONG_MAX__ - 1) == EDOM,
              "Second insertion of __LONG_LONG_MAX__");
    td_free(t);
    td_free(t2);
}

MU_TEST(test_overflow_merge) {
    td_histogram_t *x = td_new(1000);
    td_histogram_t *y = td_new(1000);
    td_histogram_t *z = td_new(10);
    mu_assert(x != NULL, "created_histogram");
    mu_assert(y != NULL, "created_histogram");
    mu_assert(z != NULL, "created_histogram");
    mu_assert_long_eq(0, x->unmerged_weight);
    mu_assert_long_eq(0, x->merged_weight);
    mu_assert_long_eq(0, y->unmerged_weight);
    mu_assert_long_eq(0, y->merged_weight);
    mu_assert(td_add(x, 1, 1) == 0, "Insertion of 1");
    mu_assert(td_add(x, 2, 1) == 0, "Insertion of 2");
    mu_assert(td_add(x, 3, 1) == 0, "Insertion of 3");
    mu_assert(td_add(x, 4, 1) == 0, "Insertion of 4");
    mu_assert(td_add(x, 5, 1) == 0, "Insertion of 5");
    mu_assert(td_add(x, 6, 1) == 0, "Insertion of 6");
    mu_assert(td_add(x, 7, 1) == 0, "Insertion of 7");
    mu_assert(td_add(x, 8, 1) == 0, "Insertion of 8");
    mu_assert(td_add(x, 9, 1) == 0, "Insertion of 9");
    mu_assert(td_add(x, 10, 1) == 0, "Insertion of 10");
    mu_assert(td_add(x, 11, 1) == 0, "Insertion of 11");
    mu_assert(td_add(x, 12, 1) == 0, "Insertion of 12");
    mu_assert(td_add(x, 13, 1) == 0, "Insertion of 13");
    mu_assert(td_add(x, 14, 1) == 0, "Insertion of 14");
    mu_assert(td_add(x, 15, 1) == 0, "Insertion of 15");
    mu_assert(td_add(x, 16, 1) == 0, "Insertion of 16");
    mu_assert(td_add(x, 17, 1) == 0, "Insertion of 17");
    mu_assert(td_add(x, 18, 1) == 0, "Insertion of 18");
    mu_assert(td_add(x, 19, 1) == 0, "Insertion of 19");
    mu_assert(td_add(x, 20, 1) == 0, "Insertion of 20");
    mu_assert(td_add(y, 101, 1) == 0, "Insertion of 101");
    mu_assert(td_add(y, 102, 1) == 0, "Insertion of 102");
    mu_assert(td_add(y, 103, 1) == 0, "Insertion of 103");
    mu_assert(td_add(y, 104, 1) == 0, "Insertion of 104");
    mu_assert(td_add(y, 105, 1) == 0, "Insertion of 105");
    mu_assert(td_add(y, 106, 1) == 0, "Insertion of 106");
    mu_assert(td_add(y, 107, 1) == 0, "Insertion of 107");
    mu_assert(td_add(y, 108, 1) == 0, "Insertion of 108");
    mu_assert(td_add(y, 109, 1) == 0, "Insertion of 109");
    mu_assert(td_add(y, 110, 1) == 0, "Insertion of 110");
    mu_assert(td_add(y, 111, 1) == 0, "Insertion of 111");
    mu_assert(td_add(y, 112, 1) == 0, "Insertion of 112");
    mu_assert(td_add(y, 113, 1) == 0, "Insertion of 113");
    mu_assert(td_add(y, 114, 1) == 0, "Insertion of 114");
    mu_assert(td_add(y, 115, 1) == 0, "Insertion of 115");
    mu_assert(td_add(y, 116, 1) == 0, "Insertion of 116");
    mu_assert(td_add(y, 117, 1) == 0, "Insertion of 117");
    mu_assert(td_add(y, 118, 1) == 0, "Insertion of 118");
    mu_assert(td_add(y, 119, 1) == 0, "Insertion of 119");
    mu_assert(td_add(y, 120, 1) == 0, "Insertion of 120");

    for (size_t i = 0; i < 10; i++) {
        td_histogram_t *zz = td_new(10);
        int self_merge_res = 0;
        mu_assert(td_merge(zz, x) == 0, "1st merge x into z");
        mu_assert(td_merge(zz, y) == 0, "1st merge y into z");
        mu_assert(td_merge(zz, x) == 0, "2nd merge x into z");
        mu_assert(td_merge(zz, y) == 0, "2nd merge y into z");
        mu_assert(td_merge(zz, x) == 0, "3rd merge x into z");
        for (size_t j = 0; j < 5; j++) {
            self_merge_res = td_merge(zz, z);
        }
        td_free(z);
        z = zz;
        mu_assert((z->merged_weight + z->unmerged_weight) > 0, "assert z contains weight");
        if (self_merge_res == EDOM)
            break;
    }

    td_free(x);
    td_free(y);
    td_free(z);
}

MU_TEST(test_quantile_interpolations) {
    td_histogram_t *t = td_new(10);
    mu_assert(t != NULL, "created_histogram");
    mu_assert_long_eq(0, t->unmerged_weight);
    mu_assert_long_eq(0, t->merged_weight);
    mu_assert(td_add(t, 5.0, 2) == 0, "add");
    mu_assert_long_eq(2, t->unmerged_weight);
    // with one data point, all quantiles lead to Rome
    mu_assert_double_eq(5.0, td_quantile(t, .0));
    mu_assert_double_eq(5.0, td_quantile(t, 0.5));
    mu_assert_double_eq(5.0, td_quantile(t, 1.0));
    mu_assert(td_compress(t) == 0, "compress");
    mu_assert_long_eq(0, t->unmerged_weight);
    mu_assert_long_eq(2, t->merged_weight);
    mu_assert(td_add(t, 100.0, 1) == 0, "Insertion");
    // we know that there are at least two centroids now
    td_free(t);
}

MU_TEST(test_trimmed_mean_simple) {
    /* Used numpy to check results validity
     import numpy as np
     from scipy import stats
     x = [5,5,5,10,15,15,15]
     np.mean(x)
     10.0
     stats.trim_mean(x, 0.0)
     10.0
     */
    td_histogram_t *t = td_new(100);
    mu_assert(t != NULL, "created_histogram");
    mu_assert_long_eq(0, t->unmerged_weight);
    mu_assert_long_eq(0, t->merged_weight);
    //    stats.trim_mean([], 0.49)
    //    nan
    mu_assert_double_eq(NAN, td_trimmed_mean_symmetric(t, .49));
    mu_assert_double_eq(NAN, td_trimmed_mean(t, 0.49, 0.51));
    mu_assert(td_add(t, 5.0, 1) == 0, "Insertion");
    // with one data point, all quantiles lead to Rome
    // stats.trim_mean(x, 0.49)
    mu_assert_double_eq(5, td_trimmed_mean_symmetric(t, .49));
    mu_assert_double_eq(5, td_trimmed_mean(t, 0.49, 0.51));
    // stats.trim_mean(x, 0.1)
    // 5.0
    mu_assert_double_eq(5, td_trimmed_mean_symmetric(t, .1));
    mu_assert_double_eq(5, td_trimmed_mean(t, 0.1, 0.9));
    // 5.0
    // stats.trim_mean(x, 0.0)
    mu_assert_double_eq(5, td_trimmed_mean_symmetric(t, .0));
    mu_assert_double_eq(5, td_trimmed_mean(t, 0.0, 1));
    // 5.0
    mu_assert(td_add(t, 5.0, 2) == 0, "Insertion");
    mu_assert_double_eq(5, td_trimmed_mean_symmetric(t, .0));
    mu_assert_double_eq(5, td_trimmed_mean(t, 0.0, 1));
    mu_assert(td_add(t, 10.0, 1) == 0, "Insertion");
    mu_assert(td_add(t, 15.0, 3) == 0, "Insertion");
    //    stats.trim_mean(x, 0.0)
    //    10.0
    mu_assert_double_eq(10, td_trimmed_mean_symmetric(t, .0));
    mu_assert_double_eq(10, td_trimmed_mean(t, 0.0, 1));
    // trimmed mean and mean should lead to 10 in here
    //    stats.trim_mean(x, 0.1)
    //    10.0
    mu_assert_double_eq(10, td_trimmed_mean_symmetric(t, .1));
    mu_assert_double_eq(10, td_trimmed_mean(t, .1, .9));
    // trimmed mean and mean should lead to 10 in here
    //    stats.trim_mean(x, 0.25)
    //    10.0
    mu_assert_double_eq(10, td_trimmed_mean_symmetric(t, .25));
    mu_assert_double_eq(10, td_trimmed_mean(t, .25, .75));
    td_free(t);
}

MU_TEST(test_trimmed_mean_complex) {
    /* Used numpy to check results validity
     import numpy as np
     from scipy import stats
     x = np.arange(20)
     stats.trim_mean(x, 0.1)
     9.5
     */
    td_histogram_t *t = td_new(100);
    mu_assert(t != NULL, "created_histogram");
    mu_assert_long_eq(0, t->unmerged_weight);
    mu_assert_long_eq(0, t->merged_weight);
    for (int i = 0; i < 20; ++i) {
        mu_assert(td_add(t, (double)i, 1) == 0, "Insertion");
    }
    // trimmed mean and mean should lead to 9.5 in here
    //    stats.trim_mean(x, 0.25)
    //    9.5
    mu_assert_double_eq(9.5, td_trimmed_mean_symmetric(t, .25));
    mu_assert_double_eq(9.5, td_trimmed_mean(t, .25, .75));
    td_free(t);
    t = td_new(100);
    mu_assert(t != NULL, "created_histogram");
    mu_assert_long_eq(0, t->unmerged_weight);
    mu_assert_long_eq(0, t->merged_weight);
    for (int i = 0; i < 200; ++i) {
        mu_assert(td_add(t, (double)i, 1) == 0, "Insertion");
    }
    // trimmed mean and mean should lead to 99.5 in here
    //    x = np.arange(200)
    //    stats.trim_mean(x, 0.25)
    //    99.5
    mu_assert_double_eq_epsilon(99.5, td_trimmed_mean_symmetric(t, .25), 0.1);
    mu_assert_double_eq_epsilon(99.5, td_trimmed_mean(t, .25, .75), 0.1);

    // Non symmetric trimmed means
    //    trim_mean(x, 0.1, 0.75)
    //    84.5
    mu_assert_double_eq_epsilon(84.5, td_trimmed_mean(t, .1, 0.75), 0.1);
    //    trim_mean(x, 0.0, 0.75)
    //    74.5
    mu_assert_double_eq_epsilon(74.5, td_trimmed_mean(t, .0, 0.75), 0.1);

    td_free(t);
    //    x = [1,2,3,4,5,6,7,8,9,10,100,100,100]
    t = td_new(100);
    for (int i = 1; i < 11; ++i) {
        mu_assert(td_add(t, (double)i, 1) == 0, "Insertion");
    }
    mu_assert(td_add(t, 100, 3) == 0, "Insertion");
    //    stats.trim_mean(x, 0.1)
    //    23.09090909090909
    mu_assert_double_eq_epsilon(23.09090909090909, td_trimmed_mean_symmetric(t, .1), 0.01);
    mu_assert_double_eq_epsilon(23.09090909090909, td_trimmed_mean(t, .1, .9), 0.01);
    //    stats.trim_mean(x, 0.25)
    //    7.0
    mu_assert_double_eq_epsilon(7.0, td_trimmed_mean_symmetric(t, .25), 0.01);
    mu_assert_double_eq_epsilon(7.0, td_trimmed_mean(t, .25, .75), 0.01);
    td_free(t);
}

MU_TEST(test_compress_small) {
    td_histogram_t *t = td_new(100);
    mu_assert(t != NULL, "created_histogram");
    mu_assert(td_add(t, 1.0, 1) == 0, "Insertion");
    mu_assert_double_eq(1.0, td_min(t));
    mu_assert_double_eq(1.0, td_max(t));
    mu_assert_double_eq(1.0, td_size(t));
    mu_assert_int_eq(1, td_centroid_count(t));
    mu_assert_long_eq(0, t->total_compressions);
    mu_assert_double_eq(1.0, td_centroids_mean_at(t, 0));
    mu_assert_long_eq(1, td_centroids_weight_at(t, 0));
    mu_assert_int_eq(1, t->unmerged_nodes);
    mu_assert_int_eq(0, t->merged_nodes);
    mu_assert(td_compress(t) == 0, "compress");
    mu_assert_long_eq(1, t->unmerged_nodes + t->merged_nodes);
    mu_assert_double_eq(1.0, td_centroids_mean_at(t, 0));
    mu_assert_long_eq(1, td_centroids_weight_at(t, 0));
    mu_assert_double_eq(1.0, td_quantile(t, 0.001));
    mu_assert_double_eq(1.0, td_quantile(t, 0.01));
    mu_assert_double_eq(1.0, td_quantile(t, 0.5));
    mu_assert_double_eq(1.0, td_quantile(t, 0.99));
    mu_assert_double_eq(1.0, td_quantile(t, 0.999));
    td_free(t);
}

MU_TEST(test_compress_large) {
    td_histogram_t *t = td_new(100);
    mu_assert(t != NULL, "created_histogram");
    for (int i = 1; i <= 1000; ++i) {
        mu_assert(td_add(t, (double)i, 1) == 0, "Insertion");
    }

    mu_assert_double_eq(1.0, td_min(t));
    mu_assert_double_eq(1000.0, td_max(t));
    mu_assert_double_eq(1000.0, td_size(t));
    // TODO: add this test cases
    // EXPECT_EQ(500500, digest.sum());
    // EXPECT_EQ(500.5, digest.mean());
    // mu_assert_double_eq(1.5, td_quantile(t, 0.001));
    mu_assert_double_eq(10.5, td_quantile(t, 0.01));
    // mu_assert_double_eq_epsilon(500.25, td_quantile(t, 0.5), 0.5);
    // TODO: swap this one by the bellow
    // mu_assert_double_eq(990.25, td_quantile(t, 0.99));
    mu_assert_double_eq_epsilon(990.25, td_quantile(t, 0.99), 0.5);
    // mu_assert_double_eq(999.5, td_quantile(t, 0.999));
    td_free(t);
}

MU_TEST(test_negative_values) {
    td_histogram_t *t = td_new(1000);
    mu_assert(t != NULL, "created_histogram");
    for (int i = 1; i <= 100; ++i) {
        mu_assert(td_add(t, (double)i, 1) == 0, "Insertion");
        mu_assert(td_add(t, -(double)i, 1) == 0, "Insertion");
    }
    mu_assert_double_eq(-100.0, td_min(t));
    mu_assert_double_eq(100.0, td_max(t));
    mu_assert_double_eq(200.0, td_size(t));
    mu_assert_double_eq(-100, td_quantile(t, 0.0));
    mu_assert_double_eq(-100, td_quantile(t, 0.001));
    // TODO: fix my epsilon
    mu_assert_double_eq_epsilon(-98.5, td_quantile(t, 0.01), 0.75);
    mu_assert_double_eq_epsilon(98.5, td_quantile(t, 0.99), 0.75);
    mu_assert_double_eq(100, td_quantile(t, 0.999));
    mu_assert_double_eq(100, td_quantile(t, 1));
    td_free(t);
}

MU_TEST(test_negative_values_merge) {
    td_histogram_t *d1 = td_new(100);
    td_histogram_t *d2 = td_new(100);
    mu_assert(d1 != NULL, "created_histogram");
    mu_assert(d2 != NULL, "created_histogram");
    for (int i = 1; i <= 100; ++i) {
        mu_assert(td_add(d1, (double)i, 1) == 0, "Insertion");
        mu_assert(td_add(d2, -(double)i, 1) == 0, "Insertion");
    }
    td_merge(d1, d2);
    mu_assert_double_eq(-100.0, td_min(d1));
    mu_assert_double_eq(100.0, td_max(d1));
    mu_assert_double_eq(200.0, td_size(d1));
    mu_assert_double_eq(-100, td_quantile(d1, 0.0));
    mu_assert_double_eq(-100, td_quantile(d1, 0.001));
    // TODO: fix my epsilon
    mu_assert_double_eq_epsilon(-98.5, td_quantile(d1, 0.01), 0.75);
    mu_assert_double_eq_epsilon(98.5, td_quantile(d1, 0.99), 0.75);
    mu_assert_double_eq(100, td_quantile(d1, 0.999));
    mu_assert_double_eq(100, td_quantile(d1, 1));
    td_free(d1);
    td_free(d2);
}

MU_TEST(test_large_outlier_test) {
    td_histogram_t *t = td_new(100);
    mu_assert(t != NULL, "created_histogram");
    for (int i = 1; i <= 19; ++i) {
        mu_assert(td_add(t, (double)i, 1) == 0, "Insertion");
    }
    mu_assert(td_add(t, 1000000, 1) == 0, "Insertion");
    mu_assert(td_quantile(t, 0.5) < td_quantile(t, 0.9),
              "False: td_quantile(t, 0.5) < td_quantile(t, 0.9)");
    td_free(t);
}

MU_TEST(test_nans) {
    td_histogram_t *t = td_new(1000);
    mu_assert(isnan(td_quantile(t, 0)), "empty value at 0");
    mu_assert(isnan(td_quantile(t, 0.5)), "empty value at .5");
    mu_assert(isnan(td_quantile(t, 1)), "empty value at 1");
    mu_assert(isnan(td_centroids_mean_at(t, 1)), "td_centroids_mean_at on pos > h->merged_nodes");
    mu_assert(isnan(td_centroids_mean_at(t, -1)), "td_centroids_mean_at on pos < 0");
    mu_assert(td_add(t, 1, 1) == 0, "Insertion");
    mu_assert(isnan(td_quantile(t, -.1)), "value at -0.1");
    mu_assert(isnan(td_quantile(t, 1.1)), "value at 1.1");
    td_free(t);
}

MU_TEST(test_two_interp) {
    td_histogram_t *t = td_new(1000);
    mu_assert(td_add(t, 1, 1) == 0, "Insertion");
    mu_assert(td_add(t, 10, 1) == 0, "Insertion");
    mu_assert(isfinite(td_quantile(t, .9)), "test_two_interp: value at .9");
    td_reset(t);
    // if the left centroid has more than one sample, we still know
    // that one sample occurred at min so we can do some interpolation
    mu_assert(td_add(t, 1, 10) == 0, "Insertion");
    mu_assert(td_add(t, 10, 1) == 0, "Insertion");
    mu_assert_double_eq(1.0, td_quantile(t, .1));
    td_reset(t);
    // if the right-most centroid has more than one sample, we still know
    // that one sample occurred at max so we can do some interpolation
    mu_assert(td_add(t, 1, 1) == 0, "Insertion");
    mu_assert(td_add(t, 10, 10) == 0, "Insertion");
    mu_assert_double_eq(10.0, td_quantile(t, .9));
    td_reset(t);
    // in between extremes we interpolate between centroids
    mu_assert(td_add(t, 1, 1) == 0, "Insertion");
    mu_assert(td_add(t, 5, 1) == 0, "Insertion");
    mu_assert(td_add(t, 10, 1) == 0, "Insertion");
    // centroids i and i+1 bracket our current point
    // check for unit weight
    // within the singleton's sphere
    // left
    mu_assert_double_eq(5.0, td_quantile(t, .5));
    td_reset(t);
    // in between extremes we interpolate between centroids
    mu_assert(td_add(t, 1, 1) == 0, "Insertion");  // q0
    mu_assert(td_add(t, 4, 1) == 0, "Insertion");  // q20
    mu_assert(td_add(t, 8, 1) == 0, "Insertion");  // q40
    mu_assert(td_add(t, 12, 1) == 0, "Insertion"); // q60
    mu_assert(td_add(t, 16, 1) == 0, "Insertion"); // q80
    mu_assert(td_add(t, 20, 1) == 0, "Insertion"); // q100
    // centroids i and i+1 bracket our current point
    // check for unit weight
    // within the singleton's sphere
    // TODO: check for right
    // mu_assert_double_eq(4.0, td_quantile(t, .20) );
    // mu_assert_double_eq(8.0, td_quantile(t, .40) );
    // mu_assert_double_eq(12.0, td_quantile(t, .60) );
    // mu_assert_double_eq(7.0, td_quantile(t, .70) );
    // mu_assert_double_eq(8.0, td_quantile(t, .75) );
    td_free(t);
}

MU_TEST(test_cdf) {
    td_histogram_t *t = td_new(100);
    mu_assert(isnan(td_cdf(t, 1.1)), "no data to examine");
    // interpolate if somehow we have weight > 0 and max != min
    mu_assert(td_add(t, 1, 1) == 0, "Insertion");
    // bellow lower bound
    mu_assert_double_eq(0, td_cdf(t, 0));
    // exactly one centroid, should have max==min
    // min and max are too close together to do any viable interpolation
    mu_assert_double_eq(0.5, td_cdf(t, 1));
    // above upper bound
    mu_assert_double_eq(1.0, td_cdf(t, 2));
    mu_assert(td_add(t, 10, 1) == 0, "Insertion");
    mu_assert_double_eq(.25, td_cdf(t, 1));
    mu_assert_double_eq(.5, td_cdf(t, 5.5));
    // // TODO: fix this
    // mu_assert_double_eq(1,td_cdf(t, 10));
    td_free(t);
}

MU_TEST(test_td_size) {
    load_histograms();
    mu_assert(td_size(histogram) == STREAM_SIZE, "td_size(histogram) != STREAM_SIZE");
}

MU_TEST(test_td_max) {
    load_histograms();
    mu_assert_double_eq_epsilon(10.0, td_max(histogram), 0.001);
}

MU_TEST(test_td_min) {
    load_histograms();
    mu_assert_double_eq_epsilon(0.0, td_min(histogram), 0.001);
}

MU_TEST(test_td_init) {
    td_histogram_t *t;
    // overflow detected
    // mu_assert_long_eq(1, td_init(10000000000000000, &t));
    t = NULL;
    // bellow overflow
    mu_assert_long_eq(0, td_init(1000, &t));
    td_free(t);

    mu_assert_long_eq(0, td_init(1000000, &t));
    td_free(t);

    mu_assert_long_eq(0, td_init(100000000, &t));
    td_free(t);
}

MU_TEST(test_quantiles) {
    load_histograms();
    mu_assert_double_eq_epsilon(0.0, td_quantile(histogram, 0.0), 0.001);
    mu_assert_double_eq_epsilon(1.0, td_quantile(histogram, 0.1), 0.02);
    mu_assert_double_eq_epsilon(2.0, td_quantile(histogram, 0.2), 0.02);
    mu_assert_double_eq_epsilon(3.0, td_quantile(histogram, 0.3), 0.03);
    mu_assert_double_eq_epsilon(4.0, td_quantile(histogram, 0.4), 0.04);
    mu_assert_double_eq_epsilon(5.0, td_quantile(histogram, 0.5), 0.05);
    mu_assert_double_eq_epsilon(6.0, td_quantile(histogram, 0.6), 0.04);
    mu_assert_double_eq_epsilon(7.0, td_quantile(histogram, 0.7), 0.03);
    mu_assert_double_eq_epsilon(8.0, td_quantile(histogram, 0.8), 0.02);
    mu_assert_double_eq_epsilon(9.0, td_quantile(histogram, 0.9), 0.02);
    mu_assert_double_eq_epsilon(9.99, td_quantile(histogram, 0.999), 0.01);
    mu_assert_double_eq_epsilon(9.999, td_quantile(histogram, 0.9999), 0.01);
    mu_assert_double_eq_epsilon(9.9999, td_quantile(histogram, 0.99999), 0.01);
    mu_assert_double_eq_epsilon(10.0, td_quantile(histogram, 1), 0.001);
}

MU_TEST(test_quantiles_multiple) {
    load_histograms();
    const size_t quantiles_arr_size = 14;
    double values[14] = {0.0};
    double percentiles[14] = {0.0, 0.1, 0.2, 0.3,   0.4,    0.5,     0.6,
                              0.7, 0.8, 0.9, 0.999, 0.9999, 0.99999, 1.0};
    mu_assert(td_quantiles(histogram, NULL, values, quantiles_arr_size) == EINVAL,
              "td_quantiles on NULL percentiles should return EINVAL");
    mu_assert(td_quantiles(histogram, percentiles, NULL, quantiles_arr_size) == EINVAL,
              "td_quantiles on NULL values should return EINVAL");
    mu_assert(td_quantiles(histogram, percentiles, values, quantiles_arr_size) == 0,
              "td_quantiles return should be 0");
    mu_assert_double_eq_epsilon(0.0, values[0], 0.001);
    mu_assert_double_eq_epsilon(1.0, values[1], 0.02);
    mu_assert_double_eq_epsilon(2.0, values[2], 0.02);
    mu_assert_double_eq_epsilon(3.0, values[3], 0.03);
    mu_assert_double_eq_epsilon(4.0, values[4], 0.04);
    mu_assert_double_eq_epsilon(5.0, values[5], 0.05);
    mu_assert_double_eq_epsilon(6.0, values[6], 0.04);
    mu_assert_double_eq_epsilon(7.0, values[7], 0.03);
    mu_assert_double_eq_epsilon(8.0, values[8], 0.02);
    mu_assert_double_eq_epsilon(9.0, values[9], 0.02);
    mu_assert_double_eq_epsilon(9.99, values[10], 0.01);
    mu_assert_double_eq_epsilon(9.999, values[11], 0.01);
    mu_assert_double_eq_epsilon(9.9999, values[12], 0.01);
    mu_assert_double_eq_epsilon(10.0, values[13], 0.001);
    td_free(histogram);
    td_histogram_t *t = td_new(100);
    mu_assert(td_quantiles(t, percentiles, values, quantiles_arr_size) == 0,
              "td_quantiles return should be 0");
    for (int i = 0; i < quantiles_arr_size; ++i) {
        mu_assert(isnan(values[i]), "no data to examine");
    }
    mu_assert(td_add(t, 1, 1) == 0, "Insertion");
    // with one data point, all quantiles lead to Rome
    mu_assert(td_quantiles(t, percentiles, values, quantiles_arr_size) == 0,
              "td_quantiles return should be 0");
    for (int i = 0; i < quantiles_arr_size; ++i) {
        mu_assert_double_eq_epsilon(1.0, values[i], 0.02);
    }
    // q should be in [0,1]
    double percentiles_nans[14] = {-10.0, 10.1, 10.2, 10.3,   10.4,    10.5,     10.6,
                                   10.7,  10.8, 10.9, -0.999, -0.9999, -0.99999, -1.0};
    mu_assert(td_quantiles(t, percentiles_nans, values, quantiles_arr_size) == 0,
              "td_quantiles return should be 0");
    for (int i = 0; i < quantiles_arr_size; ++i) {
        mu_assert(isnan(values[i]), " q should be in [0,1]");
    }
    td_free(t);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_basic);
    MU_RUN_TEST(test_td_init);
    MU_RUN_TEST(test_compress_small);
    MU_RUN_TEST(test_compress_large);
    MU_RUN_TEST(test_nans);
    MU_RUN_TEST(test_negative_values);
    MU_RUN_TEST(test_negative_values_merge);
    MU_RUN_TEST(test_large_outlier_test);
    MU_RUN_TEST(test_two_interp);
    MU_RUN_TEST(test_cdf);
    MU_RUN_TEST(test_td_size);
    MU_RUN_TEST(test_td_max);
    MU_RUN_TEST(test_td_min);
    MU_RUN_TEST(test_quantiles);
    MU_RUN_TEST(test_quantiles_multiple);
    MU_RUN_TEST(test_quantile_interpolations);
    MU_RUN_TEST(test_trimmed_mean_simple);
    MU_RUN_TEST(test_trimmed_mean_complex);
    MU_RUN_TEST(test_overflow);
    MU_RUN_TEST(test_overflow_merge);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
