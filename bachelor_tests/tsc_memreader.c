#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

// Präprozessor-Konstanten
#define SHM_NAME "/TSC_SHM"
#define SHM_SIZE 1024

int main() {
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
    uint64_t value = 0;
    uint64_t* tsc_ptr = (uint64_t*) ptr;

    for(size_t i = 0; i < 1;) {
    	printf("%lu\n", *tsc_ptr);
    }
    // Nachricht lesen

    printf("Leser: Gelesene Nachricht: '%s'\n", (char *)ptr);

    // Aufräumen
    munmap(ptr, SHM_SIZE);
    close(shm_fd);

    return 0;
}
