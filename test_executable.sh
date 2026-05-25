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
    seq 10000 -1 1 | test reversed
    for _ in $(seq 10000); do
        echo $RANDOM
    done | test random
    gen_almost_sorted 10000 | test almost_sorted
    gen_stepped 10000 | test stepped
    gen_many_duplicates 10000 | test many_duplicates
    gen_all_equal 10000 | test all_equal
    gen_with_negatives 10000 | test with_negatives

    while [[ $# -gt 0 ]]; do
        name=$(basename $1 .in)
        test $name < $1
        shift
    done
}

gen_almost_sorted() {
    local n=$1
    local swaps=$((n / 100))
    local arr=()
    local i k a b tmp
    for ((i = 1; i <= n; i++)); do
        arr+=("$i")
    done
    for ((k = 0; k < swaps; k++)); do
        a=$((RANDOM % n))
        b=$((RANDOM % n))
        tmp=${arr[a]}
        arr[a]=${arr[b]}
        arr[b]=$tmp
    done
    printf '%s\n' "${arr[@]}"
}

gen_stepped() {
    local n=$1
    local step_up=$((n / 20))
    local step_down=$((step_up / 4))
    local val=0 count=0 i
    while ((count < n)); do
        for ((i = 0; i < step_up && count < n; i++)); do
            val=$((val + 1))
            echo $val
            count=$((count + 1))
        done
        for ((i = 0; i < step_down && count < n; i++)); do
            val=$((val - 1))
            echo $val
            count=$((count + 1))
        done
    done
}

gen_many_duplicates() {
    local n=$1
    local i
    for ((i = 0; i < n; i++)); do
        echo $((RANDOM % 10))
    done
}

gen_all_equal() {
    local n=$1
    yes 1 | head -n "$n"
}

gen_with_negatives() {
    local n=$1
    local i
    for ((i = 0; i < n; i++)); do
        echo $(( (RANDOM % 2 == 0 ? 1 : -1) * RANDOM ))
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
