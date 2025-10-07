#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

#include <linux/sched/types.h>
#include <linux/sched.h>

#include <linux/tick.h>
#include <linux/irq.h>
#include <linux/interrupt.h> // per cpu irq disable/enable

#include <linux/rcupdate.h>
#include <linux/context_tracking.h>

#include <linux/trace_clock.h>
#include <linux/debugfs.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/sched/clock.h>
#include <linux/delay.h>

#define time_sub(a,b)   ((a) - (b))
#define AMOUNT 30000000000ULL
#define EXPECTED_WCET 5000000
#define RESOLUTION 1

MODULE_AUTHOR("roma479");
MODULE_DESCRIPTION("Latency measurement module");
MODULE_LICENSE("GPL");

#define LATENCY_ARRAY_SIZE (EXPECTED_WCET / RESOLUTION + 1)
static s64 latencies[LATENCY_ARRAY_SIZE];

static struct dentry *debugfs_dir;
static struct dentry *debugfs_file;
static struct dentry *debugfs_trigger;

static struct task_struct *measure_thread;
static atomic_t trigger_flag = ATOMIC_INIT(0);

static int cpu_target = 9; // gewünschte CPU (z.B. 0)
void record_latency(s64);
void print_latencies(void);
uint64_t getCpuFrequency (void);
void record_latency(s64 nsecs)
{
    s64 index = nsecs / RESOLUTION;
    if (index >= LATENCY_ARRAY_SIZE)
        index = LATENCY_ARRAY_SIZE - 1;
    latencies[index]++;
}

void print_latencies(void)
{
    for (u64 i = 0; i < LATENCY_ARRAY_SIZE; i++) {
        if (latencies[i])
            printk(KERN_INFO "[%llu ns] %lld\n", i * RESOLUTION, latencies[i]);
    }
}

static ssize_t histogram_read(struct file *file, char __user* buffer,
                              size_t count, loff_t* offset)
{
    char buf[128];
    ssize_t ret = 0;

    while (*offset < LATENCY_ARRAY_SIZE && !latencies[*offset]) {
        (*offset)++;
    }

    if (*offset >= LATENCY_ARRAY_SIZE)
        return 0;

    if (latencies[*offset]) {
        int len = snprintf(buf, sizeof(buf), "[%lld ns] %lld\n",
                           (*offset) * RESOLUTION,
                           latencies[*offset]);
        if (copy_to_user(buffer, buf, len))
            return -EFAULT;
        *offset += 1;
        ret = len;
    }
    return ret;
}

static const struct file_operations fops = {
    .read = histogram_read,
};

/* trigger write: nur Flag setzen */
static ssize_t trigger_write(struct file *file, const char __user *buffer,
                             size_t count, loff_t *ppos)
{
    //atomic_set(&trigger_flag, 1);
    //wake_up_process(measure_thread);
    //return count;
    char kbuf[64];
    size_t len = min(count, sizeof(kbuf) -1);
    if (copy_from_user(kbuf, buffer, len))
    {
        return -EFAULT;
    }
    kbuf[len] = '\0';
    
    if (strncmp(kbuf, "0", 1) == 0)
    {
        atomic_set(&trigger_flag, 0);
    } else atomic_set(&trigger_flag, 1);
    wake_up_process(measure_thread);
    return count;
}

static const struct file_operations trigger_fops = {
    .write = trigger_write,
};


#define DEBUG
#define TEST_RUNS 5


static __inline__ uint64_t read_tsc(void) {
    unsigned int lo, hi;
    // Serialize with cpuid to ensure accurate measurement
    __asm__ __volatile__ (
        "cpuid\n\t"            // Serialize before rdtsc
        "rdtsc\n\t"            // Read TSC
        : "=a" (lo), "=d" (hi) // Output
        : "a" (0)              // Input: eax = 0
        : "%ebx", "%ecx");     // Clobbered registers
    return ((uint64_t)hi << 32) | lo;
}

static __inline__ uint64_t read_tscp(void)
{
    unsigned int lo,hi;

    __asm__ __volatile__ (
                    //"mfence\n\t" If software requires RDTSCP to be executed only after all previous stores are globally visible, it can execute MFENCE immediately before RDTSCP
                    "RDTSCP\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t"
                    "CPUID\n\t": "=r" (hi), "=r" (lo)::
                    "%rax", "%rbx", "%rcx", "%rdx");
    return ((uint64_t)hi << 32) | lo;
}


