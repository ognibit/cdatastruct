#!/bin/bash

# Run all tests

make || exit 1

TESTS=$(find tests -name 'test_*.exe')

for t in $TESTS
do
    echo "Run $t..."
    ${t}
done
