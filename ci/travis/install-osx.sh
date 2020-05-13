#!/usr/bin/env bash

brew update > /dev/null 2>&1

items=(
    "cmake"
    "lcov"
    "doxygen"
    "ninja"
)

for item in ${items[@]}
do
    if [ -z "$(which ${item})" ]; then
        echo "install ${item}"
        brew install "${item}" > /dev/null 2>&1
    else
        echo "update ${item}"
        brew upgrade "${item}" > /dev/null 2>&1
    fi
done
