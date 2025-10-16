#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>


#include "timing_services.h"
// Präprozessor-Konstanten
#define SHM_NAME "/TSC_SHM"
#define SHM_SIZE 1024


#define HIST_SIZE 10000

uint64_t histogram[HIST_SIZE] = {0};

void print_histogram(int)
{
    for(size_t i = 0; i < HIST_SIZE; i++) {
    	if(histogram[i]) printf("[%luns]: %lu\n", i, histogram[i]);
    }
    exit(0);
}


int main() {
    
    signal(SIGINT, print_histogram);
    // Shared Memory Objekt öffnen
    
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Speicherbereich mappen
    void *ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    uint64_t read1, read2 = 0;
    uint64_t* tsc_ptr = (uint64_t*) ptr;
    uint64_t diff;
    for(size_t i = 0; i < 1;) {
	read1 = *tsc_ptr;
	//read2 = *tsc_ptr;
	read2 = read_tsc();
	printf("%lu\n", read1);
	exit(0);
	diff = read2-read1;
	if(diff < HIST_SIZE)
	{
	    histogram[diff]++;
	} else {
	    histogram[HIST_SIZE-1]++;
	}
    }
    
    print_histogram(0);

    // Aufräumen
    munmap(ptr, SHM_SIZE);
    close(shm_fd);

    return 0;
}
