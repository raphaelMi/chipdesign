# Create or clear the output CSV file
output_file="table.csv"
> "$output_file"

# Initialize an array to hold the contents of each file
declare -a contents

# Loop through each .txt file in the directory
for file in outputs/*.txt; do
    if [ -f "$file" ]; then
        # Get the filename without the .txt extension
        filename=$(basename "$file" .txt)
        
        # Read the content of the file into an array
        mapfile -t file_content < "$file"
        
        # Store the filename and its content in the contents array
        contents+=("$filename,${file_content[*]}")
    fi
done

# Transpose the contents and write to the CSV file
{
    # Print headers
    for entry in "${contents[@]}"; do
        IFS=',' read -r header rest <<< "$entry"
        echo -n "$header,"
    done
    echo

    # Determine the maximum number of lines across all files
    max_lines=0
    for entry in "${contents[@]}"; do
        IFS=',' read -r _ rest <<< "$entry"
        num_lines=$(echo "$rest" | tr ' ' '\n' | wc -l)
        if (( num_lines > max_lines )); then
            max_lines=$num_lines
        fi
    done

    # Print the transposed content
    for ((i=0; i<max_lines; i++)); do
        for entry in "${contents[@]}"; do
            IFS=',' read -r header rest <<< "$entry"
            line=$(echo "$rest" | tr ' ' '\n' | sed -n "$((i + 1))p")
            echo -n "${line:-},"
        done
        echo
    done
} > "$output_file"


