#!/usr/bin/env bash
#

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get -qq update > /dev/null 2>&1
sudo apt-get install -y g++ cmake make lcov ninja-build

# multilib amd64->x86 cross compiling
if [ "${TARGET_CPU}" == "x86" ]; then
    sudo dpkg --add-architecture i386
    sudo apt-get -qq update > /dev/null 2>&1
    # g++-multilib
    sudo apt-get install -y gcc-multilib
    sudo apt-get install -y g++-multilib
fi

# CMAKE install
#CMAKE_VERSION="3.17.2"
#CMAKE_VERSION_DIR="v3.17"
#CMAKE_OS="Linux-x86_64"
#CMAKE_TAR="cmake-${CMAKE_VERSION}-${CMAKE_OS}.tar.gz"
#CMAKE_URL="http://www.cmake.org/files/${CMAKE_VERSION_DIR}/${CMAKE_TAR}"
#CMAKE_DIR="$(pwd)/cmake-${CMAKE_VERSION}"
#wget --quiet "${CMAKE_URL}"
#mkdir -p "${CMAKE_DIR}"
#tar --strip-components=1 -xzf "${CMAKE_TAR}" -C "${CMAKE_DIR}"
#export PATH="${CMAKE_DIR}/bin:${PATH}"