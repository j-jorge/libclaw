#!/bin/bash

set -euo pipefail

if [[ $# -ne 1 ]] || [[ "$1" = "-h" ]] || [[ "$1" = "--help" ]]
then
    echo "Compile all example programs installed with Claw."
    echo "Usage: ${BASH_SOURCE[0]} INSTALL_PREFIX"
    exit
fi

install_prefix="$(cd "$1"; pwd)"

tmp_dir="$(mktemp --directory)"
trap 'rm --force --recursive "$tmp_dir"' EXIT

cd "$tmp_dir"

pass=()
fail=()

find "$install_prefix/share/doc/libclaw1/example" -name "*.tar.gz" \
    | ( while read -r archive
        do
            tar xf "$archive"
            example_name="$(basename "$archive" .tar.gz)"
            mkdir "$example_name-build"
            pushd "$example_name-build" >/dev/null

            cmake "../$example_name" -DCMAKE_PREFIX_PATH="$install_prefix"
            if cmake --build . --parallel $(nproc)
            then
                pass+=("$example_name")
            else
                fail+=("$example_name")
            fi

            popd >/dev/null
        done

        [[ ${#pass[@]} -eq 0 ]] \
            || printf '\033[0;32m[PASS]\033[0;0m %s\n' "${pass[@]}"

        [[ ${#fail[@]} -eq 0 ]] \
            || printf '\033[1;31m[FAIL]\033[0;0m %s\n' "${fail[@]}"

        printf '%d pass, %d fail.\n' ${#pass[@]} ${#fail[@]}
)
