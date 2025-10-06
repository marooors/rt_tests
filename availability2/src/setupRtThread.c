/*
 * setupRtThread.c
 *
 *  Created on: Sep 19, 2025
 *      Author: root
 */

#include "setupRtThread.h"

int setupRtThread ( void ) {

	int s;
    struct sched_param param;
	pthread_t thread;
	cpu_set_t mask;

    // Set affinity to CPU 3
    CPU_ZERO(&mask);
    CPU_SET(3, &mask);
//    sched_setaffinity ( 0, sizeof(mask), &mask );
	thread = pthread_self();
	s = pthread_setaffinity_np(thread, sizeof(mask), &mask);
	if (s != 0) {
       perror("pthread_setaffinity_np failed");
       return EXIT_FAILURE;
	}
	/* Check the actual affinity mask assigned to the thread. */

	s = pthread_getaffinity_np(thread, sizeof(mask), &mask);
	if (s != 0) {
        perror("pthread_getaffinity_np failed");
        return EXIT_FAILURE;
	}
	printf("Set returned by pthread_getaffinity_np() contained:\n");
 	for (size_t j = 0; j < CPU_SETSIZE; j++) {
       if (CPU_ISSET(j, &mask	)) printf("    CPU %zu\n", j);
    }
    //////////////////////////////////////////////////////////////////

    // Run Process with Real-Time Priority
    // Set priority (must be between 1 and 99 for real-time)
	param.sched_priority = PROC_PRIO;
    // Try to set real-time FIFO scheduling
	if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler failed");
        return EXIT_FAILURE;
    }
    printf("Process is now running with SCHED_FIFO, priority %d\n", param.sched_priority);
    //////////////////////////////////////////////////////////////////

   // Lock Memory to Prevent Paging
    mlockall(MCL_CURRENT | MCL_FUTURE);
    //////////////////////////////////////////////////////////////////

    return 0;

}
