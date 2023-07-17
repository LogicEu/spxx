#!/bin/bash

cc=gcc
std=-std=c89
opt=-O2
inc=(-I.)
ipath=/usr/local/include
modules=$(git submodule | awk '{ print $2 }')

for m in ${modules[*]}; do inc+=(-I$m); done

lib=(
    -lglfw 
    -lfreetype
    -ljpeg
    -lpng
    -lz
    -lm
)

wflag=(
    -Wall
    -Wextra
    -pedantic
)

linux=(
    -lGL
    -lGLEW
)

mac=(
    -framework OpenGL
)

cmd() {
    echo "$@" && $@
}

compile() {
    if echo "$OSTYPE" | grep -q "darwin"; then
        os=${mac[*]}
    elif echo "$OSTYPE" | grep -q "linux"; then
        os=${linux[*]}
    else
        echo "This OS is not supported by this build script yet..."
    fi
    
    cmd $cc $std $opt ${wflag[*]} ${inc[*]} ${lib[*]} ${os[*]} $1
}

cleanf() {
    [ -f $1 ] && cmd rm $1
}

clean() {
    cleanf a.out
    return 0
}

install() {
    [ "$EUID" -ne 0 ] && echo "Run with 'sudo' to install" && exit
    [ ! -d $ipath] && echo "install directory 'ipath' not found '$ipath'" && exit
    cmd cp spxx.h $ipath
    for m in ${modules[*]}; do cmd cp $m/$m.h $ipath; done
    echo "Successfully installed spxx"
    return 0
}

uninstall() {
    [ "$EUID" -ne 0 ] && echo "Run with 'sudo' to uninstall" && exit
    [ ! -d $ipath] && echo "install directory 'ipath' not found '$ipath'" && exit
    cleanf $ipath/spxx.h
    for m in ${modules[*]}; do cleanf $ipath/$m.h; done
    echo "Successfully uninstalled spxx"
    return 0
}

fail() {
    echo "file '$1' not found" && exit 
}

if (( $# < 1 )); then 
    echo "Enter a file to compile with spxx"
    echo "Use 'install' to install header files in $ipath"
    echo "Use 'clean' to remove local builds"
    exit
fi

case "$1" in
    "clean")
        clean;;
    "install")
        install;;
    "uninstall")
        uninstall;;
    "run")
        shift
        [ -f $1 ] && compile $1 && ./a.out || fail $1;;
    *)
        [ -f $1 ] && compile $1 || fail $1;;
esac
