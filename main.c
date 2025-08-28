#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h> // necessary for mlockall
#include <signal.h>

uint64_t resolution = 1000; // 1 = ns, 1000 = us, 1000000 = ms
uint64_t expected_wcet = 80000;
uint64_t* values;
uint64_t array_size;

void print_statistics()
{
    for(size_t i = 0; i < array_size; i++)
    {
         if(values[i]) printf("[%d ns] %d\n", i*resolution, values[i]);
    }

    exit(0);
}

void add_histogram()
{

}
int main(void) {
    int ret = mlockall(MCL_CURRENT | MCL_FUTURE);
    signal(SIGINT, print_statistics);
    if (ret) {
        return 1;
    }
    struct timespec ts;
     
    array_size = expected_wcet / resolution;
    values = malloc(sizeof(uint64_t) * array_size);
    
    if(values == NULL) return 1;
    memset(values, 0, array_size * sizeof(uint64_t));

	size_t attempts = 1000000000;

    uint64_t current = -1;
    bool val1 = false;
    bool val2 = false;

	for(size_t i = 0; i < attempts; i++)
	{
        val1 = clock_gettime(CLOCK_MONOTONIC, &ts);
        uint64_t now = ts.tv_sec * 1000000000ULL + ts.tv_nsec;
        if(current != -1)
        {
            uint64_t cet = now-current;
            if(cet / resolution > array_size)
            {
                values[array_size-1] += 1;
            } else
            {
                values[cet / resolution] +=1;
            }
            if(val1 || val2) return 1;

        }
        val2 = val1;
		current = now;
	}
    print_statistics();
    return 0;
}

