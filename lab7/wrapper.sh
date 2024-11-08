#! /usr/bin/env bash

# Leaving script by errors
set -o errexit
set -o nounset
set -o pipefail

# Directory with generated tests
readonly TESTS_DIR=$(pwd)/tests

# Success logger
function log_success()
{
    local message=${1}
    _log "${message} [\033[32m OK \033[37m]"
}

# Info logger
function log_info()
{
    local message=${1}
    _log "${message}"
}

# Error logger
function log_error()
{
    local message=${1}
    _log "${message} [\033[0m\033[31m FAIL \033[37m]"
}

# Logger of level, date and message
function _log()
{
    local message=${1}
    local now=$(date +"%Y-%m-%d %H:%M:%S")
    echo -e "[${now}] ${message}"
}

# Main function
function main()
{
    # Binary of main file
    local bin=solution
    # Binary of test generator
    local generating_bin=test_generator.py

    # Compiling with Makefile stage
    log_info "Stage #1. Compiling..."
    # Running Makefile
    if ! make ; then
        log_error "Failed to compile"
        return 1
    fi
    log_success "Programm compiled successfully"

    # Test generating stage
    log_info "Stage #2. Test generating..."
    mkdir ${TESTS_DIR}
    # Running test generator
    if ! ./${generating_bin} ${TESTS_DIR} ; then
        log_error "Failed to generate tests"
        return 1
    fi
    log_success "Tests were generated successfully"

    # Test checking stage
    log_info "Stage #3. Checking..."
    # For each file in test directory
    for test_file in $( ls ${TESTS_DIR}/*.t ) ; do
        # Temporary output file
        local tmp_output=tmp
        local diff_output=diffo
        echo > ${diff_output}
        # Running of porgramm with test and output files
        if ! ./${bin} < ${test_file} > ${tmp_output} ; then
            log_error "Failed to run test"
            return 1
        fi
        # File with answers
        local answer_file=${test_file%.*}.a
        # Finding differences between our output and answers
        if ! diff -u ${tmp_output} ${answer_file} >> ${diff_output}; then
            log_error "Failed to check test ${test_file}"
        else
            log_success "${test_file}"
        fi
    done

    # Trash cleaning
    make clear
    rm diffo
    rm -rf ${TESTS_DIR}
    rm tmp
}

# Entrypoint
main