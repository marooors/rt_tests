#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    struct timespec ts;
	struct timespec ts2;
    
    uint64_t resolution = 1000; // 1 = ns, 1000 = us, 1000000 = ms
    uint64_t expected_wcet = 80000; 
    uint64_t* values;
    
    uint64_t array_size = expected_wcet / resolution;
    values = malloc(sizeof(uint64_t) * array_size);
    
    if(values == NULL) return 1;
    memset(values, 0, array_size * sizeof(uint64_t));

	size_t attempts = 10000000000;
	uint64_t wcet = 0;

	for(size_t i = 0; i < attempts; i++)
	{
		bool val = clock_gettime(CLOCK_MONOTONIC, &ts);
		bool val2 = clock_gettime(CLOCK_MONOTONIC, &ts2);
		uint64_t cet = (ts2.tv_sec - ts.tv_sec) * 1000000000ULL + ts2.tv_nsec - ts.tv_nsec;
        
        if(cet / resolution > array_size)
        {
            values[array_size-1] += 1;
        } else
        {
            values[cet / resolution] +=1;
        }
		if(cet > wcet) wcet = cet;
		if(val || val2) return 1;
	}

    // CLOCK_REALTIME = aktuelle Systemzeit
    // CLOCK_MONOTONIC = monoton steigende Zeit (seit Boot)
	printf("WCET: %lu\n", wcet);
    for(size_t i = 0; i < array_size; i++)
    {
        if(values[i]) printf("[%d] %d\n", i, values[i]);
    }
    return 0;
}

