static __inline__ uint64_t measure_duration_inline(void)
{
    uint64_t result;
    __asm__ __volatile__ (
        // Startmessung
        "cpuid\n\t"
        "rdtsc\n\t"
        "shl $32, %%rdx\n\t"
        "or %%rdx, %%rax\n\t"
        "mov %%rax, %%r8\n\t"     // Save start in r8

        // <<< Hier würde dein Code stehen >>>

        // Endmessung
        "rdtscp\n\t"
        "shl $32, %%rdx\n\t"
        "or %%rdx, %%rax\n\t"
        "sub %%r8, %%rax\n\t"     // end - start
        "mov %%rax, %0\n\t"
        "cpuid\n\t"
        : "=r" (result)
        :
        : "%rax", "%rbx", "%rcx", "%rdx", "%r8", "memory"
    );
    return result;
}
