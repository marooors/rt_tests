#!/bin/bash
tuned-adm profile network-latency
sh -c 'echo -1 > /proc/sys/kernel/sched_rt_runtime_us'

