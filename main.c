#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

int main(void) {
    	struct timespec ts;
	struct timespec ts2;

	size_t attempts = 100000000;
	uint64_t wcet = 0;
	for(size_t i = 0; i < attempts; i++)
	{
		bool val = clock_gettime(CLOCK_MONOTONIC, &ts);
		bool val2 = clock_gettime(CLOCK_MONOTONIC, &ts2);
		uint64_t cet = (ts2.tv_sec - ts.tv_sec) * 1000000000ULL + ts2.tv_nsec - ts.tv_nsec;
		if(cet > wcet) wcet = cet;
		if(val || val2) return 1;
	}

    // CLOCK_REALTIME = aktuelle Systemzeit
    // CLOCK_MONOTONIC = monoton steigende Zeit (seit Boot)
	printf("WCET: %lu\n", wcet);
    return 0;
}

