#!/bin/bash
# This uses bash variable substitution in a few places
# 1. getting the cmake directory for running cpack with an absolute path (chocolatey has an unfortunately named alias)

# Unset VCPKG_ROOT for GitHub actions environment
unset VCPKG_ROOT

mkdir build && cd build || exit
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=17 -DHELICS_ZMQ_SUBPROJECT=ON -DHELICS_ENABLE_PACKAGE_BUILD=ON -DSTATIC_STANDARD_LIB=static -DHELICS_BUILD_APP_EXECUTABLES=OFF -DHELICS_BUILD_APP_LIBRARY=OFF -DHELICS_BUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF ..
cmake --build . --config Release
cpack_dir="$(command -v cmake)"
cpack_dir="${cpack_dir%/cmake}"
"${cpack_dir}/cpack" -G "TGZ" -C Release -B "$(pwd)/../artifact"
cd ../artifact || exit
rm -rf _CPack_Packages
ARCHIVE_FILE="$(ls Helics-*)"
mv "$ARCHIVE_FILE" "${ARCHIVE_FILE/Helics-/Helics-shared-}"
