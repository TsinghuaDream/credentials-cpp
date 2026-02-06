#!/bin/bash
# Test credentials-cpp with different C++ standards

set -e  # Exit on error

STANDARDS=(11 14 17 20)
PROJECT_ROOT=$(cd "$(dirname "$0")" && pwd)

echo "🚀 Testing credentials-cpp with C++ standards: ${STANDARDS[*]}"
echo "Project root: $PROJECT_ROOT"
echo ""

for std in "${STANDARDS[@]}"; do
    echo "=========================================="
    echo "📦 Testing C++$std"
    echo "=========================================="
    
    BUILD_DIR="build-test-cpp$std"
    
    # Clean previous build
    rm -rf "$BUILD_DIR"
    
    # Configure
    echo "🔧 Configuring for C++$std..."
    cmake -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_STANDARD=$std \
        -DBUILD_SHARED_LIBS=ON \
        -DENABLE_UNIT_TESTS=OFF
    
    # Build
    echo "🔨 Building C++$std..."
    cmake --build "$BUILD_DIR" --config Release
    
    echo "✅ C++$std build successful!"
    echo ""
done

echo "=========================================="
echo "🎉 All C++ standards tested successfully!"
echo "=========================================="
