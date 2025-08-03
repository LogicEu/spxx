#!/bin/bash

mods=$(git submodule | awk '{print $2}')

cmd() {
    echo "$@" && $@
}

upgit() {
    for m in ${mods[*]}
    do
        pushd $m && git pull origin main && popd
    done
    return 0
}

headers() {
    for m in ${mods[*]}
    do
        cmd cp $m/$m.h spxx/$m.h
    done
    return 0
}

message() {
    echo -e "all\t: update git submodules and spxx headers"
    echo -e "git\t: update git submodules"
    echo -e "headers\t: update spxx headers from git submodules"
    return 0
}

if [ $# -eq 0 ]; then 
    echo "$0: usage" && message && exit
fi

case $1 in
    "git")
        upgit;;
    "update")
        headers;;
    "all")
        upgit && headers;;
    *)
        echo "$0: unknown option '$1'" && message && exit;;
esac

