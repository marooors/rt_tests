/*
 * availability.h
 *
 *  Created on: Sep 19, 2025
 *      Author: root
 */

#ifndef AVAILABILITY_H_
#define AVAILABILITY_H_

#define DEBUG
#define PRETTY_HISTOGRAM

#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <err.h>


#include "timingServices.h"


#define TEST_RUNS       400        // number of test runs [in millions]
#define TEST_RUNS_SCALE ((uint64_t)1000000)  // needed for above
#define HIST_SIZE       10000       // size (number of entries) of the histogram
#define OVERF_SIZE      1000       // number of recordable overflows
#define HIST_SLOT_WIDTH 1        // time interval covered by one histogram slot [ns]


struct ov_node {          // element of the overflow arry
    uint64_t iteration;
    uint32_t value;
};
typedef struct ov_node ov_type;



#endif /* AVAILABILITY_H_ */
