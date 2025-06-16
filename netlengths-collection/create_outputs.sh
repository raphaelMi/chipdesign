for file in SteinerInstances/*.txt; do
    cat "$file" | ./netlengths | tee "outputs/${file##*/}"
done