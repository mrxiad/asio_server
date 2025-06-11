#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
OUTPUT_DIR="$PROJECT_ROOT/output/lib"
mkdir -p "$OUTPUT_DIR"

echo "Building lib_public.so"

g++ -fPIC -shared "$PROJECT_ROOT/public/_public.cpp" -o "$OUTPUT_DIR/lib_public.so"

echo "Building lib_mysql.so"

g++ -fPIC -shared "$PROJECT_ROOT/public/db/_mysql.cpp" \
    -I/usr/include/mysql -L/usr/lib/x86_64-linux-gnu -lmysqlclient \
    -o "$OUTPUT_DIR/lib_mysql.so"

echo "Done. Libraries are in $OUTPUT_DIR"
