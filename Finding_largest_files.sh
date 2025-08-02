#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Wrong number of arguments"
    exit 1
fi

katalog="$1"
plik="$2"

find "$katalog" -type f -exec du -b {} + | sort -nr | head -n 2 | tee "$plik"
