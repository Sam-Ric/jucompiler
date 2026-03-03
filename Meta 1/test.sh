#!/bin/bash

TEST_PATH="./tests"
JUCOMPILER="./jucompiler"
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

trap 'rm -f "$TEST_PATH"/*.actual' EXIT

for java_file in "$TEST_PATH"/*.java; do
    test_name=$(basename "$java_file" .java)
    expected_out="$TEST_PATH/$test_name.out"
    actual_out="$TEST_PATH/$test_name.actual"

    [ ! -f "$expected_out" ] && continue

    "$JUCOMPILER" -l < "$java_file" > "$actual_out" 2>&1

    if diff -q "$actual_out" "$expected_out" > /dev/null; then
        echo -e "${GREEN}PASS: $test_name${NC}"
    else
        echo -e "${RED}FAIL: $test_name${NC}"
    fi
done