#!/usr/bin/env bash
# NirvanaEdit — dev build script
set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="$ROOT/build"

echo "=== NirvanaEdit Build ==="
echo "Root: $ROOT"
echo "Build: $BUILD"

mkdir -p "$BUILD"
cd "$BUILD"

cmake .. -DCMAKE_BUILD_TYPE=Debug -DNV_BUILD_TESTS=ON
cmake --build . -j"$(nproc)"

echo ""
echo "=== Running Tests ==="
ctest --output-on-failure

echo ""
echo "=== Build Complete ==="
