#!/usr/bin/env bash

main() {
    if [[ $# -lt 1 ]]; then
        1>&2 echo "Usage: $0 {executable} [additional test 1] [...]"
        exit 1
    fi

    EXECUTABLE=$1
    shift

    echo "test_name,input_size,is_correct,cpu_ticks"

    for i in "small:100" "big:10000" "huge:10000000"; do
        IFS=':' read prefix size <<<"$i"
        # seq switches to scientific notation (e.g. 1e+07) on large values on macOS,
        # which scanf("%lld") cannot parse — use awk to guarantee decimal output
        awk -v n="$size" 'BEGIN { for (i=1; i<=n; i++) print i }' | test "${prefix}_sorted"
        awk -v n="$size" 'BEGIN { for (i=n; i>=1; i--) print i }' | test "${prefix}_reversed"
        seq $size | awk '{print int(rand()*9223372036854775807)}' | test "${prefix}_random"
        gen_almost_sorted $size | test "${prefix}_almost_sorted"
        gen_stepped $size | test "${prefix}_stepped"
        gen_many_duplicates $size | test "${prefix}_many_duplicates"
        gen_all_equal $size | test "${prefix}_all_equal"
        gen_with_negatives $size | test "${prefix}_with_negatives"
    done

    while [[ $# -gt 0 ]]; do
        name=$(basename $1 .in)
        test $name < $1
        shift
    done
}

gen_almost_sorted() {
    local n=$1
    local swaps=$((n / 100))
    seq 1 $n | awk -v swaps="$swaps" -v n="$n" '
        BEGIN { srand() }
        {
            a[NR] = $1
        }
        END {
            for (k = 0; k < swaps; k++) {
                i = int(rand() * n) + 1
                j = int(rand() * n) + 1
                tmp = a[i]
                a[i] = a[j]
                a[j] = tmp
            }
            for (i = 1; i <= n; i++) {
                print a[i]
            }
        }
    '
}

gen_stepped() {
    local n=$1
    local step_up=$((n / 20))
    local step_down=$((step_up / 4))
    awk -v n="$n" -v step_up="$step_up" -v step_down="$step_down" '
        BEGIN {
            val = 0
            count = 0
            while (count < n) {
                for (i = 0; i < step_up && count < n; i++) {
                    val++
                    print val
                    count++
                }
                for (i = 0; i < step_down && count < n; i++) {
                    val--
                    print val
                    count++
                }
            }
        }
    '
}

gen_many_duplicates() {
    local n=$1
    awk -v n="$n" 'BEGIN { srand(); for (i = 0; i < n; i++) print int(rand() * 10) }'
}

gen_all_equal() {
    local n=$1
    yes 1 | head -n "$n"
}

gen_with_negatives() {
    local n=$1
    awk -v n="$n" 'BEGIN { srand(); for (i = 0; i < n; i++) print int(rand()*9223372036854775807) * (rand() < 0.5 ? -1 : 1) }'
}

test() {
    local name=$1
    local best_time
    local correct=1

    local inputfile=$(mktemp)
    cat - > "$inputfile"

    for i in $(seq 5); do
        local result
        result=$(PATH="." ${EXECUTABLE} < "$inputfile")
        local status=$?

        if [[ $status -ne 0 ]]; then
            correct=0
            break
        fi

        if [[ -z $best_time ]] || [[ $result -lt $best_time ]]
        then
            best_time=$result
        fi
    done

    local count=$(wc -w < "$inputfile" | xargs)
    echo "$name,$count,$correct,${best_time:-"-1"}"

    rm "$inputfile"
}

main "$@"
