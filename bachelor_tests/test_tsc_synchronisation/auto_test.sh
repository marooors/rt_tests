#!/bin/bash

# Parameter Handling
if [ $# -ne 3 ]; then
  echo "Usage: $0 <sleep_seconds> <cpu1> <cpu2>"
  exit 1
fi

sleep_time=$1
cpu1=$2
cpu2=$3


# Kompilieren
echo "Kompiliere tsc_memwriter.c..."
gcc tsc_memwriter.c -o writer || { echo "Kompilierung fehlgeschlagen"; exit 1; }

echo "Kompiliere tsc_memreader.c..."
gcc tsc_memreader.c -o reader || { echo "Kompilierung fehlgeschlagen"; exit 1; }


# Erste Runde
echo "Starte writer auf CPU $cpu1..."
taskset -c $cpu1 ./writer
echo "Starte reader..."
reader_output1=$(./reader)
echo "Reader Ausgabe 1: $reader_output1"

# Sleep
echo "Warte $sleep_time Sekunden..."
sleep "$sleep_time"

# Zweite Runde
echo "Starte writer auf CPU $cpu2..."
taskset -c $cpu2 ./writer
echo "Starte reader..."
reader_output2=$(./reader)
echo "Reader Ausgabe 2: $reader_output2"

# Berechne Differenz
# Annahme: Reader gibt eine Zahl aus
diff=$((reader_output2 - reader_output1))
echo "Differenz der Reader-Ausgaben: $diff"
