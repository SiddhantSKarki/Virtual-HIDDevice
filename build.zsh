#!/bin/zsh
set -eu

cd -- "${0:A:h}"

if (( $# > 1 )); then
    print -u2 -- "Usage: zsh build.zsh [--remove]"
    exit 1
fi

case "${1:-}" in
    --remove)
        rm -rf -- build
        ;;
    "") ;;
    *)
        print -u2 -- "Usage: zsh build.zsh [--remove]"
        exit 1
        ;;
esac

if [[ ! -f build/CMakeCache.txt ]]; then
    cmake -S . -B build
fi

cmake --build build
