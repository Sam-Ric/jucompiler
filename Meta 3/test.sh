#!/bin/bash

TEST_PATH="tests"
JUCOMPILER="./jucompiler"
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

SUCCESS=0
TOTAL=0

trap 'rm -f "$TEST_PATH"/*.actual' EXIT

for java_file in "$TEST_PATH"/*.java; do
    test_name=$(basename "$java_file" .java)
    expected_out="$TEST_PATH/$test_name.out"
    actual_out="$TEST_PATH/$test_name.actual"
    TOTAL=$((TOTAL+1))

    [ ! -f "$expected_out" ] && continue

    "$JUCOMPILER" -s < "$java_file" > "$actual_out" 2>&1

    if diff -q "$actual_out" "$expected_out" > /dev/null; then
        echo -e "${GREEN}PASS: $test_name${NC}"
        SUCCESS=$((SUCCESS+1))
    else
        echo -e "${RED}FAIL: $test_name${NC}"
    fi
done

echo "Results: ${SUCCESS}/${TOTAL}"
