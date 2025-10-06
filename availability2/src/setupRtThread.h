/*
 * setupRtThread.h
 *
 *  Created on: Sep 19, 2025
 *      Author: root
 */

#ifndef SETUPRTTHREAD_H_
#define SETUPRTTHREAD_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <err.h>

#include <pthread.h>
#include <sys/param.h>
#include <sched.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>

#include "timingServices.h"

#define PROC_PRIO       80         // thread priprity during test run

int setupRtThread ( void );

#endif /* SETUPRTTHREAD_H_ */
