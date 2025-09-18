#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

#include <linux/trace_clock.h>
#include <linux/spinlock.h>
#include <linux/irqflags.h>
#include <linux/hardirq.h>
#include <linux/module.h>
#include <linux/percpu.h>
#include <linux/sched.h>
#include <linux/sched/clock.h>
#include <linux/ktime.h>
#include <linux/trace_clock.h>

#include <linux/debugfs.h>
#include <linux/uaccess.h>

#define time_sub(a,b)   ((a) - (b))
#define time_to_us(x)   div_u64(x, 1000)
#define AMOUNT 100000000000// 1,1h-> 200000000000
#define EXPECTED_WCET 50000
#define RESOLUTION 1
MODULE_AUTHOR("roma479");
MODULE_DESCRIPTION("-");
MODULE_LICENSE("GPL");

#define ARRAY_SIZE (EXPECTED_WCET / RESOLUTION + 1)
static s64 latencies[ARRAY_SIZE];
static struct dentry *debugfs_dir;
static struct dentry *debugfs_file;

static ssize_t histogram_read(struct file *file, char __user* buffer, size_t count, loff_t* offset)
{
    ssize_t ret = 0;
    printk(KERN_INFO "TEST\n");
/*    char s[13] = "Hello World!\n";
    int len = sizeof(s);

    ret = len;

    if(*offset >= len || copy_to_user(buffer, s, len))
    {
        pr_info("copy to user failed\n");
        ret = 0;
    } else
    {
        *offset += len;
    }
*/
/*
    char* buf;
    buf = kmalloc(4096, GFP_KERNEL);
    strcpy(buf, "hallo\n");
    int len = 6;
    printk(KERN_INFO "[%s]\n", buf);
    if(*offset >= len || copy_to_user(buffer, buf, len))
    {
        pr_info("copy to user failed\n");
        ret = 0;
    } else
    {
         ret = len;
         *offset += len;
        pr_info("c2u success\n");
    }
    kfree(buf);
    
  */
    char buf[128];
    while(!latencies[*offset])
    {
        (*offset)++;
    }
    if(*offset >= ARRAY_SIZE)
    {
         return 0;
    }

    if(1)//latencies[*offset])
    {
        int len = snprintf(buf, sizeof(buf), "[%ld ns] %lld\n", (*offset)*RESOLUTION, latencies[*offset]);//latencies[i]);
        if(copy_to_user(buffer, buf, len))
        {
            pr_info("copy to user failed\n");
            ret = 0;
        } else
        {
            *offset+=1;
            ret = len;
        }
    }
/* 
    char *buf;
    int len = 0;
    int size = 4096;
    buf = kmalloc(size, GFP_KERNEL);
    if(!buf) return -ENOMEM;

    for(int i = 0; i <= EXPECTED_WCET / RESOLUTION; i++)
    {
        if(latencies[i]) scnprintf(buf+len, size-len, "[%ld ns] %lld\n", i*RESOLUTION, latencies[i]);
    }

    ret = simple_read_from_buffer(buffer, count, offset, buf, len);
    kfree(buf);
    */return ret;
}
//DEFINE_SIMPLE_ATTRIBUTE(fops, histogram_read /*read*/, NULL /*write*/, "%llu\n");
static const struct file_operations fops = {
    .read = histogram_read,
};

void record_latency(s64 nsecs)
{
    s64 index = nsecs / RESOLUTION;
    if(index > ARRAY_SIZE)
        index = ARRAY_SIZE-1;

    latencies[index]++;
}

void print_latencies(void)
{
    for(u64 i = 0; i < ARRAY_SIZE; i++)
    {
        if(latencies[i]) printk(KERN_INFO "[%ld ns] %lld\n", i*RESOLUTION, latencies[i]);
    }
}

static int __init custom_init(void)
{
    debugfs_dir = debugfs_create_dir("measurements", NULL);
    if(!debugfs_dir) return -ENOMEM;

    debugfs_file = debugfs_create_file("histogram", 0444, debugfs_dir, NULL, &fops);
    if(!debugfs_file) return -ENOMEM;

    printk(KERN_INFO "Hello World!\n");
    u64 t1, t2;
    s64 diff;

    for(u64 i = 0; i < AMOUNT; i++)
    {
        local_irq_disable();
        t1 = trace_clock_local();
        t2 = trace_clock_local();
        local_irq_enable();
        diff = (time_sub(t2, t1));
        record_latency(diff);    
    }
    printk(KERN_INFO "Time: %lld", diff);
    print_latencies();
    return 0;
}

static void __exit custom_exit(void)
{
    printk(KERN_INFO "Bye World");
    debugfs_remove_recursive(debugfs_dir);
}

module_init(custom_init);
module_exit(custom_exit);
