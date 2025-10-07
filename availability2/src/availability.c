/*
 ============================================================================
 Name        : availability.c
 Author      : hmr
 Version     : 2025-09-17
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "availability.h"
#include "setupRtThread.h"
#include "getCpuFrequency.h"

uint64_t  cpuFrequency;
uint32_t  frequencyDivisor;       

int main() {
	
    uint32_t diff;
    uint64_t histo[HIST_SIZE] = {0};
    uint64_t testRuns;
    uint32_t overflows=0;
    ov_type  ovValue[OVERF_SIZE];
    uint64_t start, end;
    struct timespec startTime,endTime;
    uint64_t nsDuration;
    double avg_sec_per_testRun, sec_per_histSlot;

	// Serup timing parameter
 	cpuFrequency     = getCpuFrequency();
 	frequencyDivisor = (cpuFrequency*HIST_SLOT_WIDTH) / (1000*1000*1000);
	#ifdef DEBUG
		printf("main(): received cpuFrequency: %lu calculated frequencyDivisor: %u\n",
		                         cpuFrequency,                frequencyDivisor );
	#endif


    testRuns = TEST_RUNS_SCALE * TEST_RUNS;

/*
 * Start of measurement
 * VVVVVVVVVVVVVVVVVVVV
 */
 
     /* read consecutive nanosecond values */
    clock_gettime(CLOCK_REALTIME,&startTime);

	for ( uint64_t i=0 ; i<testRuns ; i++ ) {
		start = read_tsc();
        end   = read_tscp();
		diff  = (end-start) / frequencyDivisor;  // diff in selected slot size
		if(i < testRuns*0.1) continue;
        if (diff < HIST_SIZE) {
			histo[diff]++;
		} else {
			if ( OVERF_SIZE > overflows ) {
				ovValue[overflows].iteration = i;
				ovValue[overflows].value     = diff;
				overflows++;
			} else {
				printf ( "\n More than %d overflowss\n", overflows );
				return EXIT_FAILURE;
			}
			
		}
    }
    
    /* read consecutive nanosecond values */
    clock_gettime(CLOCK_REALTIME,&endTime);

/*
 * ^^^^^^^^^^^^^^^^^^
 * End of measurement
 */

	sec_per_histSlot = 1.*frequencyDivisor / cpuFrequency;
	nsDuration          = 1000*1000*1000*(endTime.tv_sec-startTime.tv_sec) + endTime.tv_nsec-startTime.tv_nsec;
	avg_sec_per_testRun = ((double)nsDuration / testRuns) / (1000*1000*1000);
	
	printf("\n testRuns: %lu CPU frequency: %lu HIST_SIZE: %u frequency divisor: %u \n",
			   testRuns,     cpuFrequency,      HIST_SIZE,    frequencyDivisor );
	#ifdef PRETTY_HISTOGRAM
	    for ( int i=0 ; i<HIST_SIZE ; i++ ) {
	    	if (histo[i] != 0 ) {
				uint64_t h = i * HIST_SLOT_WIDTH;  // ns position of current slot
				if      ( h > 10000000 ) printf("%7.2f ms (%6i): %10lu\n", (double)((double)HIST_SLOT_WIDTH*i/1000000), i, histo[i] );
				else if ( h > 10000    ) printf("%7.2f us (%6i): %10lu\n", (double)((double)HIST_SLOT_WIDTH*i/1000   ), i, histo[i] );
				else                     printf("%7d ns (%6i): %10lu\n", HIST_SLOT_WIDTH*i, i, histo[i] );
			}
	    }	
	#else
	    for ( int i=0 ; i<HIST_SIZE ; i++ ) {
	    	if (histo[i] != 0 ) printf(" %6i: %10lu\n", i, histo[i]);
	    }
	#endif
    printf("\n  %u overflows :\n", overflows);
    for ( int i=0 ; i<overflows ; i++ ) {
    	printf("%10f sec (iteration %10lu): ", avg_sec_per_testRun*ovValue[i].iteration, ovValue[i].iteration );
    	printf("val=%8u gap=%10f sec\n", ovValue[i].value, sec_per_histSlot*ovValue[i].value );
    }

    return 0;
}
 
