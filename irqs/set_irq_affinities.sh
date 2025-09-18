#!/bin/bash
LIST="$1" # read first parameter

COUNT_OK=0
COUNT_FAIL=0
if [ -z "$LIST" ]; then
    echo "USAGE: $0 <hex_mask>"
    exit 1
fi

for f in /proc/irq/*/smp_affinity_list;
do
    if echo "$LIST" > "$f" 2>/dev/null; then
        ((COUNT_OK++))
    else
        ((COUNT_FAIL++))
    fi
done

echo "FERTIG"
echo " OK: $COUNT_OK"
echo " FAILED: $COUNT_FAIL"
