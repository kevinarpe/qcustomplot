#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status.
set -e
# Treat unset variables as an error when substituting.
set -u
# the return value of a pipeline is the status of
# the last command to exit with a non-zero status,
# or zero if no command exited with a non-zero status
set -o pipefail
# Print commands and their arguments as they are executed.
# set -x

main()
{
    if [ $# = 0 ]
    then
        printf -- 'ERROR: Missing QT_FLAG\n'
        printf -- '\n'
        printf -- 'Usage: "%s" QT_FLAG [MAKE_FLAG...]\n' "$0"
        printf -- '\n'
        printf -- 'REQUIRED ARGUMENTS\n'
        printf -- '    QT_FLAG: Used by qmake to set Qt version (4 or 5)\n'
        printf -- '    Example: -qt=4\n'
        printf -- '    Example: -qt=5\n'
        printf -- '\n'
        printf -- 'OPTIONAL ARGUMENTS\n'
        printf -- '    MAKE_FLAG: Passed directly to make\n'
        printf -- '    Example: -j 8\n'
        printf -- '\n'
        exit 1
    fi
    local qt_flag="$1" ; shift
    # Optional remaining args in "$@"; might be zero!

    local cwd="$(readlink -f "$(dirname "$0")")"

    # Amalgamate source code
    echo_and_run_cmd "$cwd/../run-amalgamate.sh"

    echo_cmd cd "$cwd/sharedlib-compilation"
    cd "$cwd/sharedlib-compilation"

    # Build libqcustomplot.so
    echo_and_run_cmd qmake "$qt_flag"
    echo_and_run_cmd make clean
    echo_and_run_cmd make "$@"

    echo_cmd cd "$cwd/sharedlib-usage"
    cd "$cwd/sharedlib-usage"

    # Build C++ demo
    echo_and_run_cmd qmake "$qt_flag"
    echo_and_run_cmd make clean
    echo_and_run_cmd make "$@"

    # Run C++ demo
    echo_cmd LD_LIBRARY_PATH="$cwd/sharedlib-compilation" ./sharedlib-usage \&
    LD_LIBRARY_PATH="$cwd/sharedlib-compilation" ./sharedlib-usage &
    local child_pid=$!
    sleep_kill_wait "$child_pid" 'C++'

    echo_cmd cd "$cwd/sharedlib-Py3Qt5"
    cd "$cwd/sharedlib-Py3Qt5"

    # Create Py3Qt5 C++ bindings
    local build_dir_path='build'
    echo_and_run_cmd rm -Rf "$build_dir_path"
    echo_and_run_cmd mkdir "$build_dir_path"
    echo_and_run_cmd './configure.py' "$build_dir_path"

    echo_cmd cd "$build_dir_path"
    cd "$build_dir_path"

    # Build Py3Qt5 qcustomplot.so
    echo_and_run_cmd make clean
    echo_and_run_cmd make "$@"

    # Run Py3Qt5 demo
    echo_cmd cd "$cwd/sharedlib-Py3Qt5"
    cd "$cwd/sharedlib-Py3Qt5"
    echo_cmd LD_LIBRARY_PATH="$build_dir_path:$cwd/sharedlib-compilation" PYTHONPATH="$build_dir_path" "./sharedlib-sample.py" \&
    LD_LIBRARY_PATH="$build_dir_path:$cwd/sharedlib-compilation" PYTHONPATH="$build_dir_path" "./sharedlib-sample.py" &
    local child_pid=$!
    sleep_kill_wait "$child_pid" 'Py3Qt5'

    printf -- '\nDone.\n'
}

sleep_kill_wait()
{
    local child_pid="$1" ; shift
    # Example: 'C++' or 'Python'
    local description="$1" ; shift

    printf -- 'Checking the sample %s program can run correctly...\n' "$description"
    echo_and_run_cmd sleep 3
    echo_and_run_cmd kill -TERM "$child_pid"
    echo_and_run_cmd wait
}

echo_and_run_cmd()
{
    echo_cmd "$@"
    "$@"
}

echo_cmd()
{
    printf -- '\n$'
    local arg=''
    for arg in "$@"
    do
        printf -- ' %q' "$arg"
    done
    printf -- '\n\n'
}

main "$@"

