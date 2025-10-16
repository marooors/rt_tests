#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>    // O_* Konstanten
#include <sys/mman.h> // shm_open, mmap
#include <sys/stat.h> // mode constants
#include <unistd.h>   // ftruncate, close
#include <signal.h>

#include "timing_services.h"

#define SHM_NAME "/TSC_SHM"
#define SIZE 1024

uint64_t current_tsc;

void handle_sigint(int) {
    exit(0);
}


int main() {
    signal(SIGINT, handle_sigint);
    // Shared Memory Objekt erstellen
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Größe festlegen
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        exit(1);
    }

    // Speicherbereich mappen
    void *ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    uint64_t *tsc_pointer = (uint64_t*) ptr;
    for(size_t i = 0; i < 1;)
    {
	current_tsc = read_tsc();
	*tsc_pointer = current_tsc;
    }
    // Nachricht schreiben
    const char *message = "Hallo Leser!";
    sprintf(ptr, "%s", message);
    printf("Schreiber: Nachricht '%s' wurde geschrieben.\n", message);

    printf("Drücke Enter zum Beenden und Löschen...\n");
    getchar();

    // Aufräumen
    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);

    return 0;
}
