#!/bin/bash

# remove all  prints from the minishell project
# This will remove both DEBUG_PRINT macros and fprintf(stderr, "DEBUG:...) statements

find . -type f -name "*.c" -o -name "*.h" | xargs sed -i 's/fprintf(stderr, "DEBUG:.*);/\/\/ DEBUG removed/g'
find . -type f -name "*.c" -o -name "*.h" | xargs sed -i 's/DEBUG_PRINT(.*);/\/\/ DEBUG removed/g'

echo "Debug prints removed successfully!"