uint64_t getCpuFrequency (void) {

    uint64_t start, mean, results[TEST_RUNS];

    for ( int i=0 ; i<TEST_RUNS ; i++ ) {
                start = read_tsc();
//              ndelay(1000000000);
                usleep_range(1000000, 1000001);
                results[i] = read_tscp() - start;
    }

    mean = (uint64_t)0;
    for ( int i=0 ; i<TEST_RUNS ; i++ ) {
        mean += results[i];
                #ifdef DEBUG
                printk(KERN_INFO "getCpuFrequency(): value %2i: %llu\n", i, results[i]);
                #endif
    }
    mean = mean / TEST_RUNS;
        #ifdef DEBUG
                printk (KERN_INFO "getCpuFrequency(): returning %llu\n", mean );
        #endif

    return mean;
}



int blubb = 0;
volatile int* blubbptr = &blubb;
/* Mess-Thread */
static int measure_fn(void *data)
{
    printk(KERN_INFO "Starting measurement on CPU %d...\n", smp_processor_id());
    while (!kthread_should_stop()) {
        if (atomic_read(&trigger_flag)) {
            
            u64 t1, t2;
            s64 diff;
            memset(latencies, 0, sizeof(latencies));
            printk(KERN_INFO "Starting measurement on CPU %d...\n", smp_processor_id());
            //local_irq_disable();
            
            uint64_t  cpuFrequency;
            uint32_t  frequencyDivisor = 0;  
            cpuFrequency     = getCpuFrequency();
            frequencyDivisor = (cpuFrequency) / (1000*1000*1000);
            printk(KERN_INFO "main(): received cpuFrequency: %llu calculated frequencyDivisor: %u\n",
                                         cpuFrequency,                frequencyDivisor );
            ct_idle_enter();
            //disable_percpu_irq(0);
            //local_bh_disable();
            //preempt_disable();
            for (u64 i = 0; i < AMOUNT; i++) {
                //local_irq_disable();
                //local_bh_disable();
                //disable_percpu_irq(0);
                //disable_irq(0);
                //cond_resched();
                //t1 = trace_clock_local();
                //t2 = trace_clock_local();
                t1 = read_tsc();
                //usleep_range(10, 10);
                t2 = read_tscp();

                //enable_irq(0);
                //t1 = rdtsc();
                //t2 = rdtsc();
                //t1 = ktime_get_ns();
                //t2 = ktime_get_ns();
                //t1 = ktime_get_mono_fast_ns();
                //t2 = ktime_get_mono_fast_ns();
                //local_bh_enable();
                //local_irq_enable();
                // if(!atomic_read(&trigger_flag)) break; // Avoid saving the last measurement, as it could be disrupted by a manual stop
                if(!atomic_read(&trigger_flag)) break;
                diff = time_sub(t2, t1) / frequencyDivisor;
                record_latency(diff);
            }
            ct_idle_exit();
            printk(KERN_INFO "t1 and t2 %llu %llu", t1, t2);
            //enable_percpu_irq(0, IRQ_TYPE_NONE);
            //local_irq_enable();
            //preempt_enable();
            //rcu_read_unlock();
            //local_bh_enable();
            //rcu_read_unlock();
            printk(KERN_INFO "Measurement finished\n");
            print_latencies();

            atomic_set(&trigger_flag, 0);
        }
        set_current_state(TASK_INTERRUPTIBLE);
        schedule();
        //preempt_enable();
        //local_irq_enable();
    }
    return 0;
}

static int __init custom_init(void)
{
    debugfs_dir = debugfs_create_dir("measurements", NULL);
    if (!debugfs_dir)
        return -ENOMEM;

    debugfs_file = debugfs_create_file("histogram", 0444, debugfs_dir,
                                       NULL, &fops);
    debugfs_trigger = debugfs_create_file("trigger", 0222, debugfs_dir,
                                          NULL, &trigger_fops);

    /* Thread erzeugen */
    measure_thread = kthread_create(measure_fn, NULL, "measure_thread");
    if (IS_ERR(measure_thread))
        return PTR_ERR(measure_thread);

    /* Thread an CPU binden */
    kthread_bind(measure_thread, cpu_target);


    /* Realtime Priorität auf 1 setzen */
    //https://lwn.net/Articles/818388/
    //sched_set_fifo_low(measure_thread);


    wake_up_process(measure_thread);

    printk(KERN_INFO "Module loaded. Use echo 1 > /sys/kernel/debug/measurements/trigger to start.\n");
    return 0;
}

static void __exit custom_exit(void)
{
    if (measure_thread)
        kthread_stop(measure_thread);
    debugfs_remove_recursive(debugfs_dir);
    printk(KERN_INFO "Module unloaded\n");
}

module_init(custom_init);
module_exit(custom_exit);
