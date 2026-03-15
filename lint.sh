#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${REPO_ROOT}/build"

have_cmd() {
    command -v "$1" >/dev/null 2>&1
}

echo "[*] Repo root: ${REPO_ROOT}"

if ! have_cmd cmake; then
    echo "ERROR: cmake not found in PATH" >&2
    exit 1
fi

if ! have_cmd clang-format; then
    echo "ERROR: clang-format not found in PATH" >&2
    exit 1
fi

if ! have_cmd clang-tidy; then
    echo "ERROR: clang-tidy not found in PATH" >&2
    exit 1
fi

mkdir -p "${BUILD_DIR}"

echo "[*] Configuring CMake..."
cmake -S "${REPO_ROOT}" -B "${BUILD_DIR}"

echo "[*] Collecting source files..."
mapfile -t FILES < <(
    find "${REPO_ROOT}/src" \
        \( -name '*.c' -o -name '*.h' \) \
        ! -name '*_gen.c' \
        ! -name '*_gen.h' \
        ! -path '*/preview-build/*' \
        ! -path '*/preview-bin/*' \
        | sort
)

if [ "${#FILES[@]}" -eq 0 ]; then
    echo "ERROR: no source files found" >&2
    exit 1
fi

echo "[*] Running clang-format..."
clang-format -i "${FILES[@]}"

echo "[*] Running clang-tidy..."
clang-tidy -p "${BUILD_DIR}" "${FILES[@]}"

echo "[*] Done."