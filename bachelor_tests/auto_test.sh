#!/bin/bash

# Prüfen, ob ein Parameter übergeben wurde
if [ -z "$1" ]; then
  echo "Usage: $0 <sleep_seconds>"
  exit 1
fi

x=$1

# Kompilieren
echo "Kompiliere tsc_memwriter.c..."
gcc tsc_memwriter.c -o writer || { echo "Kompilierung fehlgeschlagen"; exit 1; }

echo "Kompiliere tsc_memreader.c..."
gcc tsc_memreader.c -o reader || { echo "Kompilierung fehlgeschlagen"; exit 1; }


# Erste Runde
echo "Starte writer auf CPU 4..."
taskset -c 4 ./writer
echo "Starte reader..."
reader_output1=$(./reader)
echo "Reader Ausgabe 1: $reader_output1"

# Sleep
echo "Warte $x Sekunden..."
sleep "$x"

# Zweite Runde
echo "Starte writer auf CPU 6..."
taskset -c 6 ./writer
echo "Starte reader..."
reader_output2=$(./reader)
echo "Reader Ausgabe 2: $reader_output2"

# Berechne Differenz
# Annahme: Reader gibt eine Zahl aus
diff=$((reader_output2 - reader_output1))
echo "Differenz der Reader-Ausgaben: $diff"
