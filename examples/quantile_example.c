#include <stdio.h>
#include "mergingdigest.h"

#define STREAM_SIZE 1000000

static inline double randMToN(double M, double N)
{
    return M + (rand() / (RAND_MAX / (N - M)));
}

int main()
{
    td_histogram_t *mdigest = td_new(500);
    printf("compression is %f capacity is %d\n", mdigest->compression, mdigest->cap);
    double seeds[STREAM_SIZE];
    for (int i = 0; i < STREAM_SIZE; ++i)
    {
        seeds[i] = randMToN(0, 1);
    }

    for (int i = 0; i < STREAM_SIZE; ++i)
    {
        td_add(mdigest, seeds[i], 1);
    }

    for (int i = 0; i < 10; ++i)
    {
        const double v = seeds[i];
        printf("value %f is at percentile %f\n", v, td_quantile_of(mdigest, v));
    }
    printf("\n");
    for (int i = 0; i <= 100; i += 10)
    {
        printf("%d percentile has value %f\n", i, td_value_at(mdigest, i / 100.0));
    }
}