#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "List.h"
#include "PoolResource.h"

enum
{
    NODE_COUNT = 1000000,
    NOOP_COUNT = 100000000,
    RANDOM_OP_COUNT = 4000000,
    REPEAT_COUNT = 10,
    WARMUP_COUNT = 2,
};

typedef struct BenchmarkStats
{
    uint64_t best_cycles;
    uint64_t total_cycles;
} BenchmarkStats;

typedef struct BenchmarkResult
{
    bool ok;
    uint64_t cycles;
    uint64_t checksum;
} BenchmarkResult;

static uint64_t prng_next(uint64_t* state)
{
    uint64_t value = *state;
    value ^= value >> 12;
    value ^= value << 25;
    value ^= value >> 27;
    *state = value;
    return value * 2685821657736338717ull;
}

static uint64_t read_tsc(void)
{
    uint32_t low = 0;
    uint32_t high = 0;

    __asm__ volatile("lfence\n\t"
                     "rdtsc\n\t"
                     "lfence"
        : "=a"(low), "=d"(high)
        :
        : "memory");

    return ((uint64_t)high << 32) | low;
}

static int monotonic_now(struct timespec* time)
{
#ifdef CLOCK_MONOTONIC_RAW
    return clock_gettime(CLOCK_MONOTONIC_RAW, time);
#else
    return clock_gettime(CLOCK_MONOTONIC, time);
#endif
}

static uint64_t nsec_elapsed(struct timespec begin, struct timespec end)
{
    constexpr uint64_t BILLION = 1000000000;
    return (uint64_t)(end.tv_sec - begin.tv_sec) * BILLION
        + (uint64_t)(end.tv_nsec - begin.tv_nsec);
}

static double measure_tsc_ghz(uint64_t* cycles, uint64_t* elapsed_nsec)
{
    struct timespec begin_time = {};
    struct timespec end_time = {};

    if (monotonic_now(&begin_time) != 0)
    {
        return 0.0;
    }

    uint64_t begin_cycles = read_tsc();

    for (size_t i = 0; i < NOOP_COUNT; ++i)
    {
        __asm__ volatile("nop" ::: "memory");
    }

    uint64_t end_cycles = read_tsc();

    if (monotonic_now(&end_time) != 0)
    {
        return 0.0;
    }

    *cycles = end_cycles - begin_cycles;
    *elapsed_nsec = nsec_elapsed(begin_time, end_time);
    return (double)*cycles / (double)*elapsed_nsec;
}

static BenchmarkResult run_list_benchmark(MemoryResource* resource,
    bool destroy_list)
{
    BenchmarkResult result = {};
    volatile uint64_t checksum = 0;
    ListNode** nodes = (ListNode**)calloc(NODE_COUNT, sizeof(*nodes));
    size_t live_count = 0;
    uint64_t random_state = 0x123456789abcdef0ull;

    list_ctor(list, resource);

    uint64_t begin_cycles = read_tsc();

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        ListNode* node = list_insert_after(list, list_end(list), i);
        if (!node)
        {
            if (destroy_list)
            {
                list_dtor(list);
                free(nodes);
            }
            return result;
        }
        nodes[live_count++] = node;
    }

    for (size_t i = 0; i < RANDOM_OP_COUNT; ++i)
    {
        uint64_t random = prng_next(&random_state);
        size_t index = random % live_count;
        ListNode* node = nodes[index];
        int* value = list_node_get_value(node, int);

        switch (random & 3)
        {
            case 0:
                checksum += (uint64_t)*value;
                break;

            case 1:
                *value = (int)(random >> 32);
                checksum += (uint64_t)*value;
                break;

            case 2:
            {
                ListNode* new_node =
                    list_insert_after(list, node, (int)(random >> 32));
                if (!new_node)
                {
                    if (destroy_list)
                    {
                        list_dtor(list);
                        free(nodes);
                    }
                    return result;
                }

                if (live_count < NODE_COUNT)
                {
                    nodes[live_count++] = new_node;
                }
                else
                {
                    list_erase(list, nodes[index]);
                    nodes[index] = new_node;
                }
                break;
            }

            default:
            {
                if (live_count <= 1)
                {
                    break;
                }

                list_erase(list, node);
                nodes[index] = nodes[--live_count];
                break;
            }
        }
    }

    if (destroy_list)
    {
        list_dtor(list);
    }

    uint64_t end_cycles = read_tsc();

    free(nodes);

    result.ok = true;
    result.cycles = end_cycles - begin_cycles;
    result.checksum = checksum;
    return result;
}

