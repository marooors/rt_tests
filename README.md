VIM
cp .vimrc to ~/.vimrc
Start with: sudo taskset -c 3 chrt -r 90 make

Ergebnisse: (Ausreißer wild)
mario@rocky:~/test_programs$ sudo taskset -c 2 chrt -r 90 make
[sudo] password for mario: 
gcc -O0 -Wall    main.c   -o main
main.c: In function ‘main’:
main.c:47:33: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘size_t’ {aka ‘long unsigned int’} [-Wformat=]
   47 |         if(values[i]) printf("[%d] %d\n", i, values[i]);
      |                                ~^         ~
      |                                 |         |
      |                                 int       size_t {aka long unsigned int}
      |                                %ld
main.c:47:37: warning: format ‘%d’ expects argument of type ‘int’, but argument 3 has type ‘uint64_t’ {aka ‘long unsigned int’} [-Wformat=]
   47 |         if(values[i]) printf("[%d] %d\n", i, values[i]);
      |                                    ~^        ~~~~~~~~~
      |                                     |              |
      |                                     int            uint64_t {aka long unsigned int}
      |                                    %ld
./main
WCET: 140424
[0] 999975610
[1] 4623
[2] 543
[3] 5228
[4] 3374
[5] 2923
[6] 1623
[7] 2427
[8] 2557
[9] 286
[10] 187
[11] 212
[12] 169
[13] 80
[14] 44
[15] 17
[16] 19
[17] 14
[18] 12
[19] 10
[20] 3
[21] 2
[22] 2
[24] 1
[33] 1
[43] 1
[45] 6
[47] 4
[48] 1
[54] 2
[63] 2
[64] 1
[65] 1
[68] 1
[75] 2
[77] 1
[79] 9
mario@rocky:~/test_programs$ sudo taskset -c 3 chrt -r 90 make
./main
WCET: 56385
[0] 999999966
[1] 17
[2] 10
[3] 4
[5] 1
[7] 1
[56] 1
mario@rocky:~/test_programs$ sudo taskset -c 3 make
./main
WCET: 19020
[0] 999999966
[4] 14
[5] 3
[6] 5
[7] 9
[8] 2
[19] 1
mario@rocky:~/test_programs$ sudo taskset -c 3 chrt -r 90 make
./main
WCET: 31779
[0] 999999967
[1] 20
[2] 10
[4] 2
[31] 1
mario@rocky:~/test_programs$ sudo taskset -c 3 chrt -r 90 make
./main
WCET: 2758
[0] 999999964
[1] 24
[2] 12
mario@rocky:~/test_programs$ sudo taskset -c 3 chrt -r 90 make
./main
WCET: 5494
[0] 999999968
[1] 8
[2] 20
[3] 2
[4] 1
[5] 1

