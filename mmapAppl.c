/*
 ============================================================================
 Name        : mmapAppl.c
 Author      : hmr
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int main() {

        uint32_t out1=0xff;
        uint32_t out2=0x0;
        uint32_t in;
        uint32_t data[] ={
                 0x0
                        ,0x0
                        ,0x0
                        ,0x0
                ,0xff
        };
        int used = sizeof(data)/4;
        off_t phys_addr = 0xd9e01000;

        puts("mmapAppl: Toggle endlessly pin D0 of lpt:");

        int fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (fd < 0) {
                perror("open");
                return 1;
        }

        size_t map_size = 0x1000; // 4kB
        void *map_base = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, phys_addr);
        if (map_base == MAP_FAILED) {
                perror("mmap");
                close(fd);
                return 1;
        }

        volatile uint32_t *reg = (volatile uint32_t *)map_base;

        printf("PA: %X LA: %X\n", phys_addr, map_base);

        while (1) {
                *reg = out1;
                in = *reg;
                *reg = out2;
                in = *reg;
        }

        munmap(map_base, map_size);
        close(fd);
        return 0;
}
