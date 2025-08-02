# Script to delete all empty files in a directory and log their names to a file
#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Wrong number of arguments"
    exit 1
fi

katalog="$1"
plik="$2"

shopt -s dotglob

for file in "$katalog"/*; do
    # Checking whether the file is empty, if so its name is written to a file and then is removed from directory (katalog)
    if [[ -f "$file" && $(wc -l < "$file") -eq 0 ]]; then 
        echo "$file" >> "$plik"
        rm "$file"
        echo "File was removed"
    fi
done
