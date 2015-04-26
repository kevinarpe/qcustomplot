#!/usr/bin/env bash

main()
{
    local cwd="$(readlink -f "$(dirname "$0")")"
    source "$cwd/buildlib.bash_library"

    local qt_flag="$1" ; shift
    # Optional remaining args in "$@"; might be zero!

    # Amalgamate source code
    buildlib_echo_and_run_cmd "$cwd/../run-amalgamate.sh"

    buildlib_echo_cmd cd "$cwd/sharedlib-compilation"
    cd "$cwd/sharedlib-compilation"

    # Build libqcustomplot.so
    buildlib_echo_and_run_cmd qmake "$qt_flag"
    buildlib_echo_and_run_cmd make clean
    buildlib_echo_and_run_cmd make "$@"

    buildlib_echo_cmd cd "$cwd/sharedlib-usage"
    cd "$cwd/sharedlib-usage"

    # Build C++ demo
    buildlib_echo_and_run_cmd qmake "$qt_flag"
    buildlib_echo_and_run_cmd make clean
    buildlib_echo_and_run_cmd make "$@"

    # Run C++ demo
    buildlib_echo_cmd LD_LIBRARY_PATH="$cwd/sharedlib-compilation" ./sharedlib-usage \&
    LD_LIBRARY_PATH="$cwd/sharedlib-compilation" ./sharedlib-usage &
    local child_pid=$!
    buildlib_sleep_kill_wait "$child_pid" 'C++'

    printf -- '\nDone.\n'
}

main "$@"

