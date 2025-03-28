# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) 2017-2025, Battelle Memorial Institute; Lawrence Livermore
# National Security, LLC; Alliance for Sustainable Energy, LLC.
# See the top-level NOTICE for additional details.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cmake_minimum_required(VERSION 3.10..3.23)

project(HELICS-package_config-tests)

include(CTest)

find_package(PkgConfig)

# Test the CXX shared library target
if(HELICS_BUILD_CXX_SHARED_LIB)
    pkg_check_modules(HELICSCPP REQUIRED IMPORTED_TARGET helicscpp)
    add_executable(shared-library-test-exe ../pkg_test_code/main.cpp)
    target_link_libraries(shared-library-test-exe PkgConfig::HELICSCPP)

    add_test(NAME shared-library-test COMMAND shared-library-test-exe)
    set_property(TEST shared-library-test PROPERTY PASS_REGULAR_EXPRESSION "${HELICS_VERSION}")

    # Test the helics_app static library target
    if(HELICS_BUILD_APP_LIBRARY)
        pkg_check_modules(HELICS_APPS REQUIRED IMPORTED_TARGET helicscpp-apps)
        add_executable(apps-library-test-exe ../pkg_test_code/main-apps.cpp)
        target_link_libraries(apps-library-test-exe PkgConfig::HELICS_APPS)

        add_test(NAME apps-library-test COMMAND apps-library-test-exe)
        set_property(TEST apps-library-test PROPERTY PASS_REGULAR_EXPRESSION "${HELICS_VERSION}")
    endif()

endif()

# Test the C and C++98 targets
if(NOT HELICS_DISABLE_C_SHARED_LIB)

    pkg_check_modules(HELICS REQUIRED IMPORTED_TARGET helics)
    add_executable(c-shared-library-test-exe ../pkg_test_code/main.c)
    target_link_libraries(c-shared-library-test-exe PkgConfig::HELICS)

    add_test(NAME c-shared-library-test COMMAND c-shared-library-test-exe)
    set_property(TEST c-shared-library-test PROPERTY PASS_REGULAR_EXPRESSION "${HELICS_VERSION}")

    add_executable(cpp98-shared-library-test-exe ../pkg_test_code/main98.cpp)
    target_link_libraries(cpp98-shared-library-test-exe PkgConfig::HELICS)
    set_property(TARGET cpp98-shared-library-test-exe PROPERTY CXX_STANDARD 98)

    add_test(NAME cpp98-shared-library-test COMMAND cpp98-shared-library-test-exe)
    set_property(
        TEST cpp98-shared-library-test PROPERTY PASS_REGULAR_EXPRESSION "${HELICS_VERSION}"
    )
endif()
