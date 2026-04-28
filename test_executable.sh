#!/usr/bin/env bash

main() {
    if [[ $# -lt 1 ]]; then
        1>&2 echo "Usage: $0 {executable} [additional test 1] [...]"
        exit 1
    fi

    EXECUTABLE=$1
    shift

    echo "test_name,input_size,is_correct,cpu_ticks"

    seq 10000 | test sorted
    seq 10000 1 | test reversed
    for _ in $(seq 10000); do
        echo $RANDOM
    done | test random

    while [[ $# -gt 0 ]]; do
        name=$(basename $1 .in)
        test $name < $1
        shift
    done
}

test() {
    local name=$1
    local best_time
    local correct=0

    local input="$(cat -)"

    for i in $(seq 5); do
        local result
        result=$(PATH="." ${EXECUTABLE} <<< "$input")
        local status=$?

        if [[ $status -eq 0 ]]; then
            correct=1
        fi

        if [[ -z $best_time ]] || [[ $result -lt $best_time ]]
        then
            best_time=$result
        fi
    done

    local count=$(wc -w <<< "$input" | xargs)
    echo "$name,$count,$correct,${best_time:-"-1"}"
}

main "$@"
