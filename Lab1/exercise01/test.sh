#!/bin/bash

for program in build/out/*.out; do
    if ! "$program" "$@"; then
        echo "$program failed"
    fi
done