static BenchmarkResult run_malloc_sample(void)
{
    return run_list_benchmark(get_malloc_resource(), true);
}

static BenchmarkResult run_pool_sample(void)
{
    Result_PoolResource resource = pool_resource_ctor(NODE_COUNT);
    if (resource.error_code != EVERYTHING_FINE)
    {
        return (BenchmarkResult) {};
    }

    BenchmarkResult result = run_list_benchmark(&resource.value.base, false);
    pool_resource_dtor(&resource.value);

    return result;
}

static void print_sample(const char* name,
    size_t run_index,
    uint64_t cycles,
    double tsc_ghz)
{
    double millis = tsc_ghz > 0.0 ? (double)cycles / (tsc_ghz * 1000000.0)
                                  : 0.0;
    printf("%-6s run %3zu: %12" PRIu64 " cycles", name, run_index, cycles);
    if (tsc_ghz > 0.0)
    {
        printf("  %9.3f ms", millis);
    }
    printf("\n");
}

static bool benchmark_resource(const char* name,
    BenchmarkResult (*run_sample)(void),
    double tsc_ghz,
    BenchmarkStats* stats)
{
    *stats = (BenchmarkStats) {
        .best_cycles = UINT64_MAX,
    };

    for (size_t i = 0; i < WARMUP_COUNT; ++i)
    {
        BenchmarkResult result = run_sample();
        if (!result.ok)
        {
            fprintf(stderr, "%s warmup failed\n", name);
            return false;
        }
    }

    for (size_t i = 0; i < REPEAT_COUNT; ++i)
    {
        BenchmarkResult result = run_sample();
        if (!result.ok)
        {
            fprintf(stderr, "%s run failed\n", name);
            return false;
        }

        print_sample(name, i + 1, result.cycles, tsc_ghz);

        stats->total_cycles += result.cycles;
        stats->best_cycles = MIN(stats->best_cycles, result.cycles);
    }

    return true;
}

static void
print_summary(const char* name, BenchmarkStats stats, double tsc_ghz)
{
    uint64_t average_cycles = stats.total_cycles / REPEAT_COUNT;
    double best_ms = tsc_ghz > 0.0
        ? (double)stats.best_cycles / (tsc_ghz * 1000000.0)
        : 0.0;
    double average_ms = tsc_ghz > 0.0
        ? (double)average_cycles / (tsc_ghz * 1000000.0)
        : 0.0;

    printf("%-6s best: %12" PRIu64 " cycles", name, stats.best_cycles);
    if (tsc_ghz > 0.0)
    {
        printf("  %9.3f ms", best_ms);
    }
    printf("\n");

    printf("%-6s avg:  %12" PRIu64 " cycles", name, average_cycles);
    if (tsc_ghz > 0.0)
    {
        printf("  %9.3f ms", average_ms);
    }
    printf("\n");
}

int main(void)
{
    uint64_t noop_cycles = 0;
    uint64_t noop_nsec = 0;
    double tsc_ghz = measure_tsc_ghz(&noop_cycles, &noop_nsec);

    if (tsc_ghz > 0.0)
    {
        printf("tsc: %.3f GHz via %d nop loop\n", tsc_ghz, NOOP_COUNT);
        printf("noop: %" PRIu64 " cycles, %" PRIu64 " ns, %.3f cycles/iter\n",
            noop_cycles,
            noop_nsec,
            (double)noop_cycles / (double)NOOP_COUNT);
    }
    else
    {
        printf("tsc: calibration failed; printing cycles only\n");
    }
    printf("nodes: %d, repeats: %d, warmups: %d\n\n",
        NODE_COUNT,
        REPEAT_COUNT,
        WARMUP_COUNT);

    BenchmarkStats malloc_stats = {};
    BenchmarkStats pool_stats = {};

    if (!benchmark_resource("malloc", run_malloc_sample, tsc_ghz, &malloc_stats))
    {
        return 1;
    }
    printf("\n");

    if (!benchmark_resource("pool", run_pool_sample, tsc_ghz, &pool_stats))
    {
        return 1;
    }
    printf("\n");

    print_summary("malloc", malloc_stats, tsc_ghz);
    print_summary("pool", pool_stats, tsc_ghz);

    printf("\npool/malloc avg ratio: %.3f\n",
        (double)pool_stats.total_cycles / (double)malloc_stats.total_cycles);

    return 0;
}
