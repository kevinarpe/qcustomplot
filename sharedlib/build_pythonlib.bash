#!/usr/bin/env bash

main()
{
    local cwd="$(readlink -f "$(dirname "$0")")"
    source "$cwd/buildlib.bash_library"

    local qt_flag="$1" ; shift
    # Optional remaining args in "$@"; might be zero!

    buildlib_echo_cmd cd "$cwd/sharedlib-Py3Qt5"
    cd "$cwd/sharedlib-Py3Qt5"

    # Create Py3Qt5 C++ bindings
    local build_dir_path='build'
    buildlib_echo_and_run_cmd rm -Rf "$build_dir_path"
    buildlib_echo_and_run_cmd mkdir "$build_dir_path"
    buildlib_echo_and_run_cmd './configure.py' "$build_dir_path"

    buildlib_echo_cmd cd "$build_dir_path"
    cd "$build_dir_path"

    # Build Py3Qt5 qcustomplot.so
    buildlib_echo_and_run_cmd make clean
    buildlib_echo_and_run_cmd make "$@"

    # Run Py3Qt5 demo
    buildlib_echo_cmd cd "$cwd/sharedlib-Py3Qt5"
    cd "$cwd/sharedlib-Py3Qt5"
    buildlib_echo_cmd LD_LIBRARY_PATH="$build_dir_path:$cwd/sharedlib-compilation" PYTHONPATH="$build_dir_path" "./sharedlib-sample.py" \&
    LD_LIBRARY_PATH="$build_dir_path:$cwd/sharedlib-compilation" PYTHONPATH="$build_dir_path" "./sharedlib-sample.py" &
    local child_pid=$!
    buildlib_sleep_kill_wait "$child_pid" 'Py3Qt5'

    printf -- '\nDone.\n'
}

main "$@"

