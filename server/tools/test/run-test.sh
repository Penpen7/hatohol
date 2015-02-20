#!/bin/sh

export BASE_DIR="`dirname $0`"
tools_dir="$BASE_DIR/.."

if test x"$NO_MAKE" != x"yes"; then
    if which gmake > /dev/null; then
        MAKE=${MAKE:-"gmake"}
    else
        MAKE=${MAKE:-"make"}
    fi
    MAKE_ARGS=
    case `uname` in
        Linux)
            MAKE_ARGS="-j$(grep '^processor' /proc/cpuinfo | wc -l)"
            ;;
        *)
            :
            ;;
    esac
    $MAKE $MAKE_ARGS -C $tools_dir/ hatohol/hatohol_def.py || exit 1
fi

export PYTHONPATH=..
python -m unittest discover -p 'Test*.py'
