#!/bin/bash

set -e

LIBS_DIR="libs"
PICO_SDK_DIR="${LIBS_DIR}/pico-sdk"
PICO_SDK_VERSION="2.2.0"

echo "Setting up Pico SDK..."

if [ -d "${PICO_SDK_DIR}" ]; then
    echo "Pico SDK already exists in ${PICO_SDK_DIR}"
    exit 0
fi

mkdir -p "${LIBS_DIR}"

echo "Cloning Pico SDK v${PICO_SDK_VERSION}..."
cd "${LIBS_DIR}"
git clone --depth 1 --branch "${PICO_SDK_VERSION}" https://github.com/raspberrypi/pico-sdk.git

echo "Initializing submodules..."
cd pico-sdk
git submodule update --init

echo "Pico SDK setup complete!"