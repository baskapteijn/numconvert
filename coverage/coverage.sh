#!/usr/bin/env bash
# coverage.sh — Generate coverage for numconvert

set -euo pipefail

# Directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
COV_DIR="$SCRIPT_DIR/coverage"
BIN_NAME="numconvert_cov"

mkdir -p "$COV_DIR"

# Verbose flag
VERBOSE=0
if [[ "${1:-}" == "-v" ]]; then
    VERBOSE=1
fi

echo "==> Cleaning old coverage artifacts"
rm -f -- "$SCRIPT_DIR"/*.gcda "$SCRIPT_DIR"/*.gcno "$COV_DIR"/*.info
rm -rf "$COV_DIR/html"

echo "==> Building with coverage flags"
if [ -f "$SCRIPT_DIR/Makefile" ]; then
    make -C "$SCRIPT_DIR" CFLAGS="--coverage -O0 -g" >/dev/null
else
    gcc --coverage -O0 -g -o "$SCRIPT_DIR/$BIN_NAME" "$SCRIPT_DIR"/../src/*.c >/dev/null
fi

echo "==> Running test inputs (output suppressed by default)"

# 1️⃣ Run binary with no arguments
if [[ "$VERBOSE" -eq 1 ]]; then
    echo ">>> Running: $SCRIPT_DIR/$BIN_NAME"
    "$SCRIPT_DIR/$BIN_NAME" || true
else
    "$SCRIPT_DIR/$BIN_NAME" >/dev/null 2>&1 || true
fi

# 2️⃣ Run all test inputs
TEST_INPUTS=(
    1111111111111111111111111111111111111111111111111111111111111111b
    111111111111111111111111111111111111111111111111111111111111111b
    18446744073709551615
    184467440737095516154
    0xffffffffffffffff
    0xfffffffffffffffff
    0x0
    b
    h
    111h
    111111111111111111111111111111111111111111111111111111111111111111
    112b
    Aah
    18446744073709551616
    17446744073709551616
    1744674407370955161
    0xg
    ""
    0b
    -1
    0x/
    0z
)

for input in "${TEST_INPUTS[@]}"; do
    if [[ "$VERBOSE" -eq 1 ]]; then
        echo ">>> Running: $SCRIPT_DIR/$BIN_NAME $input"
        "$SCRIPT_DIR/$BIN_NAME" "$input" || true
    else
        "$SCRIPT_DIR/$BIN_NAME" "$input" >/dev/null 2>&1 || true
    fi
done

echo "==> Capturing coverage data"
lcov --capture --directory "$SCRIPT_DIR" --output-file "$COV_DIR/coverage.info" --rc branch_coverage=1 >/dev/null

echo "==> Generating HTML report"
genhtml "$COV_DIR/coverage.info" --output-directory "$COV_DIR/html" --branch-coverage >/dev/null

lcov --summary "$COV_DIR/coverage.info"

echo "✅ HTML report available at: $COV_DIR/html/index.html"
