#include <benchmark/benchmark.h>
#include "mergingdigest.h"
#include <math.h>
#include <random>

#ifdef _WIN32
#pragma comment(lib, "Shlwapi.lib")
#ifdef _DEBUG
#pragma comment(lib, "benchmarkd.lib")
#else
#pragma comment(lib, "benchmark.lib")
#endif
#endif

int64_t min_value = 1;
int64_t min_compression = 100;
int64_t max_compression = 500;
int64_t step_compression_unit = 100;

static void generate_arguments_pairs(benchmark::internal::Benchmark *b)
{
    for (int64_t compression = min_compression; compression <= max_compression; compression += step_compression_unit)
    {
        b = b->ArgPair((double)compression, INT64_C(10000000));
    }
}

static inline double randMToN(double M, double N)
{
    return M + (rand() / (RAND_MAX / (N - M)));
}

static void BM_hdr_record_values(benchmark::State &state)
{
    const double compression = state.range(0);
    const int64_t stream_size = state.range(1);
    td_histogram_t *mdigest = td_new(compression);
    std::vector<double> input;
    input.resize(stream_size, 0);
    std::mt19937_64 rng;
    rng.seed(std::random_device()());
    std::uniform_real_distribution<double> dist(0, 1);

    for (double &i : input)
    {
        i = dist(rng);
    }

    // benchmark::DoNotOptimize(mdigest->);

    while (state.KeepRunning())
    {
        for (int i = 0; i < stream_size; ++i)
        {
            td_add(mdigest, input[i], 1);
        }
        state.SetItemsProcessed(stream_size);
        // Set the counter as a thread-average quantity. It will
        // be presented divided by the number of threads ( in our case just one thread ).
        merge(mdigest);
        state.counters["Centroid_Count"] = benchmark::Counter(td_number_centroids(mdigest), benchmark::Counter::kAvgThreads);
        // read/write barrier
        benchmark::ClobberMemory();
    }
}

// Register the functions as a benchmark
BENCHMARK(BM_hdr_record_values)->Apply(generate_arguments_pairs);

BENCHMARK_MAIN();